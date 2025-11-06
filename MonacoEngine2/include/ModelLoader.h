#pragma once
#include "Prerequisites.h"

// Declaraciones anticipadas
class MeshComponent;
class Device;

/**
 * @class ModelLoader
 * @brief Carga y procesa mallas desde archivos OBJ.
 */
class ModelLoader {
public:
    /** Constructor por defecto. */
    ModelLoader() = default;

    /** Destructor por defecto. */
    ~ModelLoader() = default;

    /** Inicializa el cargador. */
    HRESULT init();

    /**
     * @brief Carga un modelo OBJ.
     * @param fileName Ruta del archivo (ej. "model.obj").
     * @param mesh Malla a poblar.
     * @param invertTexCoordY Invierte coordenada Y de textura.
     */
    HRESULT loadFromFile(const std::string& fileName, MeshComponent& mesh, bool invertTexCoordY = true);

    /** Libera los recursos usados. */
    void destroy();

private:
    /**
     * @brief Procesa una cara (f) y triangula vértices.
     */
    void parseFace(std::stringstream& ss,
                    std::vector<SimpleVertex>& out_vertices,
                    std::vector<unsigned int>& out_indices,
                    const std::vector<XMFLOAT3>& temp_positions,
                    const std::vector<XMFLOAT2>& temp_texcoords,
                    const std::vector<XMFLOAT3>& temp_normals,
                    std::map<std::string, unsigned int>& vertexMap,
                    bool invertTexCoordY);

    /**
     * @brief Parsea un combo v/vt/vn y genera su índice.
     */
    unsigned int parseVertexCombo(const std::string& comboToken,
                                    std::vector<SimpleVertex>& out_vertices,
                                    const std::vector<XMFLOAT3>& temp_positions,
                                    const std::vector<XMFLOAT2>& temp_texcoords,
                                    const std::vector<XMFLOAT3>& temp_normals,
                                    std::map<std::string, unsigned int>& vertexMap,
                                    bool invertTexCoordY);
};
