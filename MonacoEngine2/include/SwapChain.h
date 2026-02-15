#pragma once
/**
 * @file SwapChain.h
 * @brief Definición de la clase SwapChain.
 *
 * Encapsula la cadena de intercambio (Swap Chain) de DXGI. Es responsable de presentar
 * los cuadros renderizados en la ventana de la aplicación, gestionando el doble buffer
 * y la sincronización vertical.
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"

class Device;
class DeviceContext;
class Window;
class Texture;

/**
 * @class SwapChain
 * @brief Encapsula un @c IDXGISwapChain en Direct3D 11 para administrar buffers de presentación.
 *
 * Un Swap Chain es una colección de buffers que se utilizan para mostrar fotogramas al usuario.
 * Típicamente consta de un buffer frontal (lo que se ve en pantalla) y un buffer trasero
 * (donde la GPU dibuja el siguiente cuadro).
 *
 * Esta clase maneja:
 * - Creación de la cadena de intercambio asociada a una ventana.
 * - Obtención del Back Buffer como una textura utilizable.
 * - Presentación de la imagen final (flip/blt).
 * - Soporte para configuración de MSAA (Multisample Anti-Aliasing).
 */
class SwapChain {
public:
    /**
     * @brief Constructor por defecto.
     */
    SwapChain() = default;

    /**
     * @brief Destructor por defecto.
     * @details No libera automáticamente los recursos COM; llamar a destroy().
     */
    ~SwapChain() = default;

    /**
     * @brief Inicializa el Swap Chain y obtiene el back buffer.
     *
     * Crea el objeto @c IDXGISwapChain asociado a una ventana específica.
     * También consulta la textura del buffer trasero y la almacena en el objeto @p backBuffer proporcionado
     * para que pueda ser usado como Render Target.
     *
     * @param device        Referencia al dispositivo gráfico.
     * @param deviceContext Contexto de dispositivo asociado (para operaciones inmediatas si fuera necesario).
     * @param backBuffer    Referencia a un objeto Texture donde se guardará el puntero al buffer trasero.
     * @param window        Ventana de la aplicación donde se presentará la imagen.
     * @return HRESULT @c S_OK si fue exitoso; código de error en caso contrario.
     *
     * @post Si retorna @c S_OK, @c m_swapChain != nullptr.
     */
    HRESULT init(Device& device,
                 DeviceContext& deviceContext,
                 Texture& backBuffer,
                 Window window);

    /**
     * @brief Actualiza parámetros internos del Swap Chain.
     *
     * Método reservado para lógica futura, como manejar el redimensionado de la ventana (ResizeBuffers),
     * cambio de modo pantalla completa/ventana, o ajustes de MSAA en tiempo de ejecución.
     *
     * @note Actualmente no realiza ninguna operación.
     */
    void update();

    /**
     * @brief Ejecuta operaciones de renderizado internas.
     *
     * Método placeholder.
     * @note Actualmente no realiza ninguna operación.
     */
    void render();

    /**
     * @brief Libera todos los recursos asociados al Swap Chain.
     *
     * Libera la interfaz @c IDXGISwapChain y también las interfaces auxiliares de DXGI
     * (Device, Adapter, Factory) que se hayan obtenido durante la inicialización.
     *
     * @post @c m_swapChain == nullptr.
     */
    void destroy();

    /**
     * @brief Presenta el back buffer en pantalla.
     *
     * Llama a @c IDXGISwapChain::Present. Esto intercambia el buffer trasero con el frontal,
     * haciendo visible lo que se acaba de renderizar.
     *
     * @note Si se utiliza V-Sync, este método esperará al refresco vertical del monitor.
     */
    void present();

public:
    /**
     * @brief Puntero a la interfaz nativa de la cadena de intercambio.
     */
    IDXGISwapChain* m_swapChain = nullptr;

    /**
     * @brief Tipo de driver utilizado (hardware, referencia, software, etc.).
     */
    D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL;

private:
    /**
     * @brief Nivel de características de Direct3D soportado por el dispositivo.
     */
    D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

    /**
     * @brief Número de muestras para MSAA (Multisample Anti-Aliasing).
     * Ejemplo: 1 = sin AA, 4 = 4x MSAA.
     */
    unsigned int m_sampleCount = 1;

    /**
     * @brief Niveles de calidad soportados para la configuración de MSAA actual.
     */
    unsigned int m_qualityLevels = 0;

    // --- Interfaces DXGI internas ---

    /** @brief Interfaz DXGI para el dispositivo gráfico. */
    IDXGIDevice* m_dxgiDevice = nullptr;

    /** @brief Interfaz DXGI para el adaptador de pantalla (GPU). */
    IDXGIAdapter* m_dxgiAdapter = nullptr;

    /** @brief Interfaz DXGI para la fábrica (creador de swap chains). */
    IDXGIFactory* m_dxgiFactory = nullptr;
};