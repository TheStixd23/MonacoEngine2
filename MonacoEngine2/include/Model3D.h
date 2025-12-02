#pragma once
/**
 * @file Model3D.h
 * @brief Definición de la clase Model3D.
 *
 * Gestiona la carga y almacenamiento de modelos 3D (mallas).
 * Soporta principalmente el formato FBX mediante el uso del SDK de Autodesk FBX,
 * procesando nodos, mallas y materiales para convertirlos en componentes del motor.
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"
#include "IResource.h"
#include "MeshComponent.h"
#include "fbxsdk.h"

 /**
  * @enum ModelType
  * @brief Enumera los tipos de formatos de modelos soportados.
  */
enum ModelType {
    OBJ,    ///< Formato Wavefront OBJ (actualmente no implementado completamente).
    FBX     ///< Formato Autodesk FBX (soporte principal).
};

/**
 * @class Model3D
 * @brief Recurso que representa un modelo 3D cargado desde un archivo.
 *
 * Hereda de IResource. Se encarga de utilizar el SDK de FBX para leer la geometría,
 * materiales y texturas de un archivo y generar una lista de @c MeshComponent
 * listos para ser renderizados.
 */
class Model3D : public IResource {
public:
    /**
     * @brief Constructor de Model3D.
     *
     * Inicializa el recurso, establece su tipo y dispara la carga automática del archivo.
     *
     * @param name      Nombre o ruta del archivo del modelo.
     * @param modelType Tipo de formato del modelo (ej. FBX).
     */
    Model3D(const std::string& name, ModelType modelType)
        : IResource(name), m_modelType(modelType), lSdkManager(nullptr), lScene(nullptr) {
        SetType(ResourceType::Model3D);
        load(name);
    }

    /**
     * @brief Destructor por defecto.
     */
    ~Model3D() = default;

    /**
     * @brief Carga el modelo desde la ruta especificada.
     *
     * Implementación de @c IResource::load. Llama internamente a la lógica de carga
     * específica (como @c LoadFBXModel) dependiendo del tipo de modelo.
     *
     * @param path Ruta al archivo del modelo.
     * @return true si la carga fue exitosa, false en caso contrario.
     */
    bool load(const std::string& path) override;

    /**
     * @brief Inicializa el recurso.
     *
     * Implementación de @c IResource::init.
     * @return true si la inicialización fue correcta.
     */
    bool init() override;

    /**
     * @brief Descarga y libera la memoria del modelo.
     *
     * Libera los recursos del SDK de FBX y limpia los vectores de mallas.
     */
    void unload() override;

    /**
     * @brief Obtiene el tamaño en bytes ocupado por el recurso.
     *
     * @return size_t Tamaño aproximado en memoria.
     */
    size_t getSizeInBytes() const override;

    /**
     * @brief Obtiene las mallas generadas a partir del modelo.
     *
     * @return Referencia constante al vector de componentes de malla (@c MeshComponent).
     */
    const std::vector<MeshComponent>& GetMeshes() const { return m_meshes; }

    /* --- CARGADOR DE MODELOS FBX --- */

    /**
     * @brief Inicializa el gestor del SDK de FBX.
     *
     * Crea el @c FbxManager y los ajustes de I/O necesarios para importar archivos.
     * @return true si se inicializó correctamente.
     */
    bool InitializeFBXManager();

    /**
     * @brief Carga un archivo FBX y procesa su contenido.
     *
     * Orquesta todo el proceso de importación: inicializa el SDK, importa la escena,
     * y recorre los nodos para extraer geometría.
     *
     * @param filePath Ruta al archivo .fbx.
     * @return std::vector<MeshComponent> Lista de mallas extraídas del archivo.
     */
    std::vector<MeshComponent> LoadFBXModel(const std::string& filePath);

    /**
     * @brief Procesa un nodo de la jerarquía de la escena FBX.
     *
     * Método recursivo que recorre el árbol de nodos. Si el nodo contiene atributos
     * de malla, llama a @c ProcessFBXMesh; luego procesa los hijos del nodo.
     *
     * @param node Puntero al nodo FBX actual.
     */
    void ProcessFBXNode(FbxNode* node);

    /**
     * @brief Procesa un nodo que contiene geometría (Mesh).
     *
     * Extrae vértices, índices, normales, coordenadas UV y materiales de la malla FBX
     * y crea un objeto @c MeshComponent con estos datos.
     *
     * @param node Puntero al nodo FBX que contiene la malla.
     */
    void ProcessFBXMesh(FbxNode* node);

    /**
     * @brief Procesa los materiales asociados a una malla FBX.
     *
     * Extrae información como nombres de texturas (Difusa, Normal, etc.) del material.
     *
     * @param material Puntero al material FBX.
     */
    void ProcessFBXMaterials(FbxSurfaceMaterial* material);

    /**
     * @brief Obtiene los nombres de los archivos de textura encontrados en el modelo.
     *
     * @return std::vector<std::string> Lista de nombres de texturas.
     */
    std::vector<std::string> GetTextureFileNames() const { return textureFileNames; }

private:
    FbxManager* lSdkManager;     ///< Puntero al gestor principal del SDK de FBX.
    FbxScene* lScene;            ///< Puntero a la escena FBX importada.
    std::vector<std::string> textureFileNames; ///< Almacena rutas de texturas extraídas del archivo.

public:
    ModelType m_modelType;       ///< Tipo de modelo (FBX/OBJ).
    std::vector<MeshComponent> m_meshes; ///< Lista de componentes de malla que componen el modelo.
};