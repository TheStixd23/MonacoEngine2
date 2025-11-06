/**
 * @file SamplerState.h
 * @brief Define la clase SamplerState para la configuración de muestreo en DirectX 11.
 *
 * La clase administra un estado de muestreo (ID3D11SamplerState), utilizado para controlar
 * cómo se aplican las texturas durante el renderizado. Incluye métodos para inicializar,
 * actualizar, aplicar y liberar el recurso.
 * 
 * @author Hannin Abarca
 */
#pragma once
#include "Prerequisites.h"
#include "Device.h"
#include "DeviceContext.h"

class Device;
class DeviceContext;

class SamplerState {
public:
    SamplerState() = default;
    ~SamplerState() = default;

    HRESULT init(Device& device);
    void update();
    void render(DeviceContext& deviceContext, unsigned int StartSlot, unsigned int NumSamplers);
    void destroy();

public:
    ID3D11SamplerState* m_sampler = nullptr;
};
