#pragma once
#include "Prerequisites.h"

// Declaraciones anticipadas
class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

/**
 * @class RenderTargetView
 * @brief Administra un @c ID3D11RenderTargetView usado como destino de renderizado.
 *
 * Permite crear, limpiar, aplicar y liberar vistas de render (RTV) en Direct3D 11,
 * tanto desde el back buffer como desde texturas personalizadas.
 *
 * @author Hannin
 */
class
    RenderTargetView {
public:
    /// Constructor por defecto.
    RenderTargetView() = default;

    /// Destructor por defecto (no libera el recurso; usar destroy()).
    ~RenderTargetView() = default;

    /// Crea el RTV desde el back buffer del swap chain.
    HRESULT
        init(Device& device,
            Texture& backBuffer, 
            DXGI_FORMAT Format);

    /// Crea el RTV desde una textura genérica.
    HRESULT
        init(Device& device,
            Texture& inTex,
            D3D11_RTV_DIMENSION ViewDimension,
            DXGI_FORMAT Format);

    /// Actualiza parámetros internos (placeholder).
    void
        update();

    /// Limpia y aplica el RTV junto con un DepthStencilView.
    void
        render(DeviceContext& deviceContext,
                DepthStencilView& depthStencilView,
                unsigned int numViews,
                const float ClearColor[4]);

    /// Aplica el RTV sin limpiar ni usar DepthStencil.
    void
        render(DeviceContext& deviceContext,
                unsigned int numViews);

    /// Libera el recurso asociado.
    void
        destroy();

private:
    /// Recurso COM de Direct3D 11 para la vista de Render Target.
    ID3D11RenderTargetView* m_renderTargetView = nullptr;
};
