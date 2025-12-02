#pragma once
/**
 * @file DepthStencilView.h
 * @brief Definición de la clase DepthStencilView para la gestión de vistas de profundidad.
 *
 * Esta clase encapsula la creación y el manejo de la vista de Depth-Stencil (DSV),
 * fundamental para que el Output Merger Stage realice pruebas de profundidad y estencilado.
 *
 * @author MonacoEngine Team
 */

#include "Prerequisites.h"

class Device;
class DeviceContext;
class Texture;

/**
 * @class DepthStencilView
 * @brief Encapsula un @c ID3D11DepthStencilView para usar un recurso de profundidad/esténcil en el pipeline.
 *
 * Esta clase administra la creación, aplicación y liberación de un @c DepthStencilView en Direct3D 11,
 * necesario para vincular un buffer de profundidad/esténcil a la etapa de Output-Merger.
 *
 * @note No administra directamente la vida de la @c Texture asociada ni del @c DeviceContext.
 */
class DepthStencilView {
public:
    /**
     * @brief Constructor por defecto (no crea recursos).
     */
    DepthStencilView() = default;

    /**
     * @brief Destructor por defecto.
     * @details No libera automáticamente; llamar a destroy() explícitamente.
     */
    ~DepthStencilView() = default;

    /**
     * @brief Inicializa el @c ID3D11DepthStencilView a partir de una textura de profundidad.
     *
     * Crea y asocia un @c DepthStencilView con el recurso proporcionado (generalmente una
     * textura creada con el flag @c D3D11_BIND_DEPTH_STENCIL).
     *
     * @param device        Referencia al dispositivo gráfico para crear el recurso.
     * @param depthStencil  Textura que servirá como buffer de profundidad/esténcil.
     * @param format        Formato DXGI de la vista (ej. @c DXGI_FORMAT_D24_UNORM_S8_UINT).
     * @return HRESULT @c S_OK si la creación fue exitosa; código de error en caso contrario.
     *
     * @post Si retorna @c S_OK, @c m_depthStencilView != nullptr.
     */
    HRESULT init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

    /**
     * @brief Actualiza parámetros internos.
     * @note Actualmente es un método placeholder sin implementación.
     */
    void update() {};

    /**
     * @brief Asigna la vista de profundidad/esténcil al pipeline de render.
     *
     * Configura esta vista como el Render Target de profundidad activo en el Output Merger.
     *
     * @param deviceContext Contexto de dispositivo donde se enlazará la vista.
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Libera el recurso nativo de Direct3D.
     *
     * Llama a Release() en la interfaz COM y resetea el puntero.
     * @post @c m_depthStencilView == nullptr.
     */
    void destroy();

public:
    /**
     * @brief Puntero a la interfaz nativa de la vista de profundidad/esténcil.
     */
    ID3D11DepthStencilView* m_depthStencilView = nullptr;
};