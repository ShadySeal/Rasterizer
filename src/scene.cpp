#include "scene.h"
#include "renderer.h"

using namespace rasterizer;

Scene::Scene()
{
    Renderer renderer(*this, 800, 800, 1);
    renderer.render();
}

Scene::~Scene() {}
