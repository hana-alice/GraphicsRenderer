#pragma once

#ifndef GR_MESH_H
#define GR_MESH_H
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"

using namespace std;

struct Vertex
{
    glm::vec3 positon;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    string type;
};

class Mesh
{
public:
    vector<Vertex> m_vertices;
    vector<unsigned int > m_indices; 
    vector<Texture> m_textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void draw(Shader shader);

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_eao;
    void setupMesh();
};
#endif