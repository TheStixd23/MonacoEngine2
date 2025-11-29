#pragma once

// ============================================================================
// Librerías estándar
// ============================================================================
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <xnamath.h>
#include <thread>
#include <fstream> // Lectura de archivos (.obj, etc.)
#include <map>     // Mapa para evitar duplicar vértices

// ============================================================================
// Librerías DirectX
// ============================================================================
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Resource.h"
#include "resource.h"

// ============================================================================
// Macros de utilidad
// ============================================================================

/** Libera un recurso COM de forma segura. */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

/** Muestra mensaje de creación de recurso en la consola de depuración. */
#define MESSAGE(classObj, method, state)    \
{                                           \
   std::wostringstream os_;                 \
   os_ << classObj << "::" << method        \
       << " : " << "[CREATION OF RESOURCE : " << state << "]\n"; \
   OutputDebugStringW(os_.str().c_str());   \
}

/** Muestra un mensaje de error formateado en la consola de depuración. */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try {                                                     \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    } catch (...) {                                           \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}

// ============================================================================
// Estructuras base
// ============================================================================

/** Vértice simple con posición, textura y normal. */
struct SimpleVertex {
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
    XMFLOAT3 Norm;
};

/** Constantes para vista. */
struct CBNeverChanges {
    XMMATRIX mView;
};

/** Constantes para proyección. */
struct CBChangeOnResize {
    XMMATRIX mProjection;
};

/** Constantes para transformación y color. */
struct CBChangesEveryFrame {
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};

// ============================================================================
// Enumeraciones
// ============================================================================

/** Tipos de extensión soportados. */
enum ExtensionType {
    DDS = 0,
    PNG = 1,
    JPG = 2
};

/** Tipos de shader. */
enum ShaderType {
    VERTEX_SHADER = 0,
    PIXEL_SHADER = 1
};
