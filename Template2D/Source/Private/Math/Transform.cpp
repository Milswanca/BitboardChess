#include "Math/Transform.h"
#include <glm/ext/matrix_transform.hpp>

FTransform::FTransform()
{
    Position = glm::vec3(0.0f, 0.0f, .0f);
    Rotation = glm::identity<glm::quat>();
    Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    RelativeTransform = glm::identity<glm::mat4>();
    LocalToWorld = glm::identity<glm::mat4>();
    WorldToLocal = glm::identity<glm::mat4>();

    Parent = nullptr;
}

FTransform::~FTransform()
{

}

void FTransform::SetLocalPosition(const glm::vec3& LocalPosition)
{
    Position = LocalPosition;
    ApplyTransformation();
}

void FTransform::SetLocalRotation(const glm::quat& LocalRotation)
{
    Rotation = LocalRotation;
    ApplyTransformation();
}

void FTransform::SetLocalScale(const glm::vec3& LocalScale)
{
    Scale = LocalScale;
    ApplyTransformation();
}

void FTransform::TranslateLocal(const glm::vec3& Translation)
{
	Position += GetRotation() * Translation;
	ApplyTransformation();
}

void FTransform::TranslateWorld(const glm::vec3& Translation)
{
	Position += Translation;
    ApplyTransformation();
}

void FTransform::Rotate(float Degrees, const glm::vec3& Axis)
{
    Rotation = glm::angleAxis(glm::radians(Degrees), Axis) * Rotation;
    ApplyTransformation();
}

void FTransform::Rotate(const glm::quat& Quaternion)
{
	Rotation = Quaternion * Rotation;
	ApplyTransformation();
}

void FTransform::Rotate(const glm::vec3& Euler)
{
	Rotation = glm::quat(glm::vec3(glm::radians(Euler.x), glm::radians(Euler.y), glm::radians(Euler.z))) * Rotation;
	ApplyTransformation();
}

glm::vec3 FTransform::GetLocalPosition() const
{
    return Position;
}

glm::quat FTransform::GetLocalRotation() const
{
    return Rotation;
}

glm::vec3 FTransform::GetLocalScale() const
{
    return Scale;
}

glm::vec3 FTransform::GetPosition() const
{
    return LocalToWorld * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::quat FTransform::GetRotation() const
{
    return glm::toQuat(LocalToWorld);
}

glm::vec3 FTransform::GetForward() const
{
    return LocalToWorld * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
}

glm::vec3 FTransform::GetRight() const
{
    return LocalToWorld * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
}

glm::vec3 FTransform::GetUp() const
{
    return LocalToWorld * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

void FTransform::GetLocalToWorld(glm::mat4& OutLocalToWorld) const
{
    OutLocalToWorld = LocalToWorld;
}

void FTransform::GetWorldToLocal(glm::mat4& OutWorldToLocal) const
{
    OutWorldToLocal = WorldToLocal;
}

void FTransform::SetParent(FTransform* NewParent)
{
    if(Parent != nullptr)
    {
        Parent->RemoveChild(this);
    }

    Parent = NewParent;

    if(Parent)
    {
        Parent->AddChild(this);
    }

    ApplyTransformation();
}

FTransform* FTransform::GetParent() const
{
    return Parent;
}

void FTransform::ApplyTransformation()
{
    //Calc Matrices here
    glm::mat4 T = glm::translate(glm::identity<glm::mat4>(), Position);
    glm::mat4 R = glm::toMat4(Rotation);
    glm::mat4 S = glm::scale(glm::identity<glm::mat4>(), Scale);

    RelativeTransform = T * R * S;
    RecalculateSpaceTransforms();
}

void FTransform::RecalculateSpaceTransforms()
{
    LocalToWorld = Parent != nullptr ? Parent->LocalToWorld * RelativeTransform : RelativeTransform;
    WorldToLocal = glm::inverse(LocalToWorld);
    
    for(size_t i = 0; i < Children.size(); ++i)
    {
        Children[i]->ApplyTransformation();
    }
}

void FTransform::AddChild(FTransform* NewChild)
{
    Children.push_back(NewChild);
}

void FTransform::RemoveChild(FTransform* RemChild)
{
    remove(Children.begin(), Children.end(), RemChild);
}