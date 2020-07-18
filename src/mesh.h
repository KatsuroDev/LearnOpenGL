#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);
private:
    // render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

#endif
