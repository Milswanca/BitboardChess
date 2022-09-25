#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct FTransform
{
    FTransform();
    ~FTransform();
    
    void SetLocalPosition(const glm::vec3& LocalPosition);
    void SetLocalRotation(const glm::quat& LocalRotation);
    void SetLocalScale(const glm::vec3& LocalScale);

    void TranslateLocal(const glm::vec3& Translation);
    void TranslateWorld(const glm::vec3& Translation);

    void Rotate(float Degrees, const glm::vec3& Axis);
    void Rotate(const glm::quat& Quaternion);
    void Rotate(const glm::vec3& Euler);

    glm::vec3 GetLocalPosition() const;
    glm::quat GetLocalRotation() const;
    glm::vec3 GetLocalScale() const;

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;

    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;

    void GetLocalToWorld(glm::mat4& OutLocalToWorld) const;
    void GetWorldToLocal(glm::mat4& OutWorldToLocal) const;

    void SetParent(FTransform* NewParent);
    FTransform* GetParent() const;

private:
    void ApplyTransformation();
    void RecalculateSpaceTransforms();
    
    void AddChild(FTransform* NewChild);
    void RemoveChild(FTransform* RemChild);
    
    glm::mat4 LocalToWorld;
    glm::mat4 WorldToLocal;
    glm::mat4 RelativeTransform;

    glm::vec3 Position;
    glm::quat Rotation;
    glm::vec3 Scale;

    FTransform* Parent;
    std::vector<FTransform*> Children;
};