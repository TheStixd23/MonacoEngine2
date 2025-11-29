#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class InputLayout
 * @brief Administra un @c ID3D11InputLayout que define el formato de vértices en Direct3D 11.
 *
 * Facilita la creación, uso y liberación del recurso asociado, encargado de
 * vincular los datos del Vertex Buffer con las entradas del Vertex Shader.
 * 
 *  
 * @author Hannin
 */
class InputLayout {
public:
    InputLayout() = default;
    ~InputLayout() = default;

    /**
     * @brief Crea el Input Layout a partir de una descripción y el bytecode de un Vertex Shader.
     * @param device Dispositivo Direct3D para la creación.
     * @param Layout Descripción de los elementos de entrada.
     * @param VertexShaderData Bytecode del Vertex Shader.
     * @return @c S_OK si fue exitoso.
     */
    HRESULT init(Device& device,
        std::vector<D3D11_INPUT_ELEMENT_DESC>& Layout,
        ID3DBlob* VertexShaderData);

    /**
     * @brief Actualiza el recurso (placeholder).
     */
    void update();

    /**
     * @brief Establece el Input Layout en el contexto gráfico.
     * @param deviceContext Contexto donde se aplicará.
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Libera el recurso asociado.
     */
    void destroy();

public:
    /// Recurso COM de Direct3D 11.
    ID3D11InputLayout* m_inputLayout = nullptr;
};
