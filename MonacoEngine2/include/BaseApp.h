#pragma once
#include "Prerequisites.h"
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
#include "ModelLoader.h"

/**
 * @class BaseApp
 * @brief Clase principal que gestiona la inicialización, actualización y renderizado de la aplicación base.
 *
 * Esta clase encapsula todos los componentes esenciales del pipeline gráfico de DirectX, incluyendo
 * la ventana, el dispositivo, el contexto de dispositivo, el swap chain y los buffers necesarios
 * para renderizar una escena 3D.
 *
 * @author Hannin Abarca
 */
class BaseApp {
public:
    /**
     * @brief Constructor de la clase BaseApp.
     * @param hInst Instancia del módulo actual del programa.
     * @param nCmdShow Parámetro que controla cómo se muestra la ventana.
     */
    BaseApp(HINSTANCE hInst, int nCmdShow);

    /**
     * @brief Destructor de la clase BaseApp.
     * Libera todos los recursos utilizados por la aplicación.
     */
    ~BaseApp() { destroy(); }

    /**
     * @brief Ejecuta el ciclo principal de la aplicación.
     * @param hInst Instancia del módulo actual del programa.
     * @param nCmdShow Parámetro que controla cómo se muestra la ventana.
     * @return Código de salida de la aplicación.
     */
    int run(HINSTANCE hInst, int nCmdShow);

    /**
     * @brief Inicializa todos los componentes necesarios para la ejecución de la aplicación.
     * @return HRESULT indicando si la inicialización fue exitosa.
     */
    HRESULT init();

    /**
     * @brief Actualiza la lógica de la aplicación en cada frame.
     * @param deltaTime Tiempo transcurrido desde el último frame.
     */
    void update(float deltaTime);

    /**
     * @brief Renderiza la escena en pantalla.
     */
    void render();

    /**
     * @brief Libera los recursos y destruye los objetos utilizados por la aplicación.
     */
    void destroy();

private:
    /**
     * @brief Función de callback del sistema operativo para manejar eventos de ventana.
     * @param hWnd Manejador de la ventana.
     * @param message Código del mensaje recibido.
     * @param wParam Parámetro adicional del mensaje.
     * @param lParam Parámetro adicional del mensaje.
     * @return LRESULT Resultado del procesamiento del mensaje.
     */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    /// Componente que gestiona la ventana principal de la aplicación.
    Window              m_window;

    /// Dispositivo gráfico DirectX utilizado para crear y gestionar recursos.
    Device              m_device;

    /// Contexto del dispositivo utilizado para emitir comandos de renderizado.
    DeviceContext       m_deviceContext;

    /// Encargado de la cadena de intercambio (swap chain) para presentar los frames en pantalla.
    SwapChain           m_swapChain;

    /// Textura utilizada como back buffer de renderizado.
    Texture             m_backBuffer;

    /// Vista del render target asociada al back buffer.
    RenderTargetView    m_renderTargetView;

    /// Textura usada como depth-stencil buffer.
    Texture             m_depthStencil;

    /// Vista para el depth-stencil buffer.
    DepthStencilView    m_depthStencilView;

    /// Define el área visible de renderizado.
    Viewport            m_viewport;

    /// Programa de shaders utilizado en la escena.
    ShaderProgram       m_shaderProgram;

    /// Componente que gestiona la malla 3D.
    MeshComponent       m_mesh;

    /// Cargador de modelos 3D desde archivos externos.
    ModelLoader         m_modelLoader;

    /// Buffer que almacena los vértices de los modelos.
    Buffer              m_vertexBuffer;

    /// Buffer que almacena los índices de los modelos.
    Buffer              m_indexBuffer;

    /// Buffer constante con valores que nunca cambian durante la ejecución.
    Buffer              m_cbNeverChanges;

    /// Buffer constante con valores que cambian al redimensionar la ventana.
    Buffer              m_cbChangeOnResize;

    /// Buffer constante con valores que cambian en cada frame.
    Buffer              m_cbChangesEveryFrame;

    /// Textura cúbica usada para efectos visuales o ambientales.
    Texture             m_textureCube;

    /// Estado del muestreador de texturas utilizado por los shaders.
    SamplerState        m_samplerState;

    /// Matriz de transformación del mundo.
    XMMATRIX            m_World;

    /// Matriz de vista (posición y orientación de la cámara).
    XMMATRIX            m_View;

    /// Matriz de proyección (perspectiva o ortográfica).
    XMMATRIX            m_Projection;

    /// Color del modelo renderizado.
    XMFLOAT4            m_vMeshColor;

    /// Estructura de buffer constante para cambios al redimensionar.
    CBChangeOnResize        cbChangesOnResize;

    /// Estructura de buffer constante con datos invariables.
    CBNeverChanges          cbNeverChanges;

    /// Estructura de buffer constante con datos actualizados cada frame.
    CBChangesEveryFrame     cb;
};
