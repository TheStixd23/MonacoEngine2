#include "BaseApp.h"
#include "ResourceManager.h"
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>

// --- IMGUI INCLUDES ---
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

// Declaración del handler externo
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ---------------------------------------------------------
// ESTADO GLOBAL DEL EDITOR
// ---------------------------------------------------------
static int   g_SelectedActorIndex = -1;
static bool  g_ShowHierarchy = true;
static bool  g_ShowInspector = true;
static float g_RotationSensitivity = 1.0f;

// Variables para suavizado de cámara/edición
static EU::Vector3 g_TargetRotation = EU::Vector3(0, 0, 0);

// ---------------------------------------------------------
// ESTILOS VISUALES "DEEP SPACE" (PROFESIONAL)
// ---------------------------------------------------------
void SetupPowerStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Geometría moderna y suave
    style.WindowRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 10.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 6.0f;

    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.PopupBorderSize = 1.0f;

    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.IndentSpacing = 20.0f;

    // Paleta de Colores "Deep Space"
    ImVec4* colors = style.Colors;

    const ImVec4 bgColor = ImVec4(0.09f, 0.09f, 0.12f, 1.00f); // Fondo muy oscuro azulado
    const ImVec4 panelColor = ImVec4(0.13f, 0.13f, 0.18f, 1.00f); // Paneles
    const ImVec4 accentColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Azul brillante
    const ImVec4 accentHover = ImVec4(0.36f, 0.69f, 1.00f, 1.00f);
    const ImVec4 textSecondary = ImVec4(0.60f, 0.65f, 0.75f, 1.00f);

    colors[ImGuiCol_WindowBg] = bgColor;
    colors[ImGuiCol_ChildBg] = panelColor;
    colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.95f);
    colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.30f, 0.50f);

    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = textSecondary;

    // Headers (Listas)
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.32f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.38f, 1.00f);

    // Botones
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = accentColor;
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.45f, 0.80f, 1.00f);

    // Inputs
    colors[ImGuiCol_FrameBg] = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.09f, 0.12f, 1.00f);

    // Tabs y Títulos
    colors[ImGuiCol_TitleBg] = bgColor;
    colors[ImGuiCol_TitleBgActive] = bgColor;
    colors[ImGuiCol_TitleBgCollapsed] = bgColor;
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);

    colors[ImGuiCol_Tab] = panelColor;
    colors[ImGuiCol_TabHovered] = accentHover;
    colors[ImGuiCol_TabActive] = accentColor;
    colors[ImGuiCol_TabUnfocused] = panelColor;
    colors[ImGuiCol_TabUnfocusedActive] = panelColor;

    colors[ImGuiCol_CheckMark] = accentColor;
    colors[ImGuiCol_SliderGrab] = accentColor;
    colors[ImGuiCol_SliderGrabActive] = accentHover;
}

// --- HELPER SEGURO PARA VECTORES 3D ---
void DrawVec3Control(const char* label, float* values, float resetValue = 0.0f, float columnWidth = 80.0f) {
    ImGui::PushID(label);

    if (ImGui::BeginTable("Vec3Table", 4, ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, columnWidth);
        ImGui::TableSetupColumn("X");
        ImGui::TableSetupColumn("Y");
        ImGui::TableSetupColumn("Z");

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);

        float buttonWidth = ImGui::GetFrameHeight();

        // Eje X (Rojo)
        ImGui::TableSetColumnIndex(1);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
        if (ImGui::Button("X", ImVec2(buttonWidth, 0))) values[0] = resetValue;
        ImGui::PopStyleColor(2);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopStyleVar();

        // Eje Y (Verde)
        ImGui::TableSetColumnIndex(2);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        if (ImGui::Button("Y", ImVec2(buttonWidth, 0))) values[1] = resetValue;
        ImGui::PopStyleColor(2);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::DragFloat("##Y", &values[1], 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopStyleVar();

        // Eje Z (Azul)
        ImGui::TableSetColumnIndex(3);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f));
        if (ImGui::Button("Z", ImVec2(buttonWidth, 0))) values[2] = resetValue;
        ImGui::PopStyleColor(2);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::DragFloat("##Z", &values[2], 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopStyleVar();

        ImGui::EndTable();
    }
    ImGui::PopID();
}

