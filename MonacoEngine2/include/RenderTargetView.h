#pragma once
/**
 * @file RenderTargetView.h
 * @brief Definición de la clase RenderTargetView.
 *
 * Encapsula un ID3D11RenderTargetView, que permite utilizar un recurso de textura
 * como destino de salida para el pipeline de renderizado (Output Merger Stage).
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"

 // Forward Declarations
class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

/**
 * @class RenderTargetView
 * @brief Encapsula una vista de renderizado (RTV) de Direct3D 11.
 *
 * Esta clase administra la creación y el enlace de un Render Target View.
 * Es necesaria para renderizar gráficos en una textura (como el BackBuffer de la SwapChain)
 * o para técnicas de renderizado off-screen (como mapas de sombras o post-procesado).
 */
class RenderTargetView {
public:
    /**
     * @brief Constructor por defecto.
     */
    RenderTargetView() = default;

    /**
     * @brief Destructor por defecto.
     * @details No libera automáticamente el recurso COM; se debe llamar a destroy().
     */
    ~RenderTargetView() = default;

    /**
     * @brief Inicializa el Render Target View para el Back Buffer.
     *
     * Crea un RTV asociado a la textura del Back Buffer de la Swap Chain.
     *
     * @param device      Referencia al dispositivo gráfico para crear el recurso.
     * @param backBuffer  Referencia a la textura que representa el Back Buffer.
     * @param Format      Formato de los datos de la vista (ej. @c DXGI_FORMAT_R8G8B8A8_UNORM).
     * @return HRESULT @c S_OK si la creación fue exitosa, de lo contrario un código de error.
     *
     * @post Si tiene éxito, @c m_renderTargetView será válido.
     */
    HRESULT init(Device& device, Texture& backBuffer, DXGI_FORMAT Format);

    /**
     * @brief Inicializa el Render Target View para una textura genérica.
     *
     * Permite crear un RTV para cualquier textura compatible (bind flag D3D11_BIND_RENDER_TARGET).
     * Útil para Render-To-Texture.
     *
     * @param device        Referencia al dispositivo gráfico.
     * @param inTex         Referencia a la textura destino.
     * @param ViewDimension Tipo de dimensión de la vista (ej. @c D3D11_RTV_DIMENSION_TEXTURE2D).
     * @param Format        Formato de los datos de la vista.
     * @return HRESULT @c S_OK si la creación fue exitosa.
     */
    HRESULT init(Device& device,
        Texture& inTex,
        D3D11_RTV_DIMENSION ViewDimension,
        DXGI_FORMAT Format);

    /**
     * @brief Actualiza parámetros internos.
     * @note Método reservado para uso futuro.
     */
    void update();

    /**
     * @brief Limpia el Render Target y lo activa en el pipeline.
     *
     * Este método realiza dos acciones:
     * 1. Limpia el buffer de color con el color especificado.
     * 2. Enlaza este RTV y el DSV proporcionado al Output Merger Stage.
     *
     * @param deviceContext     Contexto del dispositivo para ejecutar los comandos.
     * @param depthStencilView  Vista de profundidad/esténcil a enlazar junto con el RTV.
     * @param numViews          Número de vistas a enlazar (generalmente 1).
     * @param ClearColor        Array de 4 floats (RGBA) con el color de fondo para limpiar.
     */
    void render(DeviceContext& deviceContext,
        DepthStencilView& depthStencilView,
        unsigned int numViews,
        const float ClearColor[4]);

    /**
     * @brief Activa el Render Target en el pipeline sin limpiar ni usar Depth Stencil.
     *
     * Útil cuando solo se quiere cambiar el destino de renderizado sin borrar el contenido anterior
     * o sin necesidad de buffer de profundidad.
     *
     * @param deviceContext Contexto del dispositivo.
     * @param numViews      Número de vistas a enlazar.
     */
    void render(DeviceContext& deviceContext, unsigned int numViews);

    /**
     * @brief Libera el recurso COM @c ID3D11RenderTargetView.
     * Utiliza SAFE_RELEASE para limpiar la memoria.
     */
    void destroy();

private:
    /**
     * @brief Puntero nativo a la interfaz de Render Target View de D3D11.
     */
    ID3D11RenderTargetView* m_renderTargetView = nullptr;
};