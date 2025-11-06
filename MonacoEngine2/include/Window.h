#pragma once
#include "Prerequisites.h"

/**
 * @class Window
 * @brief Encapsula una ventana Win32 para renderizado con Direct3D 11.
 *
 * Administra la creación, actualización, renderizado y destrucción
 * de una ventana principal del motor gráfico.
 *
 * @author Hannin Abarca
 */
class Window {
public:
    /**
     * @brief Constructor por defecto.
     */
    Window() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~Window() = default;

    /**
     * @brief Inicializa la ventana del motor.
     * @param hInstance Instancia de la aplicación.
     * @param nCmdShow  Parámetro del sistema para mostrar la ventana.
     * @param wndproc   Procedimiento de ventana (callback).
     * @return @c S_OK si la creación fue exitosa.
     */
    HRESULT init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

    /**
     * @brief Actualiza el estado de la ventana (mensajes, eventos, etc.).
     */
    void update();

    /**
     * @brief Renderiza el contenido asociado a la ventana.
     * @note Método de marcador; implementación dependiente del contexto gráfico.
     */
    void render();

    /**
     * @brief Libera y destruye la ventana.
     */
    void destroy();

public:
    /**
     * @brief Manejador de la ventana Win32.
     */
    HWND m_hWnd = nullptr;

    /**
     * @brief Dimensiones de la ventana en píxeles.
     */
    unsigned int m_width;
    unsigned int m_height;

private:
    /**
     * @brief Instancia de la aplicación Win32.
     */
    HINSTANCE m_hInst = nullptr;

    /**
     * @brief Rectángulo que define el área visible de la ventana.
     */
    RECT m_rect;

    /**
     * @brief Nombre de la ventana del motor.
     */
    std::string m_windowName = "Monaco Engine 2v";
};