// ---------------------------------------------------------
// MATEMÁTICAS (RAYCASTING SIMPLE)
// ---------------------------------------------------------
bool RaySphereIntersect(XMVECTOR rayOrigin, XMVECTOR rayDir, XMVECTOR sphereCenter, float sphereRadius, float& outDist) {
    XMVECTOR L = XMVectorSubtract(sphereCenter, rayOrigin);
    XMVECTOR tcaVec = XMVector3Dot(L, rayDir);
    float tca = XMVectorGetX(tcaVec);
    if (tca < 0) return false;

    XMVECTOR d2Vec = XMVector3Dot(L, L) - (tcaVec * tcaVec);
    float d2 = XMVectorGetX(d2Vec);
    float radius2 = sphereRadius * sphereRadius;

    if (d2 > radius2) return false;

    float thc = sqrt(radius2 - d2);
    outDist = tca - thc;
    return true;
}

// ---------------------------------------------------------
// CICLO DE VIDA DE LA APLICACIÓN
// ---------------------------------------------------------

int BaseApp::run(HINSTANCE hInst, int nCmdShow) {
    if (FAILED(m_window.init(hInst, nCmdShow, WndProc))) return 0;
    if (FAILED(init())) return 0;

    MSG msg = {};
    LARGE_INTEGER freq, prev;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&prev);

    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            LARGE_INTEGER curr;
            QueryPerformanceCounter(&curr);
            float deltaTime = static_cast<float>(curr.QuadPart - prev.QuadPart) / freq.QuadPart;
            prev = curr;
            update(deltaTime);
            render();
        }
    }
    return (int)msg.wParam;
}

HRESULT BaseApp::init() {
    HRESULT hr = S_OK;

    // 1. Inicializar Gráficos
    hr = m_swapChain.init(m_device, m_deviceContext, m_backBuffer, m_window); if (FAILED(hr)) return hr;
    hr = m_renderTargetView.init(m_device, m_backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM); if (FAILED(hr)) return hr;
    hr = m_depthStencil.init(m_device, m_window.m_width, m_window.m_height, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, 4, 0); if (FAILED(hr)) return hr;
    hr = m_depthStencilView.init(m_device, m_depthStencil, DXGI_FORMAT_D24_UNORM_S8_UINT); if (FAILED(hr)) return hr;
    hr = m_viewport.init(m_window); if (FAILED(hr)) return hr;

    // 2. Cargar Recursos y Actores
    m_Espada = EU::MakeShared<Actor>(m_device);
    if (!m_Espada.isNull()) {
        m_model = new Model3D("AnyConv.com__Espada.fbx", ModelType::FBX);
        hr = m_EspadaAlbedo.init(m_device, "crucible_baseColor", ExtensionType::PNG);

        if (SUCCEEDED(hr)) {
            std::vector<Texture> textures;
            textures.push_back(m_EspadaAlbedo);
            m_Espada->setMesh(m_device, m_model->GetMeshes());
            m_Espada->setTextures(textures);
            m_Espada->setName("Doom Sword");

            // Posición inicial
            m_Espada->getComponent<Transform>()->setTransform(
                EU::Vector3(2.0f, -4.90f, 11.60f),
                EU::Vector3(-0.60f, 3.0f, -0.20f),
                EU::Vector3(1.0f, 1.0f, 1.0f)
            );
            m_actors.push_back(m_Espada);
        }
    }

    // 3. Shaders y Buffers
    std::vector<D3D11_INPUT_ELEMENT_DESC> Layout = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = m_shaderProgram.init(m_device, "MonacoEngine2.fx", Layout); if (FAILED(hr)) return hr;
    hr = m_cbNeverChanges.init(m_device, sizeof(CBNeverChanges)); if (FAILED(hr)) return hr;
    hr = m_cbChangeOnResize.init(m_device, sizeof(CBChangeOnResize)); if (FAILED(hr)) return hr;

    // Matrices
    XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_View = XMMatrixLookAtLH(Eye, At, Up);
    cbNeverChanges.mView = XMMatrixTranspose(m_View);
    m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);

    // --- 4. INICIALIZACIÓN UI ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SetupPowerStyle(); // Aplicar el nuevo estilo

    ImGui_ImplWin32_Init(m_window.m_hWnd);
    ImGui_ImplDX11_Init(m_device.m_device, m_deviceContext.m_deviceContext);

    return S_OK;
}

