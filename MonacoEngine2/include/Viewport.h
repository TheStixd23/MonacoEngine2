#pragma once
#include "Prerequisites.h"

class Window;
class DeviceContext;

/**
 * @class Viewport
 * @brief Encapsula un @c D3D11_VIEWPORT para definir la región de renderizado en la pantalla.
 *
 * Un viewport en Direct3D 11 especifica el área rectangular del render target donde
 * se dibujan las primitivas. Puede inicializarse desde una ventana o con dimensiones personalizadas.
 *
 * @author Hannin Abarca
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
     * @brief Inicializa el viewport a partir del tamaño de una ventana.
     * @param window Ventana que define el área de renderizado.
     * @return @c S_OK si la inicialización fue exitosa.
     */
    HRESULT init(const Window& window);

    /**
     * @brief Inicializa el viewport con dimensiones específicas.
     * @param width  Ancho del viewport en píxeles.
     * @param height Alto del viewport en píxeles.
     * @return @c S_OK si la inicialización fue exitosa.
     */
    HRESULT init(unsigned int width, unsigned int height);

    /**
     * @brief Actualiza parámetros del viewport
     * @note Actualmente no realiza ninguna operación.
     */
    void update();

    /**
     * @brief Aplica el viewport al contexto de dispositivo.
     * @param deviceContext Contexto donde se aplicará.
     */
    void render(DeviceContext& deviceContext);

    /**
     * @brief Libera recursos del viewport
     */
    void destroy() {}

public:
    /**
     * @brief Estructura nativa de Direct3D 11 que define el viewport.
     */
    D3D11_VIEWPORT m_viewport;
};
