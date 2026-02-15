#pragma once
/**
 * @file Device.h
 * @brief Definición de la clase Device para la creación de recursos gráficos.
 *
 * Esta clase encapsula el ID3D11Device de DirectX 11. Su responsabilidad principal
 * es actuar como una "fábrica" para crear recursos (texturas, buffers, shaders, vistas)
 * y gestionar la vida del dispositivo gráfico principal.
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"

 /**
  * @class Device
  * @brief Encapsula un @c ID3D11Device y facilita la creación de recursos gráficos en Direct3D 11.
  *
  * Esta clase actúa como un contenedor del objeto @c ID3D11Device. Expone métodos
  * que encapsulan la creación de vistas, texturas, shaders, estados y buffers
  * para simplificar el manejo del ciclo de vida de los recursos gráficos y el manejo de errores.
  *
  * @note No administra automáticamente el @c ID3D11DeviceContext (eso lo hace la clase DeviceContext).
  */
class Device {
public:
    /**
     * @brief Constructor por defecto.
     */
    Device() = default;

    /**
     * @brief Destructor por defecto.
     * @note No libera automáticamente los recursos. Se debe llamar a destroy().
     */
    ~Device() = default;

    /**
     * @brief Inicializa el dispositivo gráfico.
     * Crea el ID3D11Device y el ID3D11DeviceContext asociados (generalmente junto con la SwapChain).
     */
    void init();

    /**
     * @brief Método de actualización (placeholder).
     */
    void update();

    /**
     * @brief Método de renderizado (placeholder).
     */
    void render();

    /**
     * @brief Libera la interfaz ID3D11Device.
     * Utiliza la macro SAFE_RELEASE para liberar el puntero COM.
     */
    void destroy();

    /**
     * @brief Crea una Render Target View (RTV).
     *
     * Una RTV permite que una recurso de recurso (como una textura) sea usado como destino de renderizado.
     *
     * @param pResource Puntero al recurso que servirá como render target.
     * @param pDesc Descriptor de la vista (puede ser nullptr para una vista por defecto).
     * @param ppRTView Puntero doble donde se almacenará la RTV creada.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreateRenderTargetView(ID3D11Resource* pResource,
        const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
        ID3D11RenderTargetView** ppRTView);

    /**
     * @brief Crea una Textura 2D.
     *
     * @param pDesc Descriptor que define las propiedades de la textura (ancho, alto, formato, etc.).
     * @param pInitialData Datos iniciales para llenar la textura (opcional).
     * @param ppTexture2D Puntero doble donde se almacenará la textura creada.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                            const D3D11_SUBRESOURCE_DATA* pInitialData,
                            ID3D11Texture2D** ppTexture2D);

    /**
     * @brief Crea una Depth Stencil View (DSV).
     *
     * Una DSV permite que una textura sea usada para pruebas de profundidad y estencilado.
     *
     * @param pResource Recurso de textura que se usará como buffer de profundidad.
     * @param pDesc Descriptor de la vista.
     * @param ppDepthStencilView Puntero doble donde se almacenará la DSV creada.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreateDepthStencilView(ID3D11Resource* pResource,
                                   const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                                   ID3D11DepthStencilView** ppDepthStencilView);

    /**
     * @brief Crea un Vertex Shader desde bytecode compilado.
     *
     * @param pShaderBytecode Puntero al bytecode del shader compilado.
     * @param BytecodeLength Longitud del bytecode en bytes.
     * @param pClassLinkage Enlace de clases dinámicas (opcional, puede ser nullptr).
     * @param ppVertexShader Puntero doble donde se almacenará el Vertex Shader creado.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreateVertexShader(const void* pShaderBytecode,
                               unsigned int BytecodeLength,
                               ID3D11ClassLinkage* pClassLinkage,
                               ID3D11VertexShader** ppVertexShader);

    /**
     * @brief Crea un Input Layout para describir la estructura de los vértices.
     *
     * @param pInputElementDescs Array de descriptores de elementos de entrada.
     * @param NumElements Número de elementos en el array.
     * @param pShaderBytecodeWithInputSignature Bytecode del shader contra el cual se validará el layout.
     * @param BytecodeLength Longitud del bytecode.
     * @param ppInputLayout Puntero doble donde se almacenará el Input Layout creado.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                              unsigned int NumElements,
                              const void* pShaderBytecodeWithInputSignature,
                              unsigned int BytecodeLength,
                              ID3D11InputLayout** ppInputLayout);

    /**
     * @brief Crea un Pixel Shader desde bytecode compilado.
     *
     * @param pShaderBytecode Puntero al bytecode del shader compilado.
     * @param BytecodeLength Longitud del bytecode.
     * @param pClassLinkage Enlace de clases dinámicas (opcional).
     * @param ppPixelShader Puntero doble donde se almacenará el Pixel Shader creado.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreatePixelShader(const void* pShaderBytecode,
                              unsigned int BytecodeLength,
                              ID3D11ClassLinkage* pClassLinkage,
                              ID3D11PixelShader** ppPixelShader);

    /**
     * @brief Crea un Buffer (Vertex, Index, Constant) en la GPU.
     *
     * @param pDesc Descriptor del buffer (tamaño, uso, flags de enlace).
     * @param pInitialData Datos iniciales para el buffer (opcional).
     * @param ppBuffer Puntero doble donde se almacenará el Buffer creado.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
                        const D3D11_SUBRESOURCE_DATA* pInitialData,
                        ID3D11Buffer** ppBuffer);

    /**
     * @brief Crea un Sampler State para el muestreo de texturas.
     *
     * @param pSamplerDesc Descriptor del estado del sampler (filtros, modos de dirección).
     * @param ppSamplerState Puntero doble donde se almacenará el Sampler State creado.
     * @return HRESULT S_OK si tiene éxito.
     */
    HRESULT CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
                               ID3D11SamplerState** ppSamplerState);

public:
    /**
     * @brief Puntero nativo al dispositivo D3D11.
     * Es público para permitir acceso directo cuando sea necesario por otras clases del motor.
     */
    ID3D11Device* m_device = nullptr;
};