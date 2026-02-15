# 🛠️ MonacoEngine2 — Motor Gráfico ECS (DirectX 11)

## 📋 Resumen

**MonacoEngine2** es una evolución del motor original, transformado en una arquitectura modular basada en componentes (**ECS - Entity Component System**) y desarrollado en C++ con **Direct3D 11**.

A diferencia de su predecesor, esta versión abandona la estructura monolítica para implementar un diseño profesional orientado a objetos, gestión de memoria personalizada, carga de modelos complejos (FBX) e integración de interfaces de depuración (ImGui).

## 🚀 Características Principales

### 🏗 Arquitectura y Diseño
* **Sistema ECS (Entity-Component-System):** Implementación de Entidades y Actores con componentes desacoplados (`Transform`, `MeshComponent`, etc.).
* **Gestión de Memoria Propia:** Implementación de Smart Pointers personalizados (`TSharedPointer`, `TUniquePtr`, `TWeakPointer`) para un control total del ciclo de vida de los objetos.
* **Librería Matemática Personalizada:** Motor matemático propio sin dependencias externas para álgebra lineal (`Matrix4x4`, `Quaternion`, `Vector2/3/4`).

### 🎨 Gráficos y Renderizado (DirectX 11)
* **Pipeline Programable:** Gestión encapsulada de Shaders (`Vertex` y `Pixel Shaders`) y `InputLayouts`.
* **Carga de Modelos 3D:** Integración completa con **FBX SDK** para cargar modelos complejos, no solo primitivas básicas.
* **Iluminación y Sombras:** Soporte para **Shadow Mapping** (visible en la clase `Actor`) e iluminación básica.
* **Texturizado:** Soporte para carga de texturas DDS, PNG, JPG y gestión de Samplers.
* **Multisampling:** Configuración de SwapChain con soporte 4x MSAA.

### 🛠 Herramientas y Utilidades
* **ImGui Integrado:** Interfaz gráfica inmediata para depuración y modificación de parámetros en tiempo real.
* **Logging System:** Macros de depuración (`MESSAGE`, `ERROR`) para trazabilidad de errores en la Output Window.

---

## 🏛 Arquitectura del Sistema

### Jerarquía de Clases Principal
El motor se estructura bajo una jerarquía ECS:

| Clase | Responsabilidad |
|---|---|
| **`Entity`** | Clase base de todos los objetos de la escena. |
| **`Actor`** | Hereda de `Entity`. Representa un objeto renderizable. Gestiona Mallas, Texturas y recursos de Sombras. |
| **`Component`** | Clase base para funcionalidades adjuntas a las entidades. |
| **`MeshComponent`** | Componente que contiene la geometría del objeto. |
| **`Transform`** | Componente que maneja Posición, Rotación y Escala. |

### Diagrama de Clases (ECS)

```mermaid
classDiagram
    class Entity {
        +init()
        +update()
        +render()
    }
    class Actor {
        -vector~MeshComponent~ m_meshes
        -vector~Texture~ m_textures
        +renderShadow()
        +setCastShadow()
    }
    class Component {
        +update()
    }
    class Transform {
        +SetPosition()
        +SetRotation()
    }
    
    Entity <|-- Actor
    Actor *-- Component
    Component <|-- Transform
    Component <|-- MeshComponent
