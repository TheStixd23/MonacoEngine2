# \# 🎮 MonacoEngine2

# 

# !\[C++](https://img.shields.io/badge/C%2B%2B-DirectX11-blue?logo=cplusplus)

# !\[Status](https://img.shields.io/badge/status-in%20development-yellow)

# !\[License](https://img.shields.io/badge/license-MIT-green)

# !\[Platform](https://img.shields.io/badge/platform-Windows-lightgrey)

# 

# ---

# 

# \## 🧩 Descripción general

# 

# \*\*MonacoEngine2\*\* es la evolución del proyecto \*\*MonacoEngine\*\*, desarrollada en \*\*C+++\*\* sobre \*\*DirectX 11\*\*, con mejoras clave para la carga de modelos OBJ y texturas DDS.  

# Esta versión integra un cargador de modelos 3D (`ModelLoader`) que admite archivos OBJ y un sistema de texturas basado en archivos DDS, permitiendo escenas más ricas y configuraciones reales de motor gráfico.

# 

# Fue creado como proyecto académico para la asignatura \_Gráficas Computacionales 3D (Generación 2026-01)\_ en la Universidad Cuauhtémoc de Querétaro.

# 

# ---

# 

# \## 🚀 Novedades en esta versión

# 

# \- 📦 Soporte para cargar modelos 3D en formato \*\*.OBJ\*\*.  

# \- 🖼️ Soporte para texturas en formato \*\*.DDS\*\* (DirectDraw Surface) para mejor compatibilidad y rendimiento.  

# \- ✅ Log de inicialización y errores mejorado (usando macros `MESSAGE`, `ERROR`).  

# \- 🧱 Integración del pipeline completo: malla, textura, sampler, shader, constante‐buffers.  

# \- 🔄 Arquitectura modular, lista para extensión hacia iluminación, post‐procesado, etc.

# 

# ---

# 

# \## ⚙️ Características principales

# 

# \- Arquitectura modular con clases dedicadas: ventana (`Window`), dispositivo (`Device`), contexto (`DeviceContext`), swap chain (`SwapChain`), texturas (`Texture`), shaders (`ShaderProgram`), mallas (`MeshComponent`), cargador de modelos (`ModelLoader`).  

# \- Ciclo completo de renderizado en tiempo real: \*init → update → render → destroy\*.  

# \- Carga dinámica de modelos e integración de buffers de vértices e índices desde mallas OBJ.  

# \- Texturas cargadas en formato DDS, aplicadas mediante `Texture` y `SamplerState`.  

# \- Base sólida para iniciar proyectos de juegos o simulaciones gráficas en Windows.

# 

# ---

# 

# \## 📁 Estructura del repositorio

# 

# MonacoEngine2/

# │

# ├── include/ # Cabeceras (.h) del motor

# ├── src/ # Código fuente (.cpp) del motor

# │ ├── Core/ # Lógica interna del motor

# │ ├── Graphics/ # Grapics pipeline (Device, Context, SwapChain, Viewport...)

# │ ├── Resources/ # Texturas, modelos, shaders

# │ └── main.cpp # Punto de entrada

# ├── assets/ # Modelos (.obj), texturas (.dds/.png), shaders (.fx)

# ├── MonacoEngine2.sln # Solución de Visual Studio

# └── README.md

# 

# yaml

# Copiar código

# 

# ---

# 

# \## 🛠 Cómo compilar y ejecutar

# 

# 1\. \*\*Clonar el repositorio\*\*  

# &nbsp;  ```bash

# &nbsp;  git clone https://github.com/TheStixd23/MonacoEngine2.git

# Abrir la solución

# 

# Abre MonacoEngine2.sln con Visual Studio 2022 (o compatible).

# 

# Asegúrate de tener instalado el Windows SDK con soporte DirectX 11.

# 

# Construir y ejecutar

# 

# Selecciona configuración Debug o Release.

# 

# Compila el proyecto y ejecuta. Aparecerá una ventana de renderizado con tu modelo OBJ cargado y textura DDS aplicada.

# 

# 🧠 Uso básico

# Reemplaza el archivo modelo: cambia espada.obj en la carpeta assets/ o indica otro modelo en BaseApp.

# 

# Sustituye la textura DDS: coloca tu .dds en assets/ y ajusta el nombre en Texture::init(..).

# 

# Extiende el motor: agrega iluminación, sistema de cámara, sombras, partículas, o editor visual.

# 

# 🗺️ Roadmap

# &nbsp;Sistema de cámara 3D con control de movimiento y perspectiva.

# 

# &nbsp;Iluminación Phong / PBR y sombras en tiempo real.

# 

# &nbsp;Post-procesado: HDR, Bloom, Tone Mapping.

# 

# &nbsp;Interfaz editor en tiempo real para manipular la escena.

# 

# &nbsp;Exportación multiplataforma más allá de Windows.

# 

# 👤 Autor

# Hannin Steve Abarca Jacinto

# Estudiante de Ingeniería en Desarrollo de Videojuegos

# 📍 Universidad Cuauhtémoc de Querétaro

# 📧 hannin11.guerrero@gmail.com

# 🔗 LinkedIn

# 

# 🧾 Licencia

# Este proyecto está bajo la licencia MIT.

# Copia, modificación o distribución permitidas con atribución al autor.

# 

# 💬 Agradecimientos

# Desarrollado como proyecto académico para la asignatura Gráficas Computacionales 3D (2026-01).

# Gracias al profesorado y compañeros por su apoyo técnico y crítico durante el desarrollo del motor.

