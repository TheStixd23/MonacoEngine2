#pragma once
/**
 * @file SamplerState.h
 * @brief Definición de la clase SamplerState.
 *
 * Encapsula un estado de muestreo (Sampler State) de Direct3D 11, que define
 * cómo se comportan las operaciones de lectura de texturas (filtrado, modo de dirección, etc.).
 *
 * @author Hannin Abarca
 */

#include "Prerequisites.h"

class Device;
class DeviceContext;

/**
 * @class SamplerState
 * @brief Encapsula un @c ID3D11SamplerState para la etapa de muestreo de texturas en Direct3D 11.
 *
 * Un Sampler State define cómo se leen las texturas en los shaders. Controla aspectos como:
 * - Filtrado: Point, Linear, Anisotropic (suavizado al acercarse o alejarse).
 * - Direccionamiento: Wrap (repetir), Mirror (espejo), Clamp (estirar borde), Border (color fijo).
 * - Nivel de detalle (LOD): Mipmapping.
 *
 * @note Esta clase administra la creación, aplicación y destrucción de un único @c ID3D11SamplerState.
 */
class SamplerState {
public:
    /**
     * @brief Constructor por defecto.
     */
    SamplerState() = default;

    /**
     * @brief Destructor por defecto.
     * @details No libera automáticamente el recurso COM; llamar a destroy().
     */
    ~SamplerState() = default;

    /**
     * @brief Inicializa el Sampler State con una configuración predeterminada.
     *
     * Crea un @c ID3D11SamplerState configurado comúnmente para gráficos 3D estándar:
     * - Filtro: Min/Mag/Mip Linear (suavizado trilineal).
     * - Dirección: Wrap (la textura se repite).
     * - LOD: Rango completo (0 a FLT_MAX).
     *
     * @param device Referencia al dispositivo con el que se creará el recurso.
     * @return HRESULT @c S_OK si fue exitoso; código de error en caso contrario.
     *
     * @post Si retorna @c S_OK, @c m_sampler != nullptr.
     */
    HRESULT init(Device& device);

    /**
     * @brief Actualiza parámetros internos del Sampler.
     *
     * Método de marcador reservado para lógica futura (ej. cambiar dinámicamente entre Linear y Point).
     * @note Actualmente no realiza ninguna operación.
     */
    void update();

    /**
     * @brief Asigna el Sampler State a la etapa de Pixel Shader.
     *
     * Llama a @c ID3D11DeviceContext::PSSetSamplers para activar este sampler en un slot específico.
     *
     * @param deviceContext Contexto donde se aplicará el sampler.
     * @param StartSlot     Slot inicial en el que se vinculará el sampler (0-15).
     * @param NumSamplers   Número de samplers a enlazar (normalmente 1).
     *
     * @pre @c m_sampler debe haberse creado con init().
     */
    void render(DeviceContext& deviceContext,
        unsigned int StartSlot,
        unsigned int NumSamplers);

    /**
     * @brief Libera el recurso @c ID3D11SamplerState.
     *
     * Utiliza SAFE_RELEASE para limpiar la memoria y resetea el puntero.
     */
    void destroy();

public:
    /**
     * @brief Puntero nativo al recurso de estado de muestreo de Direct3D 11.
     */
    ID3D11SamplerState* m_sampler = nullptr;
};