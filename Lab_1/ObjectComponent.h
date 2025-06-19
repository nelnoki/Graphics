#pragma once

#include <string>
#include "BaseComponent.h"
#include "../vcpkg/packages/assimp_x64-windows/include/assimp/postprocess.h"
#include "../vcpkg/packages/assimp_x64-windows/include/assimp/scene.h"
#include "../vcpkg/packages/assimp_x64-windows/include/assimp/Importer.hpp"




class ObjectComponent : public BaseComponent
{
protected:
    const std::string fNameModel;
    //void ProcessNode(aiNode* node, const aiScene* scene);
    //void ProcessMesh(aiMesh* mesh, const aiScene* scene);
public:
    bool isPickedUp = false;
    ObjectComponent(Game* g, std::string fileNameModel, const wchar_t* fileNameTexture);
    ~ObjectComponent() override = default;
    void Initialize() override;
    //void SetPosition(DirectX::SimpleMath::Vector3 p);
    //void SetRotation(DirectX::SimpleMath::Quaternion q);
    //void SetScale(DirectX::SimpleMath::Vector3 s);
};
