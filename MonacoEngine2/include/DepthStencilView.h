#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Texture;

/**
 * @file DepthStencilView.h
 * @brief Declaración de la clase DepthStencilView, encargada de administrar la vista de profundidad/esténcil en Direct3D 11.
 * @author Hannin
 */

 /**
  * @class DepthStencilView
  * @brief Encapsula un @c ID3D11DepthStencilView para usar un recurso de profundidad/esténcil en el pipeline gráfico.
  *
  * Esta clase administra la creación, aplicación y liberación de un @c DepthStencilView en Direct3D 11,
  * permitiendo vincular un buffer de profundidad/esténcil a la etapa de Output-Merger del pipeline.
  *
  * @note No gestiona directamente la vida útil de @c Texture ni de @c DeviceContext.
  */
class DepthStencilView {
public:
    /**
     * @brief Constructor por defecto (no crea recursos).
     */
    DepthStencilView() = default;

    /**
     * @brief Destructor por defecto.
     * @details No libera automáticamente los recursos; se debe llamar a destroy() explícitamente.
     */
    ~DepthStencilView() = default;

    /**
     * @brief Inicializa el @c ID3D11DepthStencilView a partir de una textura de profundidad.
     *
     * Crea y asocia un DepthStencilView con el recurso proporcionado (usualmente una
     * textura creada con la bandera @c D3D11_BIND_DEPTH_STENCIL).
     *
     * @post Si retorna @c S_OK, @c m_depthStencilView != nullptr.
     * @sa destroy()
     */
    HRESULT init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

    /**
     * @brief Actualiza parámetros internos si se requieren cambios en el estado.
     *
     * Método de marcador para actualizar la configuración del recurso.
     * @note Actualmente no realiza ninguna operación.
     */
    void update() {};

    /**
     * @brief Asigna la vista de profundidad/esténcil al pipeline de render.
     *
     * Llama internamente a @c OMSetRenderTargets para asociar @c m_depthStencilView al @c DeviceContext.
     *
     * @param deviceContext Contexto de dispositivo donde se enlazará el depth-stencil view.
     *
     * @pre @c m_depthStencilView debe haberse creado mediante init().
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Libera el recurso asociado al @c ID3D11DepthStencilView.
     *
     * Es un método idempotente: puede llamarse varias veces de manera segura.
     * @post @c m_depthStencilView == nullptr.
     */
    void destroy();

public:
    /**
     * @brief Vista de profundidad/esténcil de Direct3D 11.
     * @details Válido después de una llamada exitosa a init(); se establece en @c nullptr tras destroy().
     */
    ID3D11DepthStencilView* m_depthStencilView = nullptr;
};
