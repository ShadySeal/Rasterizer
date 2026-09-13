#include "scene/scene.h"
#include "rendering/renderer.h"

rasterizer::scene::Scene::Scene()
{
    rasterizer::rendering::Renderer renderer(*this, 800, 800, 1);
    renderer.render();
}

rasterizer::scene::Scene::~Scene() {}
