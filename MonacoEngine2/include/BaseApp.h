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

class BaseApp {
public:
    // Constructors & Destructor
    BaseApp(HINSTANCE hInst, int nCmdShow);
    ~BaseApp() { destroy(); }

    // Core Loop
    int     run(HINSTANCE hInst, int nCmdShow);
    HRESULT init(); //inicializar todas las clases (eso significa)
    void    update(float deltaTime);
    void    render();
    void    destroy();

private:
    // Windows Procedure Callback
    static LRESULT CALLBACK
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    // === Core Components ===
    Window              m_window;
    Device              m_device;
    DeviceContext       m_deviceContext;
    SwapChain           m_swapChain;

    // === Render Targets ===
    Texture             m_backBuffer;
    RenderTargetView    m_renderTargetView;
    Texture             m_depthStencil;
    DepthStencilView    m_depthStencilView;

    // === Pipeline & Scene ===
    Viewport            m_viewport;
    ShaderProgram       m_shaderProgram;
    MeshComponent       m_mesh;

    // === Buffers ===
    Buffer              m_vertexBuffer;
    Buffer              m_indexBuffer;
    Buffer              m_cbNeverChanges;
    Buffer              m_cbChangeOnResize;
    Buffer              m_cbChangesEveryFrame;

    // === Resources ===
    Texture             m_textureCube;
    SamplerState        m_samplerState;

    // === Matrices & Constants ===
    XMMATRIX            m_World;
    XMMATRIX            m_View;
    XMMATRIX            m_Projection;
    XMFLOAT4            m_vMeshColor; // (0.7f, 0.7f, 0.7f, 1.0f)

    // === Constant Buffer Structures ===
    CBChangeOnResize        cbChangesOnResize;
    CBNeverChanges          cbNeverChanges;
    CBChangesEveryFrame     cb;
};