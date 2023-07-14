#ifndef __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_UNIFORMS_HPP_
#define __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_UNIFORMS_HPP_

#include "../base/includes.hpp"
struct GlobalUniforms {
    alignas(16) glm::vec3 pointLightPosition;           // position of the point light
    alignas(16) glm::vec4 pointLightColor;              // color of the point light
    alignas(4) float pointLightDecayFactor;             // decay factor
    alignas(4) float pointLightTargetDistance;          // target distance
    alignas(16) glm::vec3 directionalLightDirection;    // direction of the directional light
    alignas(16) glm::vec4 directionalLightColor;        // color of the directional light
    alignas(16) glm::vec3 spotlightPosition;            // postion of the spotlight
    alignas(16) glm::vec3 spotlightDirection;           // direction of the spotlight/directional light
    alignas(16) glm::vec4 spotlightColor;               // color of the light
    alignas(4) float spotlightDecayFactor;              // decay factor
    alignas(4) float spotlightTargetDistance;           // target distance
    alignas(4) float spotlightCosIn;                    // cosine of the inner angle
    alignas(4) float spotlightCosOut;                   // cosine of the outer angle
    alignas(16) glm::vec3 eyePos;                       // position of the viewer (camera)
    //Each object will then encode the other light parameters either by linking a custom Uniform block or by supplying additional textures
    //Maybe include spherical harmonics coefficients here as well?
};

struct Uniforms {
    alignas(16) glm::mat4 mvpMat;
    alignas(16) glm::mat4 modelMat;
    alignas(16) glm::mat4 cameraMat;
};

#endif // __DESKTOP_POLIMI_PROJECTS_CG_ASTRARACCOONS_HEADERS_GRAPHICS_UNIFORMS_HPP_