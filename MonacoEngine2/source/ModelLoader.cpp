#include "ModelLoader.h"
#include "MeshComponent.h"
#include "Device.h" // No se usa directamente aquí, pero mantiene la consistencia

HRESULT
ModelLoader::init() {
    MESSAGE("ModelLoader", "init", "ModelLoader (Manual OBJ Parser) inicializado.");
    return S_OK;
}

void
ModelLoader::destroy() {
    MESSAGE("ModelLoader", "destroy", "ModelLoader destruido.");
}

HRESULT
ModelLoader::loadFromFile(const std::string& fileName, MeshComponent& mesh, bool invertTexCoordY) {

    // Almacenamiento temporal para los atributos leídos del OBJ
    std::vector<XMFLOAT3> temp_positions; // 'v'
    std::vector<XMFLOAT2> temp_texcoords; // 'vt'
    std::vector<XMFLOAT3> temp_normals;   // 'vn'

    // Almacenamiento final para la malla (MeshComponent)
    std::vector<SimpleVertex> out_vertices;
    std::vector<unsigned int> out_indices;

    // Mapa para de-duplicación de vértices.
    // La clave es el string "v/vt/vn" (ej. "5/2/3")
    // El valor es el índice en el 'out_vertices' final.
    std::map<std::string, unsigned int> vertexMap;

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::string errorMsg = "No se pudo abrir el archivo OBJ: " + fileName;
        ERROR("ModelLoader", "loadFromFile", errorMsg.c_str());
        return E_FAIL;
    }

    std::string line;
    std::string prefix;

    while (std::getline(file, line)) {
        // Ignorar líneas vacías o comentarios
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Usar un stringstream para parsear la línea
        std::stringstream ss(line);
        ss >> prefix;

        if (prefix == "v") { // Posición de vértice
            XMFLOAT3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "vt") { // Coordenada de textura
            XMFLOAT2 tex;
            ss >> tex.x >> tex.y;
            temp_texcoords.push_back(tex);
        }
        else if (prefix == "vn") { // Normal de vértice
            XMFLOAT3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        }
        else if (prefix == "f") { // Cara
            // Pasa el stringstream (sin el prefijo "f ") a la función de parseo
            parseFace(ss,
                out_vertices,
                out_indices,
                temp_positions,
                temp_texcoords,
                temp_normals,
                vertexMap,
                invertTexCoordY);
        }
        // Ignorar otros prefijos ( 's', 'o', 'g', 'usemtl', 'mtllib')
    }

    file.close();

    // Poblar el MeshComponent con los datos finales
    mesh.m_vertex = out_vertices;
    mesh.m_index = out_indices;
    mesh.m_numVertex = static_cast<int>(out_vertices.size());
    mesh.m_numIndex = static_cast<int>(out_indices.size());
    mesh.m_name = fileName;

    std::string msg = "Modelo cargado: " + fileName + ". Vértices únicos: "
        + std::to_string(mesh.m_numVertex) + ", Índices: " + std::to_string(mesh.m_numIndex);
    MESSAGE("ModelLoader", "loadFromFile", msg.c_str());

    return S_OK;
}

void
ModelLoader::parseFace(std::stringstream& ss,
    std::vector<SimpleVertex>& out_vertices,
    std::vector<unsigned int>& out_indices,
    const std::vector<XMFLOAT3>& temp_positions,
    const std::vector<XMFLOAT2>& temp_texcoords,
    const std::vector<XMFLOAT3>& temp_normals,
    std::map<std::string, unsigned int>& vertexMap,
    bool invertTexCoordY)
{
    std::string comboToken;
    std::vector<unsigned int> faceIndices; // Índices finales para esta cara

    // 1. Leer todos los combos de vértices de la cara (ej. "1/1/1", "2/2/1", "3/3/1", "4/4/1")
    while (ss >> comboToken) {
        // parseVertexCombo busca el combo en el map. Si no existe, lo crea,
        // lo añade a out_vertices y devuelve su nuevo índice.
        unsigned int index = parseVertexCombo(comboToken,
            out_vertices,
            temp_positions,
            temp_texcoords,
            temp_normals,
            vertexMap,
            invertTexCoordY);
        faceIndices.push_back(index);
    }

    // 2. Realizar triangulación (Fan Triangulation)
    // Si es un triángulo (3 vértices), solo añade 0, 1, 2
    // Si es un quad (4 vértices), añade 0, 1, 2 y luego 0, 2, 3
    // Si es un n-gon (N vértices), añade 0, 1, 2; 0, 2, 3; 0, 3, 4; ...
    for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
        out_indices.push_back(faceIndices[0]);
        out_indices.push_back(faceIndices[i]);
        out_indices.push_back(faceIndices[i + 1]);
    }
}

