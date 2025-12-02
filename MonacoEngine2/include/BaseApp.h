#pragma once
#include "Prerequisites.h"

// Includes de ImGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

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

class BaseApp {
public:
    BaseApp() = default;
    ~BaseApp() { destroy(); }

    int run(HINSTANCE hInst, int nCmdShow);

    HRESULT init();
    void update(float deltaTime);
    void render();
    void destroy();

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    // --- NUEVAS FUNCIONES PARA LA GUI (Basadas en MungoEngine) ---
    void SetupGUIStyle();       // Configura los colores oscuros
    void RenderMainMenuBar();   // Dibuja la barra de menú superior
    void RenderHierarchy();     // Dibuja la lista de actores
    void RenderInspector();     // Dibuja las propiedades del actor seleccionado
    void RenderConsole();       // Dibuja la consola (logs)
    // -------------------------------------------------------------

private:
    Window                              m_window;
    Device                              m_device;
    DeviceContext                       m_deviceContext;
    SwapChain                           m_swapChain;
    Texture                             m_backBuffer;
    RenderTargetView                    m_renderTargetView;
    Texture                             m_depthStencil;
    DepthStencilView                    m_depthStencilView;
    Viewport                            m_viewport;
    ShaderProgram                       m_shaderProgram;

    Buffer                              m_cbNeverChanges;
    Buffer                              m_cbChangeOnResize;

    Texture                             m_cyberGunAlbedo;

    XMMATRIX                            m_View;
    XMMATRIX                            m_Projection;

    std::vector<EU::TSharedPointer<Actor>> m_actors;
    EU::TSharedPointer<Actor> m_cyberGun;

    Model3D* m_model;

    CBChangeOnResize                    cbChangesOnResize;
    CBNeverChanges                      cbNeverChanges;

    // --- ESTADO DE LA GUI ---
    int m_selectedActorIndex = -1; // Índice del actor seleccionado en la jerarquía
};