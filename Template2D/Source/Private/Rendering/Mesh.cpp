#include "Rendering/Mesh.h"
#include "Core/Engine.h"
#include <glad/glad.h>

UMesh* UMesh::CreateQuad()
{
	UMesh* NewMesh = UEngine::Get()->NewObject<UMesh>();

	std::vector<glm::vec3> Vertices = {
		glm::vec3(-0.5, -0.5, 0.0), // bottom left corner
		glm::vec3(-0.5, 0.5, 0.0), // top left corner
		glm::vec3(0.5, 0.5, 0.0), // top right corner
		glm::vec3(0.5, -0.5, 0.0) // bottom right corner
	};

	std::vector<glm::vec2> UVs = {
		glm::vec2(0, 1), // bottom left corner
		glm::vec2(0, 0), // top left corner
		glm::vec2(1, 0), // top right corner
		glm::vec2(1, 1) // bottom right corner
	};

	std::vector<unsigned int> Indices = {
		0,1,2, // first triangle (bottom left - top left - top right)
		0,2,3 // second triangle (bottom left - top right - bottom right)
	};

	FMeshData* RenderData = NewMesh->GetRenderData();
	RenderData->SetPositions(Vertices);
	RenderData->SetUVs(UVs);
	RenderData->AddSection(Indices, nullptr);
	NewMesh->Build();

	return NewMesh;
}

UMesh::FMeshData::FMeshData()
{
	Positions = nullptr;
	Normals = nullptr;
	Tangents = nullptr;
	Bitangents = nullptr;
	UVs = nullptr;
	Colors = nullptr;
	Indices = nullptr;
	Sections = nullptr;

	NumPositions = 0;
	NumUVs = 0;
	NumNormals = 0;
	NumTangents = 0;
	NumBitangents = 0;
	NumColors = 0;
	NumIndices = 0;
	NumSections = 0;
}

UMesh::FMeshData::~FMeshData()
{
	for (int i = 0; i < NumSections; ++i)
	{
		delete Sections[i];
	}

	delete[] Positions;
	delete[] Normals;
	delete[] Tangents;
	delete[] Bitangents;
	delete[] UVs;
	delete[] Colors;
	delete[] Indices;
	delete[] Sections;

	NumPositions = 0;
	NumUVs = 0;
	NumNormals = 0;
	NumTangents = 0;
	NumBitangents = 0;
	NumColors = 0;
	NumIndices = 0;
	NumSections = 0;
}

void UMesh::FMeshData::SetPositions(const std::vector<glm::vec3>& InPositions)
{
	delete[] Positions;
	Positions = new glm::vec3[InPositions.size()];
	memcpy(Positions, InPositions.data(), sizeof(glm::vec3) * InPositions.size());
	NumPositions = InPositions.size();
}

void UMesh::FMeshData::SetNormals(const std::vector<glm::vec3>& InNormals)
{
	delete[] Normals;
	Normals = new glm::vec3[InNormals.size()];
	memcpy(Positions, InNormals.data(), sizeof(glm::vec3) * InNormals.size());
	NumNormals = InNormals.size();
}

void UMesh::FMeshData::SetTangents(const std::vector<glm::vec3>& InTangents)
{
	delete[] Tangents;
	Tangents = new glm::vec3[InTangents.size()];
	memcpy(Positions, InTangents.data(), sizeof(glm::vec3) * InTangents.size());
	NumTangents = InTangents.size();
}

void UMesh::FMeshData::SetBitangents(const std::vector<glm::vec3>& InBitangents)
{
	delete[] Bitangents;
	Bitangents = new glm::vec3[InBitangents.size()];
	memcpy(Bitangents, InBitangents.data(), sizeof(glm::vec3) * InBitangents.size());
	NumBitangents = InBitangents.size();
}

void UMesh::FMeshData::SetUVs(const std::vector<glm::vec2>& InUVs)
{
	delete[] UVs;
	UVs = new glm::vec2[InUVs.size()];
	memcpy(UVs, InUVs.data(), sizeof(glm::vec2) * InUVs.size());
	NumUVs = InUVs.size();
}

void UMesh::FMeshData::SetColors(const std::vector<glm::vec4>& InColors)
{
	delete[] Colors;
	Colors = new glm::vec4[InColors.size()];
	memcpy(Colors, InColors.data(), sizeof(glm::vec4) * InColors.size());
	NumUVs = InColors.size();
}

const unsigned int* UMesh::FMeshData::GetIndices() const
{
	return Indices;
}

unsigned int UMesh::FMeshData::AddSection(const std::vector<unsigned int>& InIndices, UMaterial* InMaterial)
{
	unsigned int* NewIndices = new unsigned int[NumIndices + InIndices.size()];
	memcpy(NewIndices, Indices, sizeof(unsigned int) * NumIndices);
	memcpy((NewIndices + NumIndices), InIndices.data(), sizeof(unsigned int) * InIndices.size());

	delete[] Indices;
	Indices = NewIndices;

	FMeshSection** NewSections = new FMeshSection*[NumSections + 1];
	memcpy(NewSections, Sections, sizeof(FMeshSection) * NumSections);

	FMeshSection* NewMeshSection = new FMeshSection();
	NewMeshSection->Mat = InMaterial;
	NewMeshSection->IndexOffset = NumIndices;
	NewMeshSection->NumIndices = InIndices.size();
	NewSections[NumSections] = NewMeshSection;

	delete[] Sections;
	Sections = NewSections;

	NumIndices += InIndices.size();
	NumSections += 1;

	return NumSections - 1;
}

