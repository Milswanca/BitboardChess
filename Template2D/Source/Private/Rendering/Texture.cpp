#include "Rendering/Texture.h"

std::unordered_map<UTexture::ETextureFormats, UTexture::FTexFormatToGLType> UTexture::TexFormatsToGLTypes
{
	{UTexture::ETextureFormats::RGBA8, UTexture::FTexFormatToGLType(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, sizeof(char) * 4)},
	{UTexture::ETextureFormats::RGBA16, UTexture::FTexFormatToGLType(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, sizeof(short) * 4)},
	{UTexture::ETextureFormats::RGBA32, UTexture::FTexFormatToGLType(GL_RGBA32F, GL_RGBA, GL_FLOAT, sizeof(float) * 4)},
	{UTexture::ETextureFormats::RGB8, UTexture::FTexFormatToGLType(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, sizeof(char) * 3)},
	{UTexture::ETextureFormats::RGB16, UTexture::FTexFormatToGLType(GL_RGB16F, GL_RGB, GL_HALF_FLOAT, sizeof(short) * 3)},
	{UTexture::ETextureFormats::RGB32, UTexture::FTexFormatToGLType(GL_RGB32F, GL_RGB, GL_FLOAT, sizeof(float) * 3)},
	{UTexture::ETextureFormats::RG8, UTexture::FTexFormatToGLType(GL_RG8, GL_RG, GL_UNSIGNED_BYTE, sizeof(char) * 2)},
	{UTexture::ETextureFormats::RG16, UTexture::FTexFormatToGLType(GL_RG16F, GL_RG, GL_HALF_FLOAT, sizeof(short) * 2)},
	{UTexture::ETextureFormats::RG32, UTexture::FTexFormatToGLType(GL_RG32F, GL_RG, GL_FLOAT, sizeof(float) * 2)},
	{UTexture::ETextureFormats::R8, UTexture::FTexFormatToGLType(GL_R8, GL_RED, GL_UNSIGNED_BYTE, sizeof(char) * 1)},
	{UTexture::ETextureFormats::R16, UTexture::FTexFormatToGLType(GL_R16F, GL_RED, GL_HALF_FLOAT, sizeof(short) * 1)},
	{UTexture::ETextureFormats::R32, UTexture::FTexFormatToGLType(GL_R32F, GL_RED, GL_FLOAT, sizeof(float) * 1)},
	{UTexture::ETextureFormats::Depth, UTexture::FTexFormatToGLType(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, sizeof(float) * 1)},
};

UTexture::FTextureData::FTextureData()
{
	Data = nullptr;
	Width = 0;
	Height = 0;
	Depth = 0;
	bGenerateMips = false;
	Format = ETextureFormats::RGBA8;
}

UTexture::FTextureData::~FTextureData()
{
	delete[] Data;
}

void UTexture::Begin()
{
	RenderData = new FTextureData();
	TextureID = 0;
	glGenTextures(1, &TextureID);
}

void UTexture::End()
{
	delete RenderData;
	glDeleteTextures(1, &TextureID);
}

UTexture::FTextureData* UTexture::GetTextureData() const
{
	return RenderData;
}

unsigned int UTexture::GetTextureID() const
{
	return TextureID;
}