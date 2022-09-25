#pragma once
#include "Core/Object.h"
#include <unordered_map>
#include <glad/glad.h>

class UTexture : public UObject
{
public:
	enum class ETextureFormats
	{
		R8,
		RG8,
		RGB8,
		RGBA8,
		R16,
		RG16,
		RGB16,
		RGBA16,
		R32,
		RG32,
		RGB32,
		RGBA32,
		Depth
	};

	struct FTexFormatToGLType
	{
		FTexFormatToGLType() :
			SourceFormat(GL_RGBA8),
			DesiredFormat(GL_RGBA),
			DataType(GL_BYTE),
			Stride(32)
		{}

		FTexFormatToGLType(GLenum InSourceFormat, GLenum InDesiredFormat, GLenum InDataType, unsigned int InStride) :
			SourceFormat(InSourceFormat),
			DesiredFormat(InDesiredFormat),
			DataType(InDataType),
			Stride(InStride)
		{}

		GLenum SourceFormat;
		GLenum DesiredFormat;
		GLenum DataType;
		unsigned int Stride;
	};

	struct FTextureData
	{
	public:
		FTextureData();
		~FTextureData();

		unsigned char* Data;
		int Width;
		int Height;
		int Depth;
		bool bGenerateMips;
		ETextureFormats Format;
	};

protected:
	IMPLEMENT_CONSTRUCTOR(UTexture, UObject);

	virtual void Begin() override;
	virtual void End() override;

public:
	FTextureData* GetTextureData() const;
	unsigned int GetTextureID() const;

	virtual void Build() = 0;

protected:
	static std::unordered_map<ETextureFormats, FTexFormatToGLType> TexFormatsToGLTypes;

private:
	unsigned int TextureID;
	FTextureData* RenderData;
};

