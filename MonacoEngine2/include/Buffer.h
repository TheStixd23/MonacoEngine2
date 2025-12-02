#pragma once
/**
 * @file Buffer.h
 * @brief Definición de la clase Buffer para la gestión de recursos en DirectX 11.
 *
 * Encapsula la creación, actualización y vinculación de buffers (Vertex, Index, Constant)
 * facilitando la interacción con la API de D3D11.
 *
 * @author MonacoEngine Team
 */

#include "Prerequisites.h"
#include "MeshComponent.h"

class Device;
class DeviceContext;

/**
 * @class Buffer
 * @brief Encapsula un @c ID3D11Buffer para vértices, índices o constantes.
 *
 * Esta clase administra el ciclo de vida de un recurso de buffer de Direct3D 11.
 * Permite:
 * - Crear Vertex y Index Buffers a partir de datos de malla.
 * - Crear Constant Buffers para enviar datos a los shaders.
 * - Actualizar datos en la GPU.
 * - Enlazar el buffer al pipeline gráfico para el renderizado.
 *
 * @note La clase deduce el tipo de buffer (Vertex, Index, Constant) basándose en el flag de enlace (@c m_bindFlag).
 * @warning Esta clase no realiza copias profundas del recurso COM; gestionar con cuidado las copias.
 */
class Buffer {
public:
    /**
     * @brief Constructor por defecto.
     */
    Buffer() = default;

    /**
     * @brief Destructor.
     * @note No llama automáticamente a @c destroy(). Se debe llamar explícitamente si se requiere liberar antes de la destrucción del objeto.
     */
    ~Buffer() = default;

    /**
     * @brief Inicializa el buffer como Vertex Buffer o Index Buffer desde un Mesh.
     *
     * @param device Referencia al dispositivo gráfico (@c Device) para crear el recurso.
     * @param mesh Componente de malla (@c MeshComponent) con los datos de vértices e índices.
     * @param bindFlag Tipo de buffer a crear (@c D3D11_BIND_VERTEX_BUFFER o @c D3D11_BIND_INDEX_BUFFER).
     * @return HRESULT @c S_OK si la creación fue exitosa, de lo contrario un código de error.
     */
    HRESULT init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

    /**
     * @brief Inicializa el buffer como Constant Buffer.
     *
     * @param device Referencia al dispositivo gráfico.
     * @param ByteWidth Tamaño del buffer en bytes.
     * @note Debe ser múltiplo de 16 bytes para cumplir con los requisitos de alineación de DirectX 11.
     * @return HRESULT @c S_OK si la creación fue exitosa.
     */
    HRESULT init(Device& device, unsigned int ByteWidth);

    /**
     * @brief Actualiza los datos del buffer en la GPU.
     *
     * Utiliza @c UpdateSubresource para transferir datos desde la memoria del sistema a la memoria de video.
     * Ideal para actualizar Constant Buffers frame a frame.
     *
     * @param deviceContext Contexto del dispositivo para ejecutar la actualización.
     * @param pDstResource Recurso de destino (usualmente el buffer interno, pero puede ser otro).
     * @param DstSubresource Índice del subrecurso (generalmente 0).
     * @param pDstBox Región de destino (nullptr para actualizar todo el buffer).
     * @param pSrcData Puntero a los datos fuente en memoria del sistema.
     * @param SrcRowPitch Paso de fila (ignorado para buffers).
     * @param SrcDepthPitch Paso de profundidad (ignorado para buffers).
     */
    void update(DeviceContext& deviceContext,
        ID3D11Resource* pDstResource,
        unsigned int DstSubresource,
        const D3D11_BOX* pDstBox,
        const void* pSrcData,
        unsigned int SrcRowPitch,
        unsigned int SrcDepthPitch);

    /**
     * @brief Enlaza el buffer al pipeline gráfico para su uso.
     *
     * Dependiendo del tipo de buffer (@c m_bindFlag), llama a la función adecuada de DX11:
     * - Vertex Buffer: @c IASetVertexBuffers
     * - Index Buffer: @c IASetIndexBuffer
     * - Constant Buffer: @c VSSetConstantBuffers (y opcionalmente @c PSSetConstantBuffers).
     *
     * @param deviceContext Contexto del dispositivo.
     * @param StartSlot Slot de inicio (registro) donde se enlazará el buffer.
     * @param NumBuffers Número de buffers a enlazar (generalmente 1).
     * @param setPixelShader (Solo Constant Buffer) Si es @c true, enlaza también al Pixel Shader.
     * @param format (Solo Index Buffer) Formato de los índices (ej. @c DXGI_FORMAT_R32_UINT).
     */
    void render(DeviceContext& deviceContext,
        unsigned int StartSlot,
        unsigned int NumBuffers,
        bool setPixelShader = false,
        DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);

    /**
     * @brief Libera el recurso COM asociado (@c ID3D11Buffer).
     */
    void destroy();

private:
    /**
     * @brief Método interno para la creación del buffer de DirectX.
     *
     * @param device Dispositivo gráfico.
     * @param desc Descriptor del buffer.
     * @param initData Datos iniciales (opcional).
     * @return HRESULT Resultado de la creación.
     */
    HRESULT createBuffer(Device& device,
        D3D11_BUFFER_DESC& desc,
        D3D11_SUBRESOURCE_DATA* initData);

private:
    ID3D11Buffer* m_buffer = nullptr;   ///< Puntero al recurso nativo de D3D11.
    unsigned int  m_stride = 0;         ///< Tamaño de la estructura de datos (stride) en bytes.
    unsigned int  m_offset = 0;         ///< Desplazamiento (offset) en el buffer.
    unsigned int  m_bindFlag = 0;       ///< Bandera que indica el tipo de buffer (Vertex, Index, Constant).
};