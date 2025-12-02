#pragma once
/**
 * @file ResourceManager.h
 * @brief Definición de la clase ResourceManager.
 *
 * Sistema centralizado para la gestión del ciclo de vida de los recursos (Texturas, Modelos, Shaders, etc.).
 * Implementa el patrón Singleton para acceso global y el patrón Flyweight para evitar duplicidad de recursos en memoria.
 *
 * @author MonacoEngine Team
 */

#include "Prerequisites.h"
#include "IResource.h"

 /**
  * @class ResourceManager
  * @brief Gestor de recursos Singleton que administra la carga, acceso y liberación de assets.
  *
  * Esta clase mantiene un registro (mapa) de todos los recursos cargados en memoria.
  * Antes de cargar un recurso nuevo, verifica si ya existe para devolver la instancia existente,
  * optimizando así el uso de memoria y rendimiento.
  */
class ResourceManager {
public:
    /**
     * @brief Constructor por defecto.
     */
    ResourceManager() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~ResourceManager() = default;

    /**
     * @brief Obtiene la instancia única del ResourceManager (Singleton).
     *
     * @return Referencia estática a la instancia única de la clase.
     */
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Eliminar constructor de copia y operador de asignación para garantizar Singleton
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /**
     * @brief Obtiene un recurso existente o lo carga si no está en caché.
     *
     * Este es el método principal para solicitar recursos. Verifica si la clave (@p key)
     * ya existe en el mapa. Si existe y está cargado, devuelve el puntero compartido existente.
     * Si no, crea una nueva instancia de tipo @p T, llama a su método load() e init(),
     * y lo almacena en el mapa.
     *
     * @tparam T        Tipo del recurso a cargar (debe heredar de @c IResource).
     * @tparam Args     Tipos de los argumentos adicionales para el constructor del recurso.
     * @param key       Identificador único para el recurso (usualmente el nombre o ruta).
     * @param filename  Ruta del archivo desde donde se cargará el recurso.
     * @param args      Argumentos variádicos que se pasarán al constructor del recurso.
     * @return std::shared_ptr<T> Puntero inteligente al recurso solicitado, o @c nullptr si falló la carga.
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> GetOrLoad(const std::string& key,
        const std::string& filename,
        Args&&... args) {

        static_assert(std::is_base_of<IResource, T>::value, "T debe heredar de IResource");

        // 1. ¿Ya existe el recurso en el caché?
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            // Intentar castear al tipo correcto
            auto existing = std::dynamic_pointer_cast<T>(it->second);
            if (existing && existing->GetState() == ResourceState::Loaded) {
                return existing; // Flyweight: reutilizamos la instancia existente
            }
        }

        // 2. No existe o no está cargado -> crearlo y cargarlo
        std::shared_ptr<T> resource = std::make_shared<T>(key, std::forward<Args>(args)...);

        if (!resource->load(filename)) {
            // Error al cargar desde disco
            return nullptr;
        }

        if (!resource->init()) {
            // Error al inicializar (ej. crear recursos de DX11)
            return nullptr;
        }

        // 3. Guardar en el caché y devolver
        m_resources[key] = resource;
        return resource;
    }

    /**
     * @brief Obtiene un recurso ya cargado sin intentar cargarlo de nuevo.
     *
     * Útil para consultar recursos que se asume que ya existen en memoria.
     *
     * @tparam T  Tipo al cual castear el recurso.
     * @param key Identificador único del recurso.
     * @return std::shared_ptr<T> Puntero al recurso o @c nullptr si no se encuentra.
     */
    template<typename T>
    std::shared_ptr<T> Get(const std::string& key) const
    {
        auto it = m_resources.find(key);
        if (it == m_resources.end()) return nullptr;

        return std::dynamic_pointer_cast<T>(it->second);
    }

    /**
     * @brief Libera un recurso específico de la memoria.
     *
     * Llama al método @c unload() del recurso y lo elimina del mapa de gestión.
     *
     * @param key Identificador único del recurso a liberar.
     */
    void Unload(const std::string& key)
    {
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            it->second->unload();
            m_resources.erase(it);
        }
    }

    /**
     * @brief Libera todos los recursos gestionados.
     *
     * Itera sobre todos los recursos almacenados, llama a @c unload() en cada uno
     * y limpia el mapa completo.
     */
    void UnloadAll()
    {
        for (auto& [key, res] : m_resources) {
            if (res) {
                res->unload();
            }
        }
        m_resources.clear();
    }

private:
    /**
     * @brief Mapa hash que almacena los recursos cargados.
     * La clave es un string (ID) y el valor es un puntero compartido al recurso base.
     */
    std::unordered_map<std::string, std::shared_ptr<IResource>> m_resources;
};