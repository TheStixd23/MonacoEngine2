#pragma once
/**
 * @file Viewport.h
 * @brief Definición de la clase Viewport.
 *
 * Encapsula la estructura D3D11_VIEWPORT, que define el área rectangular
 * de la ventana (o render target) donde se rasterizará la escena.
 *
 * @author MonacoEngine Team
 */

#include "Prerequisites.h"

class Window;
class DeviceContext;

/**
 * @class Viewport
 * @brief Encapsula un @c D3D11_VIEWPORT para definir la región de renderizado.
 *
 * Un viewport en Direct3D 11 especifica cómo se mapean las coordenadas normalizadas
 * del dispositivo (NDC) a los píxeles del render target.
 * Controla:
 * - Dimensiones (Ancho/Alto).
 * - Posición (TopLeftX/Y).
 * - Rango de profundidad (MinDepth/MaxDepth).
 */
class Viewport {
public:
    /**
     * @brief Constructor por defecto.
     */
    Viewport() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~Viewport() = default;

    /**
     * @brief Inicializa el viewport a partir de las dimensiones de una ventana.
     *
     * Configura el viewport para cubrir toda el área cliente de la ventana proporcionada.
     * Profundidad estándar: 0.0f a 1.0f.
     *
     * @param window Referencia a la ventana de la cual se tomarán las dimensiones.
     * @return HRESULT @c S_OK si es exitoso; código de error si la ventana no es válida.
     */
    HRESULT init(const Window& window);

    /**
     * @brief Inicializa el viewport con dimensiones específicas manuales.
     *
     * Útil cuando se quiere renderizar en una sub-región de la pantalla o en una
     * textura de tamaño diferente al de la ventana.
     *
     * @param width  Ancho del viewport en píxeles.
     * @param height Alto del viewport en píxeles.
     * @return HRESULT @c S_OK si es exitoso.
     */
    HRESULT init(unsigned int width, unsigned int height);

    /**
     * @brief Actualiza los parámetros del viewport.
     * @note Método reservado para lógica futura (ej. responder a WM_SIZE).
     */
    void update();

    /**
     * @brief Activa el viewport en el pipeline de rasterización.
     *
     * Llama a @c RSSetViewports en el contexto proporcionado.
     *
     * @param deviceContext Contexto del dispositivo donde se aplicará el viewport.
     * @pre Debe haberse llamado a @c init() previamente.
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Libera recursos.
     * @note D3D11_VIEWPORT es una estructura POD (Plain Old Data), no requiere Release().
     */
    void destroy() {}

public:
    /**
     * @brief Estructura nativa de Direct3D que almacena la configuración del viewport.
     */
    D3D11_VIEWPORT m_viewport = {};
};