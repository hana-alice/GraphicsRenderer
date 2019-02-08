#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    :m_vertices(vertices),m_indices(indices),m_textures(textures)
{
    setupMesh();
}

void Mesh::setupMesh()
{
    
}