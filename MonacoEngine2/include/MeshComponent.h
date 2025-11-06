#pragma once
#include "Prerequisites.h"
//#include "ECS\Component.h"
class DeviceContext;

/**
 * @class MeshComponent
 * @brief Componente ECS que almacena la geometría (malla) de un actor.
 *
 * Contiene vértices e índices que describen la forma del objeto y se asocia a entidades del sistema ECS.
 *
 * @author Hannin
 */
class MeshComponent /*: public Component*/ {
public:
    /// Constructor por defecto.
    MeshComponent() : m_numVertex(0), m_numIndex(0)/*, Component(ComponentType::MESH)*/ {}

    /// Destructor por defecto.
    virtual ~MeshComponent() = default;

    /// Inicializa el componente de malla (placeholder).
    void init() /*override {}*/;

    /// Actualiza la malla (placeholder).
    void update(float deltaTime)/* override {}*/;

    /// Renderiza la malla mediante el contexto del dispositivo.
    void render(DeviceContext& deviceContext) /*override {}*/;

    /// Libera los recursos asociados (placeholder).
    void destroy() /*override {}*/;

public:
    std::string m_name;                  ///< Nombre de la malla.
    std::vector<SimpleVertex> m_vertex;  ///< Lista de vértices.
    std::vector<unsigned int> m_index;   ///< Lista de índices.
    int m_numVertex;                     ///< Total de vértices.
    int m_numIndex;                      ///< Total de índices.
};
