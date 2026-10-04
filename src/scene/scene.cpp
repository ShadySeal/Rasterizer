#include "scene/scene.h"
#include "rendering/renderer.h"
#include "math/transform.h"
#include "scene/camera.h"

using namespace rasterizer::scene;
using namespace rasterizer::rendering;
using namespace rasterizer::math;

Scene::Scene()
{
    camera = Camera(Vector3(0, 0, 0), Matrix4x4::identity());

    Renderer renderer(*this, 800, 800, 1);

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

    Model sphere = Model::makeSphere("Sphere", 1, 15, 15, Color(Color::GREEN));

    Instance cube1(cube, Vector3(-1.5, 0, 7), Transform(1.0f, 0.0f, Vector3(-1.5, 1.5, 7)));
    Instance sphere1(sphere, Vector3(1.25, 2, 7.5), Transform(1.0f, 0.0f, Vector3(1.25, -1, 7.5)));

    cube1.transform.rotation = 0.5f; // Rotate the first cube

    instances = {cube1, sphere1};

    Light ambient(Light::AMBIENT, 0.2);
    Light point(Light::POINT, 0.6, Vector3(2, 1, 0));
    Light directional(Light::DIRECTIONAL, 0.2, Vector3(0, 0, 0), Vector3(1, 4, 4));

    lights = {ambient, point, directional};

    renderer.render();
}

Scene::~Scene() {}
