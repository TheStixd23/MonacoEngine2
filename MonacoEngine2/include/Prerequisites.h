#pragma once
/**
 * @file Prerequisites.h
 * @brief Archivo de cabecera con inclusiones, macros y definiciones comunes.
 *
 * Este archivo actúa como un encabezado precompilado o centralizador de dependencias.
 * Incluye las bibliotecas estándar (STL), las APIs de Windows y DirectX 11, así como
 * utilidades propias del motor (Vectores, Punteros Inteligentes).
 * También define macros de depuración y estructuras de datos fundamentales para los shaders.
 *
 * @author Hannin Abarca
 */

 // Librerias STD
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <xnamath.h>
#include <thread>
#include <memory>
#include <unordered_map>
#include <type_traits>

// Librerias DirectX
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "Resource.h"
#include "resource.h"

// Third Party Libraries (Utilidades del Motor)
#include "EngineUtilities/Vectors/Vector2.h"
#include "EngineUtilities/Vectors/Vector3.h"
#include "EngineUtilities/Memory/TSharedPointer.h"
#include "EngineUtilities/Memory/TWeakPointer.h"
#include "EngineUtilities/Memory/TStaticPtr.h"
#include "EngineUtilities/Memory/TUniquePtr.h"

// --------------------------------------------------------------------------------------
// MACROS DE UTILIDAD
// --------------------------------------------------------------------------------------

/**
 * @def SAFE_RELEASE(x)
 * @brief Libera un puntero COM de manera segura.
 *
 * Verifica si el puntero es diferente de nullptr, llama a su método Release()
 * y luego lo establece a nullptr para evitar punteros colgantes.
 */
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

 /**
  * @def MESSAGE(classObj, method, state)
  * @brief Imprime un mensaje informativo en la ventana de salida de depuración (Output Window).
  *
  * Útil para trazar la creación de recursos o cambios de estado importantes.
  * @param classObj Nombre de la clase.
  * @param method   Nombre del método.
  * @param state    Mensaje o estado a reportar.
  */
#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " << "[CREATION OF RESOURCE " << ": " << state << "] \n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

  /**
   * @def ERROR(classObj, method, errorMSG)
   * @brief Imprime un mensaje de error en la ventana de salida de depuración.
   *
   * Incluye un bloque try-catch interno para evitar que el logging de errores lance excepciones.
   * @param classObj Nombre de la clase donde ocurrió el error.
   * @param method   Nombre del método donde ocurrió el error.
   * @param errorMSG Descripción del error.
   */
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

   //--------------------------------------------------------------------------------------
   // Structures (Estructuras de Datos para Shaders)
   //--------------------------------------------------------------------------------------

   /**
    * @struct SimpleVertex
    * @brief Estructura de vértice simple.
    *
    * Utilizada para definir la geometría básica. Contiene posición y coordenadas de textura.
    * Debe coincidir con el Input Layout definido en el Shader.
    */
struct SimpleVertex
{
    XMFLOAT3 Pos; ///< Posición en el espacio 3D (x, y, z).
    XMFLOAT2 Tex; ///< Coordenadas de textura (u, v).
};

/**
 * @struct CBNeverChanges
 * @brief Estructura para Constant Buffer que rara vez cambia (ej. Matriz de Vista).
 *
 * Se actualiza normalmente una vez por frame o cuando la cámara se mueve.
 */
struct CBNeverChanges
{
    XMMATRIX mView; ///< Matriz de Vista (Cámara).
};

/**
 * @struct CBChangeOnResize
 * @brief Estructura para Constant Buffer que cambia al redimensionar la ventana.
 *
 * Contiene la matriz de proyección, que depende de la relación de aspecto (aspect ratio).
 */
struct CBChangeOnResize
{
    XMMATRIX mProjection; ///< Matriz de Proyección.
};

/**
 * @struct CBChangesEveryFrame
 * @brief Estructura para Constant Buffer que cambia en cada cuadro.
 *
 * Contiene datos dinámicos por objeto, como la matriz de mundo y colores.
 */
struct CBChangesEveryFrame
{
    XMMATRIX mWorld;      ///< Matriz de Mundo (Transformación del objeto).
    XMFLOAT4 vMeshColor;  ///< Color base del objeto (RGBA).
};

//--------------------------------------------------------------------------------------
// Enumeraciones
//--------------------------------------------------------------------------------------

/**
 * @enum ExtensionType
 * @brief Tipos de extensión de archivo de imagen soportados.
 */
enum ExtensionType {
    DDS = 0, ///< Formato DirectDraw Surface (optimizado para GPU).
    PNG = 1, ///< Formato Portable Network Graphics.
    JPG = 2  ///< Formato JPEG.
};

/**
 * @enum ShaderType
 * @brief Identifica el tipo de shader en el pipeline programable.
 */
enum ShaderType {
    VERTEX_SHADER = 0, ///< Shader de Vértices (procesa geometría).
    PIXEL_SHADER = 1   ///< Shader de Píxeles (procesa color y texturas).
};

/**
 * @enum ComponentType
 * @brief Identificadores para los tipos de componentes en el sistema ECS.
 *
 * Usado para identificar y gestionar componentes adjuntos a las entidades (Actores).
 */
enum ComponentType {
    NONE = 0,      ///< Tipo de componente no especificado o nulo.
    TRANSFORM = 1, ///< Componente de transformación (posición, rotación, escala).
    MESH = 2,      ///< Componente de malla (geometría).
    MATERIAL = 3   ///< Componente de material (texturas, shaders).
};