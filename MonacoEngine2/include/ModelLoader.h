#pragma once
#include "Prerequisites.h"

// Declaraciones anticipadas
class MeshComponent;
class Device;

/**
 * @class ModelLoader
 * @brief Administra la carga de datos de geometría (mallas) desde archivos OBJ.
 *
 * Esta clase se encarga de leer y parsear manualmente archivos .obj
 * (v, vt, vn, f) y poblarlos en un MeshComponent.
 * Realiza triangulación de polígonos y de-duplicación de vértices.
 */
class
    ModelLoader {
public:
    /**
     * @brief Constructor por defecto.
     */
    ModelLoader() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~ModelLoader() = default;

    /**
     * @brief Inicializa el cargador de modelos.
     * @return S_OK si fue exitoso.
     */
    HRESULT
        init();

    /**
     * @brief Carga un modelo desde un archivo .obj.
     *
     * @param fileName  Ruta del archivo a cargar (ej. "model.obj").
     * @param mesh      Referencia al componente de malla que se poblará.
     * @param invertTexCoordY Invierte la coordenada Y de la textura (útil para D3D).
     * @return @c S_OK si fue exitoso; código @c HRESULT en caso contrario.
     */
    HRESULT
        loadFromFile(const std::string& fileName, MeshComponent& mesh, bool invertTexCoordY = true);


    /**
     * @brief Libera recursos del cargador.
     */
    void
        destroy();

private:
    /**
     * @brief Parsea un token de cara (ej. "f v1/vt1/vn1 v2/vt2/vn2 ...")
     * y realiza la triangulación.
     */
    void
        parseFace(std::stringstream& ss,
            std::vector<SimpleVertex>& out_vertices,
            std::vector<unsigned int>& out_indices,
            const std::vector<XMFLOAT3>& temp_positions,
            const std::vector<XMFLOAT2>& temp_texcoords,
            const std::vector<XMFLOAT3>& temp_normals,
            std::map<std::string, unsigned int>& vertexMap,
            bool invertTexCoordY);

    /**
     * @brief Parsea un combo de índices (ej. "v/vt/vn") y devuelve el índice final.
     */
    unsigned int
        parseVertexCombo(const std::string& comboToken,
            std::vector<SimpleVertex>& out_vertices,
            const std::vector<XMFLOAT3>& temp_positions,
            const std::vector<XMFLOAT2>& temp_texcoords,
            const std::vector<XMFLOAT3>& temp_normals,
            std::map<std::string, unsigned int>& vertexMap,
            bool invertTexCoordY);
};