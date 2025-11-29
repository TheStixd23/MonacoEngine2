/**
 * @file ShaderProgram.h
 * @brief Clase para la gestión de Vertex y Pixel Shaders en Direct3D 11.
 *
 * Encapsula la compilación, creación y enlace de shaders al pipeline gráfico.
 * Administra también el Input Layout asociado al Vertex Shader.
 * @author Hannin Abarca
 */
#pragma once
#include "Prerequisites.h"
#include "InputLayout.h"

class Device;
class DeviceContext;

/**
 * @class ShaderProgram
 * @brief Gestiona un conjunto de Vertex y Pixel Shaders junto con su Input Layout.
 *
 * Proporciona métodos para compilar shaders desde archivos HLSL, crear los
 * recursos correspondientes en GPU y aplicarlos durante el renderizado.
 */
class ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram() = default;

    /// Inicializa los shaders desde un archivo HLSL y crea el Input Layout.
    HRESULT init(Device& device, const std::string& fileName, std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

    /// Método reservado para futuras actualizaciones dinámicas.
    void update();

    /// Aplica los shaders e Input Layout al pipeline.
    void render(DeviceContext& deviceContext);

    /// Aplica únicamente el shader indicado (VS o PS).
    void render(DeviceContext& deviceContext, ShaderType type);

    /// Libera todos los recursos asociados.
    void destroy();

    /// Crea el Input Layout del Vertex Shader.
    HRESULT CreateInputLayout(Device& device, std::vector<D3D11_INPUT_ELEMENT_DESC> Layout);

    /// Crea un shader (VS o PS) usando el archivo definido en m_shaderFileName.
    HRESULT CreateShader(Device& device, ShaderType type);

    /// Crea un shader (VS o PS) desde un archivo HLSL específico.
    HRESULT CreateShader(Device& device, ShaderType type, const std::string& fileName);

    /// Compila un shader desde archivo usando D3DCompileFromFile.
    HRESULT CompileShaderFromFile(char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

public:
    ID3D11VertexShader* m_VertexShader = nullptr;  ///< Vertex Shader creado en GPU.
    ID3D11PixelShader* m_PixelShader = nullptr;  ///< Pixel Shader creado en GPU.
    InputLayout         m_inputLayout;             ///< Layout de entrada para el VS.

private:
    std::string m_shaderFileName;  ///< Archivo HLSL fuente del programa.
    ID3DBlob* m_vertexShaderData = nullptr;  ///< Bytecode compilado del VS.
    ID3DBlob* m_pixelShaderData = nullptr;  ///< Bytecode compilado del PS.
};
