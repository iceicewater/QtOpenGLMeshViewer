#include "light.h"

Light::Light()
    : m_ambient(0.1f, 0.1f, 0.1f, 1.0f),
      m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
      m_specular(1.0f, 1.0f, 1.0f, 1.0f),
      m_lightType(LightType::Point),
      m_attenuationType(AttenuationType::Constant),
      m_innerCutOff(30.0f),
      m_outerCutOff(40.0f),
      m_enable(true)
{

}
