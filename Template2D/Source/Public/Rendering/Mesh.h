#pragma once
#include "Core/Object.h"
#include "glm/glm.hpp"
#include <vector>

class UMaterial;

class UMesh : public UObject
{
public:
	struct FMeshSection
	{
		UMaterial* Mat;
		unsigned int IndexOffset;
		unsigned int NumIndices;
	};

	struct FMeshData
	{
	public:
		void SetPositions(const std::vector<glm::vec3>& InPositions);
		void SetNormals(const std::vector<glm::vec3>& InNormals);
		void SetTangents(const std::vector<glm::vec3>& InTangents);
		void SetBitangents(const std::vector<glm::vec3>& InBitangents);
		void SetUVs(const std::vector<glm::vec2>& InUVs);
		void SetColors(const std::vector<glm::vec4>& InColors);

		const unsigned int* GetIndices() const;

		unsigned int AddSection(const std::vector<unsigned int>& InIndices, UMaterial* InMaterial);
		void RemoveSection(unsigned int SectionIndex);
		unsigned int GetNumSections() const;
		const FMeshSection* GetMeshSection(unsigned int SectionIndex) const;

	private:
		FMeshData();
		~FMeshData();

		glm::vec3* Positions;
		glm::vec3* Normals;
		glm::vec3* Tangents;
		glm::vec3* Bitangents;
		glm::vec2* UVs;
		glm::vec4* Colors;
		unsigned int* Indices;
		FMeshSection** Sections;

		unsigned int NumPositions;
		unsigned int NumUVs;
		unsigned int NumNormals;
		unsigned int NumTangents;
		unsigned int NumBitangents;
		unsigned int NumColors;
		unsigned int NumIndices;
		unsigned int NumSections;

		friend class UMesh;
	};

public:
	static UMesh* CreateQuad();

protected:
	IMPLEMENT_CONSTRUCTOR(UMesh, UObject);

public:
	virtual void Begin() override;
	virtual void End() override;

	void Build();
	FMeshData* GetRenderData() const;

	unsigned int GetVAO() const;

private:
	unsigned int VertexBuffer;
	unsigned int IndexBuffer;
	unsigned int VertexArrayObject;

	unsigned int PositionsBufferSizeBytes;
	unsigned int UVsBufferSizeBytes;
	unsigned int NormalsBufferSizeBytes;
	unsigned int TangentsBufferSizeBytes;
	unsigned int BitangentsBufferSizeBytes;
	unsigned int ColorsBufferSizeBytes;
	unsigned int VertexBufferSizeBytes;

	unsigned int PositionsOffsetBytes;
	unsigned int UVsOffsetBytes;
	unsigned int NormalsOffsetBytes;
	unsigned int TangentsOffsetBytes;
	unsigned int BitangentsOffsetBytes;
	unsigned int ColorsOffsetBytes;

	FMeshData* RenderData;
};

