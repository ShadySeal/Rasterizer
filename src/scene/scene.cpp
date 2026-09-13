#include "scene/scene.h"
#include "rendering/renderer.h"

rasterizer::scene::Scene::Scene()
{
    rasterizer::rendering::Renderer renderer(*this, 800, 800, 1);

    std::vector<Vector3> vertices = {
        { 1,  1,  1},
        {-1,  1,  1},
        {-1, -1,  1},
        { 1, -1,  1},
        { 1,  1, -1},
        {-1,  1, -1},
        {-1, -1, -1},
        { 1, -1, -1}
    };

    std::vector<Triangle> triangles = {
        {0, 1, 2, Color(Color::RED)},
        {0, 2, 3, Color(Color::RED)},
        {4, 0, 3, Color(Color::GREEN)},
        {4, 3, 7, Color(Color::GREEN)},
        {5, 4, 7, Color(Color::BLUE)},
        {5, 7, 6, Color(Color::BLUE)},
        {1, 5, 6, Color(Color::YELLOW)},
        {1, 6, 2, Color(Color::YELLOW)},
        {4, 5, 1, Color(Color::MAGENTA)},
        {4, 1, 0, Color(Color::MAGENTA)},
        {2, 6, 7, Color(Color::CYAN)},
        {2, 7, 3, Color(Color::CYAN)}
    };

    Model cube("Cube", vertices, triangles);

    Instance cube1(cube, Vector3(-1.5, 0, 7));
    Instance cube2(cube, Vector3(1.25, 2, 7.5));

    instances = {cube1, cube2};

    renderer.render();
}

rasterizer::scene::Scene::~Scene() {}
