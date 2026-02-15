#pragma once
/**
 * @file MeshComponent.h
 * @brief Definición de la clase MeshComponent.
 *
 * Componente del sistema ECS encargado de almacenar los datos geométricos de una entidad.
 * Contiene la información cruda de vértices e índices necesaria para construir buffers de GPU.
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"
#include "ECS\Component.h"

class DeviceContext;

/**
 * @class MeshComponent
 * @brief Componente ECS que almacena la información de geometría (malla) de un actor.
 *
 * Un @c MeshComponent actúa como un contenedor de datos para la geometría.
 * No se encarga de renderizarse a sí mismo directamente (eso lo hacen los sistemas o el renderer
 * usando estos datos), pero provee la información esencial:
 * - Lista de vértices (posición, normal, UV).
 * - Lista de índices para definir triángulos.
 *
 * Se asocia a entidades (Actores) para darles forma visual.
 */
class MeshComponent : public Component {
public:
    /**
     * @brief Constructor por defecto.
     *
     * Inicializa el componente con contadores a cero y establece su tipo como @c ComponentType::MESH.
     */
    MeshComponent(): 
                    m_numVertex(0),
                    m_numIndex(0), 
                    Component(ComponentType::MESH) {}

    /**
     * @brief Destructor virtual.
     */
    virtual ~MeshComponent() = default;

    /**
     * @brief Inicializa el componente.
     * @note Método heredado de Component. Actualmente vacío.
     */
    void init() 
    override {};

    /**
     * @brief Actualiza la lógica del componente.
     * @param deltaTime Tiempo transcurrido.
     * @note Método heredado de Component. Actualmente vacío.
     */
    void update(float deltaTime)
    override {};

    /**
     * @brief Método de renderizado específico del componente.
     * @param deviceContext Contexto gráfico.
     * @note Método heredado de Component. Actualmente vacío (el renderizado suele gestionarse externamente).
     */
    void render(DeviceContext& deviceContext) override {};

    /**
     * @brief Libera recursos.
     * @note Método heredado de Component. Actualmente vacío.
     */
    void destroy() override {};

public:
    /**
     * @brief Nombre identificativo de la malla (útil para debug o instanciado).
     */
    std::string m_name;

    /**
     * @brief Vector que almacena los vértices de la malla.
     * Cada vértice incluye posición, coordenadas de textura, normales, etc.
     */
    std::vector<SimpleVertex> m_vertex;

    /**
     * @brief Vector que almacena los índices para definir la topología (triángulos).
     */
    std::vector<unsigned int> m_index;

    /**
     * @brief Cantidad total de vértices almacenados.
     */
    int m_numVertex;

    /**
     * @brief Cantidad total de índices almacenados.
     */
    int m_numIndex;
};