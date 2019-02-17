#pragma once

#include "mesh.h"
#include " "

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
    void processMode();
    

}