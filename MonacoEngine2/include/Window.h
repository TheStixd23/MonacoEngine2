#pragma once
/**
 * @file Window.h
 * @brief Definición de la clase Window.
 *
 * Encapsula la creación y gestión de una ventana del sistema operativo (Win32).
 * Se encarga de registrar la clase de ventana, crear la instancia y gestionar sus dimensiones.
 *
 * @author MonacoEngine Team
 */

#include "Prerequisites.h"

 /**
  * @class Window
  * @brief Clase que administra una ventana de aplicación en Windows (Win32 API).
  *
  * Esta clase abstrae el proceso de registro y creación de ventanas de Windows.
  * Proporciona el manejador de ventana (HWND) necesario para inicializar DirectX
  * y almacena las dimensiones del área cliente.
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
     * @brief Inicializa y muestra la ventana de la aplicación.
     *
     * Realiza el registro de la clase de ventana (WNDCLASSEX) y llama a CreateWindow.
     * También ajusta el tamaño de la ventana para asegurar que el área cliente tenga
     * las dimensiones deseadas y la muestra en pantalla.
     *
     * @param hInstance Instancia de la aplicación (proporcionada por WinMain).
     * @param nCmdShow  Estado de visualización inicial de la ventana (minimizado, maximizado, etc.).
     * @param wndproc   Puntero a la función de procedimiento de ventana (Callback de mensajes).
     * @return HRESULT @c S_OK si la ventana se creó correctamente; @c E_FAIL en caso contrario.
     *
     * @post @c m_hWnd será válido y @c m_width / @c m_height tendrán las dimensiones del área cliente.
     */
    HRESULT init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

    /**
     * @brief Método de actualización de la ventana.
     *
     * @note Actualmente no realiza ninguna operación (placeholder).
     * La lógica de mensajes se suele manejar en el bucle principal de la aplicación.
     */
    void update();

    /**
     * @brief Método de renderizado de la ventana.
     *
     * @note Actualmente no realiza ninguna operación (placeholder).
     * El renderizado gráfico se delega al SwapChain y DeviceContext.
     */
    void render();

    /**
     * @brief Libera los recursos de la ventana.
     *
     * @note Actualmente no realiza ninguna operación explícita, ya que el SO libera
     * la ventana al terminar el proceso, pero podría usarse para UnregisterClass.
     */
    void destroy();

public:
    /**
     * @brief Manejador (Handle) de la ventana creada.
     * Necesario para inicializar DirectX y para funciones de la API de Windows.
     */
    HWND m_hWnd = nullptr;

    /**
     * @brief Ancho del área cliente de la ventana en píxeles.
     */
    unsigned int m_width = 0;

    /**
     * @brief Alto del área cliente de la ventana en píxeles.
     */
    unsigned int m_height = 0;

private:
    /**
     * @brief Instancia de la aplicación asociada a esta ventana.
     */
    HINSTANCE m_hInst = nullptr;

    /**
     * @brief Rectángulo que define las dimensiones de la ventana.
     */
    RECT m_rect = { 0, 0, 0, 0 };

    /**
     * @brief Nombre o título que aparecerá en la barra de la ventana.
     */
    std::string m_windowName = "Monaco Engine";
};