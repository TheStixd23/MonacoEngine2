/**
 * @file SwapChain.h
 * @brief Clase para la gestión del IDXGISwapChain en Direct3D 11.
 *
 * Administra los buffers de presentación (front y back buffer) y su presentación
 * en pantalla. Soporta configuración de MSAA para suavizado de bordes.
 */
#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;
class Window;
class Texture;

/**
 * @class SwapChain
 * @brief Encapsula la creación, uso y liberación de un IDXGISwapChain.
 *
 * Gestiona el ciclo de vida del swap chain, su back buffer y las interfaces DXGI
 * asociadas (device, adapter y factory). Permite presentar el frame final en ventana.
 * 
 * @author Hannin Abarca
 */
class SwapChain {
public:
    SwapChain() = default;
    ~SwapChain() = default;

    /// Inicializa el Swap Chain y obtiene el back buffer.
    HRESULT init(Device& device, 
                DeviceContext& deviceContext, 
                Texture& backBuffer, 
                Window window);

    /// Método reservado para futuras actualizaciones (resize, MSAA, etc.).
    void update();

    /// Método opcional para sincronización o depuración de buffers.
    void render();

    /// Libera todos los recursos DXGI asociados.
    void destroy();

    /// Presenta el back buffer en pantalla (con o sin V-Sync).
    void present();

public:
    IDXGISwapChain* m_swapChain = nullptr;   ///< Objeto principal del Swap Chain.
    D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL; ///< Tipo de driver usado.

private:
    D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0; ///< Nivel de características D3D.
    unsigned int m_sampleCount = 1;        ///< Número de muestras MSAA.
    unsigned int m_qualityLevels = 0;      ///< Niveles de calidad de MSAA.

    IDXGIDevice* m_dxgiDevice = nullptr; ///< Interfaz DXGI del dispositivo.
    IDXGIAdapter* m_dxgiAdapter = nullptr; ///< Adaptador (GPU) usado.
    IDXGIFactory* m_dxgiFactory = nullptr; ///< Fábrica DXGI para creación del swap chain.
};