void BaseApp::update(float deltaTime) {
    // Actualizar Matrices
    cbNeverChanges.mView = XMMatrixTranspose(m_View);
    m_cbNeverChanges.update(m_deviceContext, nullptr, 0, nullptr, &cbNeverChanges, 0, 0);
    m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (FLOAT)m_window.m_height, 0.01f, 100.0f);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
    m_cbChangeOnResize.update(m_deviceContext, nullptr, 0, nullptr, &cbChangesOnResize, 0, 0);

    // --- LÓGICA DE SELECCIÓN (Sin Targets Extras) ---
    ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureMouse) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            POINT mousePos; GetCursorPos(&mousePos); ScreenToClient(m_window.m_hWnd, &mousePos);
            float mouseX = (float)mousePos.x;
            float mouseY = (float)mousePos.y;

            XMVECTOR mouseNear = XMVectorSet(mouseX, mouseY, 0.0f, 0.0f);
            XMVECTOR mouseFar = XMVectorSet(mouseX, mouseY, 1.0f, 0.0f);
            XMVECTOR rayOrigin = XMVector3Unproject(mouseNear, 0, 0, m_window.m_width, m_window.m_height, 0.0f, 1.0f, m_Projection, m_View, XMMatrixIdentity());
            XMVECTOR rayEnd = XMVector3Unproject(mouseFar, 0, 0, m_window.m_width, m_window.m_height, 0.0f, 1.0f, m_Projection, m_View, XMMatrixIdentity());
            XMVECTOR rayDir = XMVector3Normalize(rayEnd - rayOrigin);

            int hitIndex = -1;
            float closestDist = FLT_MAX;

            for (size_t i = 0; i < m_actors.size(); ++i) {
                auto transform = m_actors[i]->getComponent<Transform>();
                if (transform) {
                    EU::Vector3 pos = transform->getPosition();
                    XMVECTOR center = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
                    float avgScale = (transform->getScale().x + transform->getScale().y + transform->getScale().z) / 3.0f;
                    float radius = 2.0f * avgScale;

                    float dist = 0.0f;
                    if (RaySphereIntersect(rayOrigin, rayDir, center, radius, dist)) {
                        if (dist < closestDist) {
                            closestDist = dist;
                            hitIndex = (int)i;
                        }
                    }
                }
            }

            g_SelectedActorIndex = hitIndex;
        }
    }

    // Actualizar Actores
    for (auto& actor : m_actors) {
        actor->update(deltaTime, m_deviceContext);
    }
}

