#include "ModelLoader.h"
#include "MeshComponent.h"
#include "Device.h"

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

    std::vector<XMFLOAT3> temp_positions;
    std::vector<XMFLOAT2> temp_texcoords;
    std::vector<XMFLOAT3> temp_normals;

    std::vector<SimpleVertex> out_vertices;
    std::vector<unsigned int> out_indices;

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
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        ss >> prefix;

        if (prefix == "v") {
            XMFLOAT3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
        }
        else if (prefix == "vt") {
            XMFLOAT2 tex;
            ss >> tex.x >> tex.y;
            temp_texcoords.push_back(tex);
        }
        else if (prefix == "vn") {
            XMFLOAT3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            temp_normals.push_back(norm);
        }
        else if (prefix == "f") {
            parseFace(ss,
                out_vertices,
                out_indices,
                temp_positions,
                temp_texcoords,
                temp_normals,
                vertexMap,
                invertTexCoordY);
        }
    }

    file.close();

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
    std::vector<unsigned int> faceIndices;

    while (ss >> comboToken) {
        unsigned int index = parseVertexCombo(comboToken,
                                out_vertices,
                                temp_positions,
                                temp_texcoords,
                                temp_normals,
                                vertexMap,
                                invertTexCoordY);
        faceIndices.push_back(index);
    }

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
    auto it = vertexMap.find(comboToken);
    if (it != vertexMap.end()) {
        return it->second;
    }

    SimpleVertex newVertex = {};
    int vIdx = -1, vtIdx = -1, vnIdx = -1;

    std::stringstream ss(comboToken);
    std::string segment;
    int part = 0;

    while (std::getline(ss, segment, '/')) {
        if (!segment.empty()) {
            int index = std::stoi(segment) - 1;

            if (part == 0)      vIdx = index;
            else if (part == 1) vtIdx = index;
            else if (part == 2) vnIdx = index;
        }
        part++;
    }

    if (part == 2 && comboToken.find("//") != std::string::npos) {
        vnIdx = vtIdx;
        vtIdx = -1;
    }

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
            newVertex.Tex.y = 1.0f - newVertex.Tex.y;
        }
    }
    else {
        newVertex.Tex = { 0.0f, 0.0f };
    }

    if (vnIdx >= 0 && vnIdx < temp_normals.size()) {
        newVertex.Norm = temp_normals[vnIdx];
    }
    else {
        newVertex.Norm = { 0.0f, 1.0f, 0.0f };
    }

    out_vertices.push_back(newVertex);
    unsigned int newIndex = static_cast<unsigned int>(out_vertices.size() - 1);

    vertexMap[comboToken] = newIndex;

    return newIndex;
}
