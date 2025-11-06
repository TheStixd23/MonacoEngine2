Jajaja ok 😅, entiendo perfectamente — quieres algo más \*\*limpio, visual y profesional\*\*, como el del \*\*primer MonacoEngine\*\*, sin tanta decoración ni texto relleno.



Vamos a hacerlo \*\*idéntico en estilo\*\* al anterior, pero actualizado con lo nuevo (modelos `.obj` + texturas `.dds`).

Aquí tienes una versión \*\*más sobria, estructurada y “de desarrollador”\*\* 👇



---



```markdown

\# MonacoEngine2



Motor gráfico desarrollado en \*\*C++\*\* y \*\*DirectX 11\*\*, continuación directa del proyecto \*MonacoEngine\*.  

Esta versión añade soporte para \*\*modelos .OBJ\*\* y \*\*texturas .DDS\*\*, integrados en un pipeline modular optimizado para pruebas de renderizado 3D.



---



\## Características principales



\- Sistema de inicialización de DirectX 11 completo (Device, SwapChain, Context, Viewport).  

\- Carga y renderizado de modelos en formato \*\*.OBJ\*\*.  

\- Soporte para texturas \*\*.DDS\*\* con mapeo UV automático.  

\- Estructura modular por componentes (ventana, shaders, buffers, malla, textura).  

\- Macros de depuración (`MESSAGE`, `ERROR`) para trazabilidad en consola.  

\- Preparado para expansión hacia iluminación, cámara y materiales.



---



\## Estructura del proyecto



```



MonacoEngine2/

│

├── include/              # Cabeceras del motor

├── src/                  # Código fuente

│   ├── Core/             # Clases base (App, Window)

│   ├── Graphics/         # Pipeline gráfico (Device, Context, SwapChain, etc.)

│   ├── Resources/        # Cargadores de modelos, texturas y shaders

│   └── main.cpp          # Punto de entrada

├── assets/               # Modelos (.obj) y texturas (.dds)

└── MonacoEngine2.sln     # Solución de Visual Studio



````



---



\## Requisitos



\- \*\*Windows 10 / 11\*\*  

\- \*\*Visual Studio 2022\*\* con el SDK de \*\*DirectX 11\*\*  

\- \*\*C++17\*\* o superior  



---



\## Compilación y ejecución



1\. Clona el repositorio:

&nbsp;  ```bash

&nbsp;  git clone https://github.com/TheStixd23/MonacoEngine2.git

````



2\. Abre `MonacoEngine2.sln` en Visual Studio.

3\. Compila y ejecuta el proyecto.

4\. Se mostrará una ventana con el modelo y textura cargados.



---



\## Créditos



Proyecto desarrollado por \*\*Hannin Steve Abarca Jacinto\*\*

Universidad Cuauhtémoc de Querétaro – Ingeniería en Desarrollo de Videojuegos

📧 \[hannin11.guerrero@gmail.com](mailto:hannin11.guerrero@gmail.com)



---



\## Licencia



Este proyecto se distribuye bajo la licencia \*\*MIT\*\*.



```



---



¿Quieres que también te haga la \*\*versión en inglés\*\*, como el del repo anterior (para que quede más profesional en GitHub)?  

Puedo dejártelo igual de limpio pero con estilo “readme de motor gráfico open-source”.

```



