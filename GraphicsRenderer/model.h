#pragma once
#include "mesh.h"
#include "assimp/Improter.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{
    public:
        Model(char* path)
        {
            loadModel(path);
        }
        void Draw();

    private:
    vector<Mesh> m_vecMeshes;
    string m_directory;

    void loadModel(std::string path);
    void processNode();
    

}