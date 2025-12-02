#pragma once
/**
 * @file DeviceContext.h
 * @brief Definición de la clase DeviceContext.
 *
 * Encapsula el ID3D11DeviceContext (Contexto Inmediato) de DirectX 11.
 * Es responsable de enviar comandos de renderizado a la GPU, como configurar estados,
 * enlazar recursos (shaders, buffers) y emitir las llamadas de dibujo (Draw calls).
 *
 * @author MonacoEngine Team
 */

#include "Prerequisites.h"

 /**
  * @class DeviceContext
  * @brief Envoltorio para el contexto inmediato de DirectX 11 (@c ID3D11DeviceContext).
  *
  * Esta clase permite configurar el pipeline gráfico antes de dibujar.
  * Controla etapas como:
  * - Input Assembler (IA): Layouts, topología, buffers de vértices/índices.
  * - Vertex/Pixel Shader (VS/PS): Shaders, recursos (texturas), samplers, constantes.
  * - Rasterizer (RS): Viewports, estados de rasterización.
  * - Output Merger (OM): Render targets, depth stencil, blending.
  */
class DeviceContext {
public:
    /** @brief Constructor por defecto. */
    DeviceContext() = default;

    /** @brief Destructor por defecto. */
    ~DeviceContext() = default;

    /**
     * @brief Inicializa el contexto.
     * @note En este diseño, el puntero suele asignarse externamente tras crear el Device.
     */
    void init();

    /** @brief Método placeholder para actualizaciones lógicas del contexto. */
    void update();

    /** @brief Método placeholder para operaciones de renderizado internas. */
    void render();

    /**
     * @brief Libera el recurso COM @c ID3D11DeviceContext.
     * Llama a Release() y pone el puntero a nullptr.
     */
    void destroy();

    /**
     * @brief Configura los viewports en la etapa de Rasterización (RS).
     * Define el área de la ventana donde se dibujará.
     *
     * @param NumViewports Número de viewports a configurar.
     * @param pViewports Puntero al array de estructuras D3D11_VIEWPORT.
     */
    void RSSetViewports(unsigned int NumViewports, const D3D11_VIEWPORT* pViewports);

    /**
     * @brief Asigna recursos (texturas/SRVs) a la etapa de Pixel Shader (PS).
     *
     * @param StartSlot Slot inicial de registro (típicamente 0 para la textura principal).
     * @param NumViews Número de vistas a asignar.
     * @param ppShaderResourceViews Array de punteros a las vistas de recurso (SRV).
     */
    void PSSetShaderResources(unsigned int StartSlot,
        unsigned int NumViews,
        ID3D11ShaderResourceView* const* ppShaderResourceViews);

    /**
     * @brief Establece el Input Layout activo en la etapa Input Assembler (IA).
     * Define cómo la GPU debe leer los datos de los vértices.
     *
     * @param pInputLayout Puntero al objeto Input Layout.
     */
    void IASetInputLayout(ID3D11InputLayout* pInputLayout);

    /**
     * @brief Asigna un Vertex Shader (VS) al pipeline.
     *
     * @param pVertexShader Puntero al Vertex Shader.
     * @param ppClassInstances Interfaces de enlace de clases dinámicas (opcional).
     * @param NumClassInstances Número de instancias de clase.
     */
    void VSSetShader(ID3D11VertexShader* pVertexShader,
        ID3D11ClassInstance* const* ppClassInstances,
        unsigned int NumClassInstances);

    /**
     * @brief Asigna un Pixel Shader (PS) al pipeline.
     *
     * @param pPixelShader Puntero al Pixel Shader.
     * @param ppClassInstances Interfaces de enlace de clases dinámicas (opcional).
     * @param NumClassInstances Número de instancias de clase.
     */
    void PSSetShader(ID3D11PixelShader* pPixelShader,
        ID3D11ClassInstance* const* ppClassInstances,
        unsigned int NumClassInstances);

    /**
     * @brief Actualiza datos en un recurso de GPU (Buffer o Textura) desde la CPU.
     *
     * @param pDstResource Recurso de destino en la GPU.
     * @param DstSubresource Índice del subrecurso (0 para buffers simples).
     * @param pDstBox Caja que define la región a actualizar (nullptr para todo el recurso).
     * @param pSrcData Puntero a los datos en memoria del sistema.
     * @param SrcRowPitch Tamaño de fila en bytes (para texturas).
     * @param SrcDepthPitch Tamaño de profundidad en bytes (para texturas 3D).
     */
    void UpdateSubresource(ID3D11Resource* pDstResource,
        unsigned int DstSubresource,
        const D3D11_BOX* pDstBox,
        const void* pSrcData,
        unsigned int SrcRowPitch,
        unsigned int SrcDepthPitch);

