#define MESHOBJECT_IMPLEMENTATION

#include "../../../headers/engine/scenes/Scene.hpp"
#include "../../../headers/objects/asteroids/Pietrone.hpp"

// register to asteroid factory
#define ASTEROIDFACTORY_IMPLEMENTATION
#include "../../../headers/objects/asteroids/AsteroidFactory.hpp"
static const bool PietroneRegistered =
    (AsteroidFactory::registerAsteroid<Pietrone>(), true);

void Pietrone::Instantiate() {
    AbstractAsteroidObject::Instantiate();

    model.setShader("shaders/asteroids/BaseAsteroid_Vert.spv",
                    "shaders/asteroids/Pietrone_Frag.spv");
    model.addTexture("textures/asteroids/spaceRock/Albedo.png");
    model.addTexture("textures/asteroids/spaceRock/NormalMap.png");
    const std::vector<std::string> textures = {
        "textures/sky/right.png", "textures/sky/left.png",
        "textures/sky/top.png",   "textures/sky/bottom.png",
        "textures/sky/back.png",  "textures/sky/front.png"};
    model.addCubicTexture(textures);
}

std::string Pietrone::getModelName() { return "models/Pietrone.gltf"; }