#pragma once
#include "Prerequisites.h"
#include "MeshComponent.h"

class Device;
class DeviceContext;

/**
 * @class Buffer
 * @brief Encapsula un @c ID3D11Buffer para vértices, índices o constantes, incluyendo creación, actualización y enlace.
 *
 * Esta clase administra la vida de un único buffer de D3D11 y su uso dentro del pipeline de renderizado.
 * Permite crear buffers de distintos tipos (vértices, índices o constantes), actualizarlos dinámicamente y
 * enlazarlos a las etapas apropiadas del pipeline gráfico.
 *
 *
 * @author Hannin Steve Abarca Jacinto
 */
class Buffer {
public:
    /**
     * @brief Constructor por defecto. No crea recursos.
     */
    Buffer() = default;

    /**
     * @brief Destructor por defecto.
     * @details No libera automáticamente el recurso; se debe invocar @c destroy() para liberar el buffer.
     */
    ~Buffer() = default;

    /**
     * @brief Inicializa el buffer como Vertex o Index Buffer usando un @c MeshComponent.
     *
     * Crea internamente un @c ID3D11Buffer con los datos del mesh (vértices o índices) según el @p bindFlag indicado.
     */
    HRESULT init(Device& device, const MeshComponent& mesh, unsigned int bindFlag);

    /**
     * @brief Inicializa el buffer como Constant Buffer.
     *
     * Crea un @c ID3D11Buffer con la bandera @c D3D11_BIND_CONSTANT_BUFFER y tamaño @p ByteWidth (múltiplo de 16 bytes recomendado).
     */
    HRESULT init(Device& device, unsigned int ByteWidth);

    /**
     * @brief Actualiza el contenido del buffer.
     *
     * Este método permite escribir nuevos datos en el buffer, comúnmente usado para actualizar constantes
     * cada cuadro o subir datos modificados de vértices o índices.
     */
    void update(DeviceContext& deviceContext,
                ID3D11Resource* pDstResource,
                unsigned int    DstSubresource,
                const D3D11_BOX* pDstBox,
                const void* pSrcData,
                unsigned int    SrcRowPitch,
                unsigned int    SrcDepthPitch);

    /**
     * @brief Enlaza el buffer a la etapa correspondiente del pipeline de render.
     */
    void render(DeviceContext& deviceContext,
                unsigned int   StartSlot,
                unsigned int   NumBuffers,
                bool           setPixelShader = false,
                DXGI_FORMAT    format = DXGI_FORMAT_UNKNOWN);

    /**
     * @brief Libera el recurso COM asociado al buffer y limpia sus metadatos.
     *
     * Idempotente; puede llamarse múltiples veces sin causar errores.
     *
     * @post @c m_buffer == nullptr, @c m_stride == 0, @c m_offset == 0 y @c m_bindFlag == 0.
     */
    void destroy();

    /**
     * @brief Crea un buffer genérico utilizando un descriptor @c D3D11_BUFFER_DESC.
     *
     * Función auxiliar invocada internamente por @c init() para factorizar la creación de recursos.
     *
     * @param device   Dispositivo utilizado para la creación del buffer.
     * @param desc     Descriptor del buffer (uso, tamaño, flags, etc.).
     * @param initData Datos iniciales opcionales (puede ser @c nullptr).
     * @return @c S_OK si la creación fue exitosa; código @c HRESULT en caso contrario.
     */
    HRESULT createBuffer(Device& device,
                        D3D11_BUFFER_DESC& desc,
                        D3D11_SUBRESOURCE_DATA* initData);

private:
    /// Recurso COM de D3D11 administrado por esta clase.
    ID3D11Buffer* m_buffer = nullptr;

    /// Tamaño de un elemento en bytes (solo para Vertex Buffers).
    unsigned int m_stride = 0;

    /// Desplazamiento inicial en bytes (solo para Vertex Buffers).
    unsigned int m_offset = 0;

    /// Bandera de enlace que indica el tipo de buffer (@c D3D11_BIND_*).
    unsigned int m_bindFlag = 0;
};
