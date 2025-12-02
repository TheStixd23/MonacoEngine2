#pragma once
/**
 * @file BaseApp.h
 * @brief Archivo de cabecera principal para la aplicación base del motor.
 *
 * Este archivo define la clase BaseApp, que actúa como el núcleo del motor gráfico.
 * Gestiona la creación de la ventana, la inicialización de DirectX 11, el bucle de mensajes,
 * la integración de ImGui y el ciclo de vida de los actores (Update/Render).
 */

#include "Prerequisites.h"

 // --- Includes de ImGui ---
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
// -------------------------

#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "MeshComponent.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "Model3D.h"
#include "ECS/Actor.h"

/**
 * @class BaseApp
 * @brief Clase central que encapsula la aplicación y el motor gráfico.
 *
 * BaseApp es responsable de orquestar todos los subsistemas del motor.
 * Maneja la ventana del sistema operativo, el dispositivo gráfico, la carga de recursos,
 * y ejecuta el bucle principal de la aplicación.
 */
class BaseApp {
public:
    /**
     * @brief Constructor por defecto.
     */
    BaseApp() = default;

    /**
     * @brief Destructor de la clase.
     * Llama automáticamente a destroy() para asegurar la liberación de recursos COM.
     */
    ~BaseApp() { destroy(); }

    /**
     * @brief Inicia el bucle principal de mensajes de la aplicación.
     * * Este método inicializa la ventana y entra en el bucle while(WM_QUIT) donde
     * se procesan los inputs y se llama a update() y render().
     * * @param hInst Manejador de la instancia de la aplicación (Win32).
     * @param nCmdShow Parámetro que indica cómo se debe mostrar la ventana (minimizada, maximizada, etc.).
     * @return int Código de salida de la aplicación (wParam del mensaje WM_QUIT).
     */
    int run(HINSTANCE hInst, int nCmdShow);

    /**
     * @brief Inicializa los subsistemas del motor.
     * * Configura DirectX 11 (Device, SwapChain, Views), compila shaders, crea buffers,
     * carga modelos iniciales e inicializa el contexto de ImGui.
     * * @return HRESULT S_OK si la inicialización fue exitosa, de lo contrario un código de error.
     */
    HRESULT init();

    /**
     * @brief Actualiza la lógica del juego y del motor.
     * * Se llama una vez por frame. Actualiza matrices, lógica de actores y prepara la UI.
     * * @param deltaTime Tiempo transcurrido desde el último frame en segundos.
     */
    void update(float deltaTime);

    /**
     * @brief Renderiza la escena y la interfaz de usuario.
     * * Limpia el backbuffer, dibuja la geometría 3D, dibuja la capa de ImGui
     * y presenta el resultado en pantalla (SwapChain Present).
     */
    void render();

    /**
     * @brief Libera los recursos y memoria.
     * * Se encarga de llamar a .destroy() o .Release() en todos los objetos DirectX y punteros.
     */
    void destroy();

private:
    /**
     * @brief Procedimiento de ventana (Callback) para manejar eventos de Windows.
     * * Procesa inputs de teclado, ratón y eventos de ventana (cerrar, redimensionar).
     * También pasa los eventos a ImGui.
     * * @param hWnd Manejador de la ventana.
     * @param message Identificador del mensaje.
     * @param wParam Parámetro adicional del mensaje.
     * @param lParam Parámetro adicional del mensaje.
     * @return LRESULT Resultado del procesamiento del mensaje.
     */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // --- FUNCIONES DE LA GUI (Interfaz de Usuario) ---

    /** @brief Configura los colores y estilo visual de ImGui ("Deep Space"). */
    void SetupGUIStyle();

    /** @brief Renderiza la barra de menú superior (File, Edit, Window). */
    void RenderMainMenuBar();

    /** @brief Renderiza el panel de jerarquía con la lista de actores. */
    void RenderHierarchy();

    /** @brief Renderiza el panel inspector con las propiedades del actor seleccionado. */
    void RenderInspector();

    /** @brief Renderiza el panel de consola/log en la parte inferior. */
    void RenderConsole();

private:
    // --- Componentes del Sistema ---
    Window                              m_window;           ///< Abstracción de la ventana de Win32.
    Device                              m_device;           ///< Wrapper del ID3D11Device (creación de recursos).
    DeviceContext                       m_deviceContext;    ///< Wrapper del ID3D11DeviceContext (comandos de renderizado).
    SwapChain                           m_swapChain;        ///< Cadena de intercambio para el doble buffer.

    // --- Recursos de Renderizado ---
    Texture                             m_backBuffer;       ///< Textura que representa el buffer de imagen actual.
    RenderTargetView                    m_renderTargetView; ///< Vista para dibujar en el backbuffer.
    Texture                             m_depthStencil;     ///< Textura para el buffer de profundidad.
    DepthStencilView                    m_depthStencilView; ///< Vista para el test de profundidad.
    Viewport                            m_viewport;         ///< Configuración del área de dibujo en pantalla.
    ShaderProgram                       m_shaderProgram;    ///< Gestión de Vertex y Pixel Shaders.

    // --- Buffers y Recursos de Escena ---
    Buffer                              m_cbNeverChanges;   ///< Buffer constante para datos estáticos (ej. proyección si no cambia).
    Buffer                              m_cbChangeOnResize; ///< Buffer constante para datos que cambian al redimensionar.

    Texture                             m_EspadaAlbedo;     ///< Textura específica para el modelo de la espada.

    // --- Matrices ---
    XMMATRIX                            m_View;             ///< Matriz de vista (Cámara).
    XMMATRIX                            m_Projection;       ///< Matriz de proyección.

    // --- Gestión de Entidades (ECS) ---
    std::vector<EU::TSharedPointer<Actor>> m_actors;        ///< Contenedor de todos los actores en la escena.
    EU::TSharedPointer<Actor>              m_Espada;        ///< Puntero al actor principal (ejemplo).

    Model3D* m_model;    ///< Puntero al modelo 3D cargado (raw data).

    // --- Estructuras para Constant Buffers ---
    CBChangeOnResize                    cbChangesOnResize;  ///< Estructura de datos para enviar al shader (Resize).
    CBNeverChanges                      cbNeverChanges;     ///< Estructura de datos para enviar al shader (Static).

    // --- Estado de la Interfaz ---
    int m_selectedActorIndex = -1; ///< Índice del actor actualmente seleccionado en la UI (-1 = ninguno).
};