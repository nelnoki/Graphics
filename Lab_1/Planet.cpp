#include "Planet.h"

void Planet::Update() {

    /*rotationAngle += rotationSpeed * game->DeltaTime;
    orbitAngle += orbitSpeed * game->DeltaTime;

    rotationAngle = fmodf(rotationAngle, DirectX::XM_2PI);
    orbitAngle = fmodf(orbitAngle, DirectX::XM_2PI);

    pos.x = orbitRadius * cos(orbitAngle);
    pos.z = orbitRadius * sin(orbitAngle);

    mesh->worldMatrix =
        Matrix::CreateTranslation(pos) *
        Matrix::CreateRotationY(rotationAngle);*/

    pos = XMVector4Transform(
        pos,
        Matrix::CreateFromAxisAngle(
            Vector3::Up,
            game->DeltaTime * orbitSpeed));
    rotation *= Quaternion::CreateFromAxisAngle(
        Vector3::Up,
        rotationSpeed * game->DeltaTime);

    mesh->worldMatrix =
        Matrix::CreateFromQuaternion(rotation) *
        Matrix::CreateTranslation(pos);
}

void Moon::Update() {

    /*rotationAngle += rotationSpeed * game->DeltaTime;
    rotationAngle = fmodf(rotationAngle, DirectX::XM_2PI);

    orbitAngle += orbitSpeed * game->DeltaTime;
    orbitAngle = fmodf(orbitAngle, DirectX::XM_2PI);

    pos.x = orbitRadius * cos(orbitAngle);
    pos.z = orbitRadius * sin(orbitAngle);

    mesh->worldMatrix =
        Matrix::CreateRotationY(rotationAngle) *
        Matrix::CreateTranslation(pos) *
        parentPlanet->mesh->worldMatrix;*/

    relativePos = XMVector4Transform(
        relativePos,
        Matrix::CreateFromAxisAngle(
            Vector3::Up, 
            game->DeltaTime * orbitSpeed));
    pos = parentPlanet->pos + relativePos;
    rotation *= Quaternion::CreateFromAxisAngle(
        Vector3::Up,
        rotationSpeed * game->DeltaTime);
    mesh->worldMatrix =
        Matrix::CreateFromQuaternion(rotation) *
        Matrix::CreateTranslation(pos);
}