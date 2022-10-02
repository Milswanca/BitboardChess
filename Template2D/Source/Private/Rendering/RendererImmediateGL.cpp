#include "Rendering/RendererImmediateGL.h"
#include "Rendering/Sprite.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture2D.h"
#include "Components/CameraComponent.h"

#include <glm/ext/matrix_transform.hpp>

UMesh* URendererImmediateGL::QuadMesh = nullptr;
UShader* URendererImmediateGL::SpriteDefaultShader = nullptr;
UMaterial* URendererImmediateGL::SpriteDefaultMaterial = nullptr;

void URendererImmediateGL::Begin()
{
	UObject::Begin();

	if (QuadMesh == nullptr)
		QuadMesh = UMesh::CreateQuad();

	if (SpriteDefaultShader == nullptr)
	{
		UShader::FCreateShaderParams Params;
		Params.VertexShader = "Engine/Shaders/SpriteVertex.shader";
		Params.FragmentShader = "Engine/Shaders/SpriteFragment.shader";
		SpriteDefaultShader = UShader::Create(Params);
	}

	if (SpriteDefaultMaterial == nullptr)
		SpriteDefaultMaterial = UMaterial::Create(SpriteDefaultShader);
}

void URendererImmediateGL::End()
{
	UObject::End();
}

void URendererImmediateGL::DrawSprite(USprite* Sprite, UMaterial* Material, const glm::vec4& Tint, const glm::mat4& Transform)
{
	if (Sprite != nullptr)
	{
		glm::mat4 PixelScale = Sprite->GetScaleMatrix();

		UMesh::FMeshData* RenderData = QuadMesh->GetRenderData();
		int NumSections = RenderData->GetNumSections();

		glBindVertexArray(QuadMesh->GetVAO());
		for (int i = 0; i < NumSections; ++i)
		{
			glm::mat4 ProjectionView = UCameraComponent::GetMain() != nullptr ? UCameraComponent::GetMain()->GetProjectionView() : glm::identity<glm::mat4>();

			UMaterial* Mat = Material == nullptr ? SpriteDefaultMaterial : Material;
			Mat->SetMat4("gProjectionView", ProjectionView);
			Mat->SetMat4("gModel", Transform * PixelScale);
			Mat->SetVector4("gTint", Tint);
			Mat->SetTexture("gSprite", Sprite != nullptr ? Sprite->GetTexture() : nullptr);
			Mat->Bind();

			const UMesh::FMeshSection* Section = RenderData->GetMeshSection(i);
			glDrawElements(GL_TRIANGLES, Section->NumIndices, GL_UNSIGNED_INT, reinterpret_cast<void*>(Section->IndexOffset));

			Mat->Unbind();
		}
		glBindVertexArray(0);
	}
}