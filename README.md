\# 🛠️ MonacoEngine — Motor Gráfico (Proyecto Parcial)



\## 📋 Resumen  

MonacoEngine es un motor gráfico 3D educativo desarrollado para la materia de Gráficas Computacionales. Tiene por objetivo servir como base para exploración de pipelines gráficos, renderizado, manejo de recursos, escenas y shaders.



\## 📌 Índice  

\- \[Resumen](#-resumen)  

\- \[Objetivos del proyecto](#-objetivos-del-proyecto)  

\- \[Arquitectura general](#-arquitectura-general)  

&nbsp; - \[Componentes principales](#componentes-principales)  

&nbsp; - \[Relaciones operativas](#relaciones-operativas)  

\- \[Pipeline gráfico implementado](#-pipeline-gráfico-implementado)  

\- \[Flujo de inicialización](#-flujo-de-inicialización)  

\- \[Flujo de render (por-frame)](#-flujo-de-render-por-frame)  

\- \[Clases / API clave](#-clases--api-clave-ejemplos)  

\- \[Requisitos / Cómo ejecutar](#-requisitos--cómo-ejecutar)  

\- \[Limitaciones y trabajo futuro](#-limitaciones-y-trabajo-futuro)  

\- \[Créditos / Referencias](#-créditos--referencias)  



---



\## 🎯 Objetivos del proyecto  

| Objetivo | Descripción |

|---|---|

| Arquitectura modular | Separar responsabilidades (gestión de ventana, dispositivo gráfico, recursos, escena, presentación) |

| Comprensión de pipeline | Implementar paso a paso la creación de recursos, configuración del pipeline y ciclo de render |

| Render mínimo funcional | Mostrar un objeto (por ejemplo, un cubo texturizado) actualizándose por frame |

| Extensibilidad | Proveer funciones `update()` / `render()` o puntos de extensión para agregar más características |

| Gestión de recursos | Manejo de buffers, texturas, shaders y limpieza adecuada |



---



\## 🏗 Arquitectura general



> Diseño modular que favorece la mantenibilidad, escalabilidad y extensión futura.



\### Componentes principales



| Componente | Responsabilidad | API / recursos clave |

|---|---|---|

| \*\*Window / Ventana\*\* | Crear y manejar la ventana de la aplicación (resolución, eventos) | `init(...)`, `destroy()`, manejo de callbacks de ventana |

| \*\*GraphicsDevice / Contexto\*\* | Inicializar el medio gráfico (OpenGL, Vulkan, etc.), gestionar comandos | creación de buffers, estados gráﬁcos, envío de draw calls |

| \*\*Swap Chain / Presentación\*\* | Intercambiar buffers frente / posterior para mostrar el frame | `present()`, configurar doble/triple búfer |

| \*\*Shader Manager\*\* | Cargar, compilar y gestionar programas de shaders (vertex, fragment, etc.) | `loadShader()`, `useShader()`, recompilación |

| \*\*Mesh / Geometry\*\* | Representar geometría 3D con vértices, índices, normales, UVs | `VertexBuffer`, `IndexBuffer`, `Draw()` |

| \*\*Material / Textura\*\* | Asociar propiedades visuales (texturas, colores, mapas normales) | binding de texturas, parámetros uniformes |

| \*\*Camera\*\* | Coordenadas de vista y proyección | matrices view / proj, actualización de posición |

| \*\*Light / Sistema de iluminación\*\* | Definir fuentes de luz, parámetros (color, posición, intensidad) | cálculo de iluminación en shader |

| \*\*Renderer / Pipeline\*\* | Orquestar el flujo de renderizado usando los componentes anteriores | `render(scene)`, administración de estados gráficos |

| \*\*Scene / Escena\*\* | Mantener los objetos, luces y cámara de la escena | `addObject()`, `update()`, `getRenderableObjects()` |



\### Relaciones operativas



El flujo típico entre componentes es:



1\. `Window` inicializa la ventana del sistema.  

2\. `GraphicsDevice` crea el contexto/gráficos.  

3\. `SwapChain` se conecta al `GraphicsDevice` y `Window` para la presentación.  

4\. `Renderer` consulta la `Scene` para obtener objetos visibles.  

5\. Por cada objeto, el `Renderer` selecciona el `Mesh`, el `Material`, obtiene el `Shader` desde `Shader Manager`, y envía draw calls al `GraphicsDevice`.  

6\. Al final del frame, se llama a `SwapChain.present()` para mostrar el resultado en pantalla.



---



\## 📷 Pipeline gráfico implementado



Este es el pipeline gráfico básico que MonacoEngine debe implementar:



1\. \*\*Inicialización gráfica\*\*  

&nbsp;  - Crear contexto / dispositivo gráfico  

&nbsp;  - Crear y compilar shaders  

&nbsp;  - Crear buffers (vertex, index, constantes)  

&nbsp;  - Configurar estado gráfico (viewport, rasterizador, depth test, blending)



2\. \*\*Transformaciones / Etapa de vértice\*\*  

&nbsp;  - Aplicar transformaciones: `model → world → view → projection`  

&nbsp;  - Realizar frustum culling o backface culling si aplica  

&nbsp;  - Enviar posiciones, normales, UVs a la etapa de fragmento



3\. \*\*Rasterización / fragmentación\*\*  

&nbsp;  - Rasterizar primitivas (triángulos)  

&nbsp;  - Interpolar atributos (normales, UVs) por fragmento



4\. \*\*Shading / iluminación\*\*  

&nbsp;  - Ejecutar shader fragmento para calcular color: ambient + difusa + especular  

&nbsp;  - Muestreo de texturas, mapas normales u otros mapas



5\. \*\*Depth / Blending / Salida\*\*  

&nbsp;  - Prueba de profundidad (z-buffer)  

&nbsp;  - Blending si hay transparencia  

&nbsp;  - Escritura al frame buffer



6\. \*\*Presentación / Swap\*\*  

&nbsp;  - Intercambiar los buffers para mostrar la imagen final



---



\## 🚀 Flujo de inicialización



1\. `Window.init(...)` → crea la ventana del sistema  

2\. `GraphicsDevice.init()` → inicializa el contexto gráfico  

3\. `ShaderManager.loadShaders(...)` → compila los shaders iniciales  

4\. `Mesh` / `Material` se crean / cargan, buffers asignados  

5\. Estado gráfico configurado (viewport, pruebas de profundidad, etc.)  

6\. Bucle principal: `while(running) { update(); render(); }`



---



\## ⏱ Flujo de render (por frame)



1\. Limpiar buffers (color, profundidad)  

2\. Preparar la lista de objetos visibles / frustum culling  

3\. Para cada objeto:  

&nbsp;  - Actualizar constantes (matrices, parámetros del material)  

&nbsp;  - Bind de shader / texturas / recursos  

&nbsp;  - Establecer buffers de vértices / índices  

&nbsp;  - Llamada de dibujo (draw)  

4\. Ejecutar efectos post-proceso si existen  

5\. Presentar / swap buffers  



---



\## 📚 Clases / API clave (ejemplos)



Aquí algunos métodos importantes que podrían existir:



\- `Window::init(hInst, width, height)`  

\- `GraphicsDevice::createBuffer(...)`, `createTexture(...)`, `setRenderState(...)`  

\- `ShaderManager::loadShader(name, vsPath, fsPath)`  

\- `Mesh::setVertices(...)`, `Mesh::draw()`  

\- `Material::bind()`  

\- `Renderer::render(Scene \&scene)`  

\- `Camera::getViewProjMatrix()`  

\- `Light::parameters()`  



---