    /**
     * @brief Enlaza buffers de vértices a la etapa Input Assembler (IA).
     *
     * @param StartSlot Slot de entrada inicial (0 a 15).
     * @param NumBuffers Número de buffers a enlazar.
     * @param ppVertexBuffers Array de punteros a los Vertex Buffers.
     * @param pStrides Array con el tamaño (stride) de la estructura de vértice de cada buffer.
     * @param pOffsets Array con el desplazamiento (offset) inicial en cada buffer.
     */
    void IASetVertexBuffers(unsigned int StartSlot,
        unsigned int NumBuffers,
        ID3D11Buffer* const* ppVertexBuffers,
        const unsigned int* pStrides,
        const unsigned int* pOffsets);

    /**
     * @brief Enlaza un buffer de índices a la etapa Input Assembler (IA).
     *
     * @param pIndexBuffer Puntero al Index Buffer.
     * @param Format Formato de los índices (ej. DXGI_FORMAT_R16_UINT o R32_UINT).
     * @param Offset Desplazamiento inicial en bytes.
     */
    void IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
        DXGI_FORMAT Format,
        unsigned int Offset);

    /**
     * @brief Asigna estados de muestreo (Samplers) a la etapa Pixel Shader.
     * Controla cómo se filtran las texturas.
     *
     * @param StartSlot Slot inicial.
     * @param NumSamplers Número de samplers.
     * @param ppSamplers Array de punteros a los estados de muestreo.
     */
    void PSSetSamplers(unsigned int StartSlot,
        unsigned int NumSamplers,
        ID3D11SamplerState* const* ppSamplers);

    /**
     * @brief Configura el estado del Rasterizador (RS).
     * Controla culling, wireframe vs sólido, etc.
     *
     * @param pRasterizerState Puntero al objeto de estado.
     */
    void RSSetState(ID3D11RasterizerState* pRasterizerState);

    /**
     * @brief Configura el estado de mezcla (Blending) en el Output Merger (OM).
     * Controla transparencias y mezcla de colores.
     *
     * @param pBlendState Puntero al objeto de estado de mezcla.
     * @param BlendFactor Array de 4 floats para el factor de mezcla (si se usa).
     * @param SampleMask Máscara de cobertura de muestras.
     */
    void OMSetBlendState(ID3D11BlendState* pBlendState,
        const float BlendFactor[4],
        unsigned int SampleMask);

    /**
     * @brief Define los destinos de renderizado en el Output Merger (OM).
     *
     * @param NumViews Número de Render Targets.
     * @param ppRenderTargetViews Array de vistas de render target (donde se dibuja el color).
     * @param pDepthStencilView Vista de profundidad/esténcil (donde se guarda la profundidad).
     */
    void OMSetRenderTargets(unsigned int NumViews,
        ID3D11RenderTargetView* const* ppRenderTargetViews,
        ID3D11DepthStencilView* pDepthStencilView);

    /**
     * @brief Define la topología de las primitivas (Triángulos, Líneas, Puntos).
     *
     * @param Topology Tipo de topología (ej. D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST).
     */
    void IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

    /**
     * @brief Limpia un Render Target con un color sólido.
     * Típicamente se llama al inicio de cada frame.
     *
     * @param pRenderTargetView Vista a limpiar.
     * @param ColorRGBA Color de fondo (RGBA).
     */
    void ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
        const float ColorRGBA[4]);

    /**
     * @brief Limpia el buffer de profundidad y/o esténcil.
     *
     * @param pDepthStencilView Vista a limpiar.
     * @param ClearFlags Banderas (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL).
     * @param Depth Valor de profundidad a restablecer (usualmente 1.0f).
     * @param Stencil Valor de esténcil a restablecer.
     */
    void ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
        unsigned int ClearFlags,
        float Depth,
        UINT8 Stencil);

    /**
     * @brief Asigna Constant Buffers al Vertex Shader (VS).
     *
     * @param StartSlot Slot inicial.
     * @param NumBuffers Número de buffers.
     * @param ppConstantBuffers Array de punteros a los buffers constantes.
     */
    void VSSetConstantBuffers(unsigned int StartSlot,
        unsigned int NumBuffers,
        ID3D11Buffer* const* ppConstantBuffers);

    /**
     * @brief Asigna Constant Buffers al Pixel Shader (PS).
     *
     * @param StartSlot Slot inicial.
     * @param NumBuffers Número de buffers.
     * @param ppConstantBuffers Array de punteros a los buffers constantes.
     */
    void PSSetConstantBuffers(unsigned int StartSlot,
        unsigned int NumBuffers,
        ID3D11Buffer* const* ppConstantBuffers);

    /**
     * @brief Ejecuta el comando de dibujo indexado.
     *
     * @param IndexCount Número de índices a dibujar.
     * @param StartIndexLocation Ubicación del primer índice en el buffer.
     * @param BaseVertexLocation Valor sumado a cada índice antes de leer el vértice.
     */
    void DrawIndexed(unsigned int IndexCount,
        unsigned int StartIndexLocation,
        int BaseVertexLocation);

public:
    /**
     * @brief Puntero nativo al contexto inmediato de D3D11.
     */
    ID3D11DeviceContext* m_deviceContext = nullptr;
};