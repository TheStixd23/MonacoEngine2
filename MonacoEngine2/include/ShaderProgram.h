#pragma once
/**
 * @file ShaderProgram.h
 * @brief Definición de la clase ShaderProgram.
 *
 * Gestiona la carga, compilación y activación de shaders (Vertex y Pixel) en DirectX 11.
 * También se encarga de la creación del Input Layout asociado al Vertex Shader.
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"
#include "InputLayout.h"

class Device;
class DeviceContext;

/**
 * @class ShaderProgram
 * @brief Encapsula la creación, compilación y uso de Vertex Shader y Pixel Shader.
 *
 * Esta clase administra el ciclo de vida de un par de shaders (VS y PS) definidos en un
 * archivo HLSL. Se encarga de compilarlos, crear los recursos de GPU y el Input Layout
 * necesario para pasar datos de vértices.
 */
class ShaderProgram {
public:
    /**
     * @brief Constructor por defecto.
     */
    ShaderProgram() = default;

    /**
     * @brief Destructor por defecto.
     * @note No libera recursos automáticamente; llamar a destroy() explícitamente.
     */
    ~ShaderProgram() = default;

    /**
     * @brief Inicializa el programa de shaders completo desde un archivo HLSL.
     *
     * Compila el Vertex Shader (entry point "VS") y el Pixel Shader (entry point "PS")
     * desde el archivo dado, y crea el Input Layout asociado.
     *
     * @param device    Referencia al dispositivo para crear los recursos.
     * @param fileName  Nombre del archivo HLSL que contiene el código de los shaders.
     * @param Layout    Descripción del formato de los vértices (Input Layout).
     * @return HRESULT @c S_OK si todo fue exitoso; código de error en caso contrario.
     */
    HRESULT init(Device& device,
                const std::string& fileName,
                std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

    /**
     * @brief Método de actualización placeholder.
     */
    void update();

    /**
     * @brief Activa el programa de shaders en el pipeline gráfico.
     *
     * Establece el Vertex Shader, el Pixel Shader y el Input Layout en el contexto
     * para que sean usados en las siguientes llamadas de dibujo.
     *
     * @param deviceContext Contexto del dispositivo.
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Activa solo un tipo específico de shader.
     *
     * @param deviceContext Contexto del dispositivo.
     * @param type          Tipo de shader a activar (VertexShader o PixelShader).
     */
    void render(DeviceContext& deviceContext, ShaderType type);

    /**
     * @brief Libera todos los recursos de shaders y blobs de memoria.
     */
    void destroy();

    /**
     * @brief Crea el Input Layout basado en el Vertex Shader compilado actualmente.
     *
     * @param device Dispositivo gráfico.
     * @param Layout Vector de descripción de elementos.
     * @return HRESULT @c S_OK si tiene éxito.
     */
    HRESULT CreateInputLayout(Device& device,
                              std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

    /**
     * @brief Crea un shader específico desde el archivo almacenado en m_shaderFileName.
     *
     * @param device Dispositivo gráfico.
     * @param type   Tipo de shader (Vertex o Pixel).
     * @return HRESULT @c S_OK si tiene éxito.
     */
    HRESULT CreateShader(Device& device, ShaderType type);

    /**
     * @brief Crea un shader específico desde un archivo HLSL dado.
     *
     * @param device    Dispositivo gráfico.
     * @param type      Tipo de shader.
     * @param fileName  Nombre del archivo HLSL.
     * @return HRESULT @c S_OK si tiene éxito.
     */
    HRESULT CreateShader(Device& device, ShaderType type, const std::string& fileName);

    /**
     * @brief Compila un shader desde un archivo en disco.
     *
     * Utiliza D3DCompileFromFile para generar el bytecode.
     *
     * @param szFileName    Ruta del archivo.
     * @param szEntryPoint  Nombre de la función principal (ej. "VS", "PS").
     * @param szShaderModel Modelo de shader (ej. "vs_4_0", "ps_4_0").
     * @param ppBlobOut     Puntero doble donde se guardará el bytecode compilado.
     * @return HRESULT @c S_OK si la compilación es exitosa.
     */
    HRESULT CompileShaderFromFile(char* szFileName,
                                  LPCSTR szEntryPoint,
                                  LPCSTR szShaderModel,
                                  ID3DBlob** ppBlobOut);

public:
    /** @brief Puntero al Vertex Shader de D3D11. */
    ID3D11VertexShader* m_VertexShader = nullptr;

    /** @brief Puntero al Pixel Shader de D3D11. */
    ID3D11PixelShader* m_PixelShader = nullptr;

    /** @brief Objeto que gestiona el Input Layout. */
    InputLayout m_inputLayout;

private:
    /** @brief Nombre del archivo fuente de los shaders. */
    std::string m_shaderFileName;

    /** @brief Blob con el bytecode del Vertex Shader (necesario para el Input Layout). */
    ID3DBlob* m_vertexShaderData = nullptr;

    /** @brief Blob con el bytecode del Pixel Shader. */
    ID3DBlob* m_pixelShaderData = nullptr;
};