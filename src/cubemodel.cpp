#include "cubemodel.h"

Model *cubeModel;
void createCubeModel(){
    cubeModel = new Model(8, 12);
    cubeModel->vertices.push_back({1, 1, 1});   // 0

    cubeModel->vertices.push_back({-1, 1, 1});  // 1

    cubeModel->vertices.push_back({-1, -1, 1}); // 2

    cubeModel->vertices.push_back({1, -1, 1});  // 3

    cubeModel->vertices.push_back({1, 1, -1});  // 4 

    cubeModel->vertices.push_back({-1, 1, -1}); // 5 

    cubeModel->vertices.push_back({-1, -1, -1});// 6 

    cubeModel->vertices.push_back({1, -1, -1}); // 7 

    // cubeModel->triangles.push_back( {0, 1, 2, {255, 0, 0, 255}} );

    // cubeModel->triangles.push_back( {0, 2, 3, {255, 0, 0, 255}} );

    // cubeModel->triangles.push_back( {4, 0, 3, {0, 255, 0, 255}} );

    // cubeModel->triangles.push_back( {4, 3, 7, {0, 255, 0, 255}} );

    cubeModel->triangles.push_back( {5, 4, 7, {0, 0, 255, 255}} );

    cubeModel->triangles.push_back( {5, 7, 6, {0, 0, 255, 255}} );

    // cubeModel->triangles.push_back( {1, 5, 6, {255, 255, 0, 255}} );

    // cubeModel->triangles.push_back( {1, 6, 2, {255, 255, 0, 255}} );

    // cubeModel->triangles.push_back( {4, 5, 1, {128, 0, 128, 255}} );

    // cubeModel->triangles.push_back( {4, 1, 0, {128, 0, 128, 255}} );

    // cubeModel->triangles.push_back( {2, 6, 7, {0, 255, 255, 255}} );

    // cubeModel->triangles.push_back( {2, 7, 3, {0, 255, 255, 255}} );

    // cubeModel->textureCoors.push_back({{1, 1}, {0, 1}, {0, 0}});
    // cubeModel->textureCoors.push_back({{1, 1}, {0, 0}, {1, 0}});
    // cubeModel->textureCoors.push_back({{0, 1}, {1, 1}, {1, 0}});
    // cubeModel->textureCoors.push_back({{0, 1}, {1, 0}, {0, 0}});
    cubeModel->textureCoors.push_back({{0, 0}, {1, 0}, {1, 1}});
    cubeModel->textureCoors.push_back({{0, 0}, {1, 1}, {0, 1}});
    // cubeModel->textureCoors.push_back({{0, 1}, {1, 1}, {1, 0}});
    // cubeModel->textureCoors.push_back({{0, 1}, {1, 0}, {0, 0}});
    // cubeModel->textureCoors.push_back({{1, 0}, {0, 0}, {0, 1}});
    // cubeModel->textureCoors.push_back({{1, 0}, {0, 1}, {1, 1}});
    // cubeModel->textureCoors.push_back({{0, 1}, {0, 0}, {1, 0}});
    // cubeModel->textureCoors.push_back({{0, 1}, {1, 0}, {1, 1}});

}

Instance cubeInstance;
void createCubeInstance(){
    createCubeModel();
    cubeInstance.model = cubeModel;
    cubeInstance.transform = {{0, 0, -2}, {0, 0, 0}, 1};
};