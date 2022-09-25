#include "Rendering/Texture2D.h"
#include "Core/Engine.h"
#include "Utils/FileUtils.h"
#include "SOIL/SOIL.h"

UTexture2D* UTexture2D::Create(const std::string& File)
{
	UTexture2D* NewTexture = UEngine::Get()->NewObject<UTexture2D>();

	FTextureData* Data = NewTexture->GetTextureData();
	Data->bGenerateMips = false;

	int NumChannels;
	Data->Data = SOIL_load_image(File.c_str(), &Data->Width, &Data->Height, &NumChannels, SOIL_LOAD_AUTO);

	switch (NumChannels)
	{
	case 1:
		Data->Format = ETextureFormats::R8;
		break;
	case 2:
		Data->Format = ETextureFormats::RG8;
		break;
	case 3:
		Data->Format = ETextureFormats::RGB8;
		break;
	case 4:
		Data->Format = ETextureFormats::RGBA8;
		break;
	}

	NewTexture->Build();
	return NewTexture;
}

void UTexture2D::Build()
{
	FTextureData* TextureData = GetTextureData();
	
	FTexFormatToGLType GLData = TexFormatsToGLTypes[TextureData->Format];
	glBindTexture(GL_TEXTURE_2D, GetTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GLData.SourceFormat, TextureData->Width, TextureData->Height, 0, GLData.DesiredFormat, GLData.DataType, TextureData->Data);

	if (TextureData->bGenerateMips)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}