unsigned int
ModelLoader::parseVertexCombo(const std::string& comboToken,
    std::vector<SimpleVertex>& out_vertices,
    const std::vector<XMFLOAT3>& temp_positions,
    const std::vector<XMFLOAT2>& temp_texcoords,
    const std::vector<XMFLOAT3>& temp_normals,
    std::map<std::string, unsigned int>& vertexMap,
    bool invertTexCoordY)
{
    // 1. Verificar si este combo de "v/vt/vn" ya existe (de-duplicación)
    auto it = vertexMap.find(comboToken);
    if (it != vertexMap.end()) {
        // Si existe, simplemente re-usa su índice
        return it->second;
    }

    // 2. Si no existe, es un vértice nuevo. Hay que parsear el combo.
    SimpleVertex newVertex = {};
    int vIdx = -1, vtIdx = -1, vnIdx = -1;

    std::stringstream ss(comboToken);
    std::string segment;
    int part = 0;

    // Parsear el string "v/vt/vn".
    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            // El formato OBJ es 1-based, así que restamos 1 para hacerlo 0-based.
            int index = std::stoi(segment) - 1;

            if (part == 0)      vIdx = index;
            else if (part == 1) vtIdx = index;
            else if (part == 2) vnIdx = index;
        }
        part++;
    }

    // Manejar el caso "v//vn" donde part==1 (vt) se saltó
    // y el loop anterior lo interpretó mal (puso vnIdx en vtIdx)
    if (part == 2 && comboToken.find("//") != std::string::npos) {
        // vIdx ya está bien
        vnIdx = vtIdx; // El índice que se leyó como 'vt' era en realidad 'vn'
        vtIdx = -1;    // No había 'vt'
    }


    // 3. Ensamblar el SimpleVertex
    if (vIdx >= 0 && vIdx < temp_positions.size()) {
        newVertex.Pos = temp_positions[vIdx];
    }
    else {
        ERROR("ModelLoader", "parseVertexCombo", "Índice 'v' fuera de rango.");
        newVertex.Pos = { 0.0f, 0.0f, 0.0f };
    }

    if (vtIdx >= 0 && vtIdx < temp_texcoords.size()) {
        newVertex.Tex = temp_texcoords[vtIdx];
        if (invertTexCoordY) {
            newVertex.Tex.y = 1.0f - newVertex.Tex.y; // Invertir Y para D3D
        }
    }
    else {
        newVertex.Tex = { 0.0f, 0.0f }; // Default si no hay 'vt'
    }

    if (vnIdx >= 0 && vnIdx < temp_normals.size()) {
        newVertex.Norm = temp_normals[vnIdx];
    }
    else {
        // Si no hay normal, ponemos una por defecto. 
        // Una mejor implementación calcularía las normales aquí.
        newVertex.Norm = { 0.0f, 1.0f, 0.0f }; // Normal apuntando hacia arriba
    }

    // 4. Añadir el nuevo vértice al buffer final
    out_vertices.push_back(newVertex);
    unsigned int newIndex = static_cast<unsigned int>(out_vertices.size() - 1);

    // 5. Guardar el nuevo índice en el mapa para de-duplicar
    vertexMap[comboToken] = newIndex;

    return newIndex;
}