void UMesh::FMeshData::RemoveSection(unsigned int SectionIndex)
{
	FMeshSection* Section = Sections[SectionIndex];
	unsigned int StartIndex = Section->IndexOffset;
	unsigned int EndIndices = Section->IndexOffset + Section->NumIndices;
	unsigned int SectionNumIndices = Section->NumIndices;
	unsigned int NewNumIndices = NumIndices - SectionNumIndices;

	// Remove the Indices by not copying it
	unsigned int* NewIndices = new unsigned int[NewNumIndices];
	memcpy(NewIndices, Indices, sizeof(unsigned int) * StartIndex);
	memcpy((NewIndices + StartIndex), (Indices + EndIndices), sizeof(unsigned int) * (NumIndices - EndIndices));
	NumIndices = NewNumIndices;

	delete[] Indices;
	Indices = NewIndices;

	for (unsigned int i = SectionIndex; i < NumSections - 1; ++i)
	{
		Sections[i]->IndexOffset -= SectionNumIndices;
	}

	// Remove the mesh section by not copying it
	FMeshSection** NewSections = new FMeshSection*[NumSections - 1];
	memcpy(NewSections, Sections, sizeof(FMeshSection) * SectionIndex);
	memcpy((NewSections + SectionIndex), (Sections + SectionIndex + 1), sizeof(FMeshSection) * (NumSections - (SectionIndex + 1)));

	delete[] Sections;
	Sections = NewSections;

	NumSections--;
}

unsigned int UMesh::FMeshData::GetNumSections() const
{
	return NumSections;
}

const UMesh::FMeshSection* UMesh::FMeshData::GetMeshSection(unsigned int SectionIndex) const
{
	return Sections[SectionIndex];
}

void UMesh::Begin()
{
	glGenBuffers(1, &IndexBuffer);
	glGenBuffers(1, &VertexBuffer);
	glGenVertexArrays(1, &VertexArrayObject);

	RenderData = new FMeshData();

	PositionsBufferSizeBytes = 0;
	UVsBufferSizeBytes = 0;
	NormalsBufferSizeBytes = 0;
	TangentsBufferSizeBytes = 0;
	BitangentsBufferSizeBytes = 0;
	ColorsBufferSizeBytes = 0;
	VertexBufferSizeBytes = 0;

	PositionsOffsetBytes = 0;
	UVsOffsetBytes = 0;
	NormalsOffsetBytes = 0;
	TangentsOffsetBytes = 0;
	BitangentsOffsetBytes = 0;
	ColorsOffsetBytes = 0;
}

void UMesh::End()
{
	glDeleteBuffers(1, &IndexBuffer);
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteVertexArrays(1, &VertexArrayObject);

	delete RenderData;
}

void UMesh::Build()
{
	PositionsBufferSizeBytes = (sizeof(glm::vec3) * RenderData->NumPositions);
	UVsBufferSizeBytes = (sizeof(glm::vec2) * RenderData->NumUVs);
	NormalsBufferSizeBytes = (sizeof(glm::vec3) * RenderData->NumNormals);
	TangentsBufferSizeBytes = (sizeof(glm::vec3) * RenderData->NumTangents);
	BitangentsBufferSizeBytes = (sizeof(glm::vec3) * RenderData->NumBitangents);
	ColorsBufferSizeBytes = (sizeof(glm::vec4) * RenderData->NumColors);

	VertexBufferSizeBytes =
		PositionsBufferSizeBytes + UVsBufferSizeBytes + NormalsBufferSizeBytes +
		TangentsBufferSizeBytes + BitangentsBufferSizeBytes + ColorsBufferSizeBytes;

	PositionsOffsetBytes = 0;
	UVsOffsetBytes = PositionsOffsetBytes + PositionsBufferSizeBytes;
	NormalsOffsetBytes = UVsOffsetBytes + UVsBufferSizeBytes;
	TangentsOffsetBytes = NormalsOffsetBytes + NormalsBufferSizeBytes;
	BitangentsOffsetBytes = TangentsOffsetBytes + TangentsBufferSizeBytes;
	ColorsOffsetBytes = BitangentsOffsetBytes + BitangentsBufferSizeBytes;

	glBindVertexArray(VertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSizeBytes, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, PositionsOffsetBytes, PositionsBufferSizeBytes, RenderData->Positions);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)PositionsOffsetBytes);

	glBufferSubData(GL_ARRAY_BUFFER, UVsOffsetBytes, UVsBufferSizeBytes, RenderData->UVs);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)UVsOffsetBytes);

	glBufferSubData(GL_ARRAY_BUFFER, NormalsOffsetBytes, NormalsBufferSizeBytes, RenderData->Normals);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (GLvoid*)NormalsOffsetBytes);

	glBufferSubData(GL_ARRAY_BUFFER, TangentsOffsetBytes, TangentsBufferSizeBytes, RenderData->Tangents);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 0, (GLvoid*)TangentsOffsetBytes);

	glBufferSubData(GL_ARRAY_BUFFER, BitangentsOffsetBytes, BitangentsBufferSizeBytes, RenderData->Bitangents);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, 0, (GLvoid*)BitangentsOffsetBytes);

	glBufferSubData(GL_ARRAY_BUFFER, ColorsOffsetBytes, ColorsBufferSizeBytes, RenderData->Colors);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)ColorsOffsetBytes);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RenderData->NumIndices * sizeof(unsigned int), RenderData->Indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

UMesh::FMeshData* UMesh::GetRenderData() const
{
	return RenderData;
}

unsigned int UMesh::GetVAO() const
{
	return VertexArrayObject;
}