void BaseApp::render() {
    // 1. Inicio de Frame UI
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // --- A. BARRA DE MENÚ PRINCIPAL ---
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) PostQuitMessage(0);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")) {
            ImGui::MenuItem("Hierarchy", nullptr, &g_ShowHierarchy);
            ImGui::MenuItem("Inspector", nullptr, &g_ShowInspector);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Dimensiones de la pantalla
    float screenW = (float)m_window.m_width;
    float screenH = (float)m_window.m_height;
    float menuH = 19.0f; // Aprox altura del menú
    float sidePanelW = 280.0f;

    // --- B. PANEL DE JERARQUÍA (Izquierda) ---
    if (g_ShowHierarchy) {
        ImGui::SetNextWindowPos(ImVec2(10, menuH + 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(sidePanelW, screenH * 0.6f), ImGuiCond_FirstUseEver);

        ImGui::Begin("Hierarchy", &g_ShowHierarchy);

        // Buscador
        static char filter[128] = "";
        ImGui::PushItemWidth(-1);
        ImGui::InputTextWithHint("##search", "Search...", filter, IM_ARRAYSIZE(filter));
        ImGui::PopItemWidth();
        ImGui::Separator();

        // Lista
        ImGui::BeginChild("HierarchyList");
        for (int i = 0; i < m_actors.size(); ++i) {
            ImGui::PushID(i);
            bool isSelected = (g_SelectedActorIndex == i);
            std::string name = m_actors[i]->getName();
            if (name.empty()) name = "Untitled Actor";

            std::string label = "[O] " + name;

            if (ImGui::Selectable(label.c_str(), isSelected)) {
                g_SelectedActorIndex = i;
            }
            ImGui::PopID();
        }
        ImGui::EndChild();
        ImGui::End();
    }

    // --- C. PANEL INSPECTOR (Derecha) ---
    if (g_ShowInspector) {
        ImGui::SetNextWindowPos(ImVec2(screenW - sidePanelW - 10, menuH + 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(sidePanelW, screenH * 0.8f), ImGuiCond_FirstUseEver);

        ImGui::Begin("Inspector", &g_ShowInspector);

        if (g_SelectedActorIndex >= 0 && g_SelectedActorIndex < m_actors.size()) {
            auto actor = m_actors[g_SelectedActorIndex];

            // Cabecera del Actor
            bool active = true;
            ImGui::Checkbox("##Active", &active);
            ImGui::SameLine();

            char nameBuffer[128];
#ifdef _MSC_VER
            strcpy_s(nameBuffer, actor->getName().c_str());
#else
            strncpy(nameBuffer, actor->getName().c_str(), sizeof(nameBuffer));
#endif

            ImGui::PushItemWidth(-1);
            if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
                actor->setName(nameBuffer);
            }
            ImGui::PopItemWidth();

            ImGui::Spacing();
            ImGui::Separator();

            // Transform
            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto transform = actor->getComponent<Transform>();
                if (transform) {
                    EU::Vector3 pos = transform->getPosition();
                    EU::Vector3 rot = transform->getRotation();
                    EU::Vector3 scl = transform->getScale();

                    float p[3] = { pos.x, pos.y, pos.z };
                    float r[3] = { rot.x, rot.y, rot.z };
                    float s[3] = { scl.x, scl.y, scl.z };

                    ImGui::Spacing();
                    DrawVec3Control("Location", p);
                    DrawVec3Control("Rotation", r);
                    DrawVec3Control("Scale   ", s, 1.0f);
                    ImGui::Spacing();

                    transform->setPosition(EU::Vector3(p[0], p[1], p[2]));
                    transform->setRotation(EU::Vector3(r[0], r[1], r[2]));
                    transform->setScale(EU::Vector3(s[0], s[1], s[2]));
                }
            }
            // SIN BOTÓN ADD COMPONENT AQUÍ

        }
        else {
            // Mensaje cuando no hay selección
            float winW = ImGui::GetWindowSize().x;
            float textW = ImGui::CalcTextSize("No Selection").x;
            ImGui::SetCursorPosX((winW - textW) * 0.5f);
            ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.4f);
            ImGui::TextDisabled("No Selection");
        }
        ImGui::End();
    }

    // 2. Renderizar Escena 3D (Fondo)
    float ClearColor[4] = { 0.15f, 0.15f, 0.18f, 1.0f }; // Gris azulado acorde al tema
    m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);

    m_viewport.render(m_deviceContext);
    m_depthStencilView.render(m_deviceContext);
    m_shaderProgram.render(m_deviceContext);
    m_cbNeverChanges.render(m_deviceContext, 0, 1);
    m_cbChangeOnResize.render(m_deviceContext, 1, 1);

    for (auto& actor : m_actors) {
        actor->render(m_deviceContext);
    }

    // 3. Finalizar UI
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // 4. Presentar
    m_swapChain.present();
}

void BaseApp::destroy() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (m_deviceContext.m_deviceContext) m_deviceContext.m_deviceContext->ClearState();

    m_cbNeverChanges.destroy();
    m_cbChangeOnResize.destroy();
    m_shaderProgram.destroy();
    m_depthStencil.destroy();
    m_depthStencilView.destroy();
    m_renderTargetView.destroy();
    m_swapChain.destroy();
    m_backBuffer.destroy();
    m_deviceContext.destroy();
    m_device.destroy();
}

LRESULT BaseApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message) {
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}