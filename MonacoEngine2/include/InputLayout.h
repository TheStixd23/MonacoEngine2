#pragma once
/**
 * @file InputLayout.h
 * @brief Definición de la clase InputLayout.
 *
 * Gestiona el objeto ID3D11InputLayout, que actúa como un "puente" o "contrato"
 * entre los datos de los vértices en la CPU (Vertex Buffer) y lo que espera
 * recibir el Vertex Shader en la GPU.
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class InputLayout
 * @brief Encapsula un @c ID3D11InputLayout que describe el formato de los vértices.
 *
 * Un Input Layout en Direct3D 11 define cómo se interpretan los datos de memoria de un Vertex Buffer
 * (posición, normales, UVs, etc.) y cómo se asignan a las semánticas de entrada de un Vertex Shader
 * compilado (e.g. "POSITION", "TEXCOORD").
 *
 * Esta clase administra la creación, activación en el pipeline y destrucción del recurso.
 */
class InputLayout {
public:
    /**
     * @brief Constructor por defecto.
     */
    InputLayout() = default;

    /**
     * @brief Destructor por defecto.
     * @note No libera automáticamente el recurso COM; se debe llamar a destroy().
     */
    ~InputLayout() = default;

    /**
     * @brief Inicializa el Input Layout a partir de una descripción y el bytecode de un Vertex Shader.
     *
     * Crea el recurso @c ID3D11InputLayout. DirectX valida que la descripción proporcionada
     * coincida con la firma de entrada (Input Signature) del shader compilado.
     *
     * @param device            Referencia al dispositivo para crear el recurso.
     * @param Layout            Vector con la descripción de los elementos de entrada (D3D11_INPUT_ELEMENT_DESC).
     * @param VertexShaderData  Bytecode compilado del Vertex Shader contra el cual se validará el layout.
     * @return HRESULT @c S_OK si la creación fue exitosa; código de error en caso contrario.
     *
     * @post Si retorna @c S_OK, @c m_inputLayout != nullptr.
     */
    HRESULT init(Device& device,
        std::vector<D3D11_INPUT_ELEMENT_DESC>& Layout,
        ID3DBlob* VertexShaderData);

    /**
     * @brief Actualiza parámetros internos.
     *
     * Método reservado para lógica futura. Actualmente no realiza ninguna operación.
     */
    void update();

    /**
     * @brief Activa el Input Layout en el pipeline gráfico.
     *
     * Llama a @c IASetInputLayout en el contexto del dispositivo, indicando a la GPU
     * cómo leer los vértices para las siguientes llamadas de dibujo.
     *
     * @param deviceContext Contexto donde se establecerá el Input Layout.
     * @pre @c m_inputLayout debe estar inicializado.
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Libera el recurso @c ID3D11InputLayout.
     *
     * Utiliza SAFE_RELEASE para decrementar el contador de referencias y limpiar el puntero.
     * @post @c m_inputLayout == nullptr.
     */
    void destroy();

public:
    /**
     * @brief Puntero al recurso nativo de Direct3D 11.
     */
    ID3D11InputLayout* m_inputLayout = nullptr;
};