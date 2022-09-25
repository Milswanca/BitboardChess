#include "Rendering/Material.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Core/Engine.h"
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.inl>

UMaterial* UMaterial::Create(UShader* Shader)
{
	UMaterial* Material = UEngine::Get()->NewObject<UMaterial>();
	Material->SetShader(Shader);
	return Material;
}

void UMaterial::End()
{
	UObject::End();

	for (auto Iter : Parameters)
	{
		delete[] Iter.second->Bytes;
	}
}

void UMaterial::Bind()
{
	if (Shader == nullptr)
		return;

	glUseProgram(Shader->GetProgramID());

	for (auto Iter : Parameters)
	{
		int Loc = Shader->GetParameterLocation(Iter.first);

		switch (Iter.second->Type)
		{
		case ParameterTypes::Int:
			glUniform1i(Loc, *(GLint*)(Iter.second->Bytes));
			break;

		case ParameterTypes::Float:
			glUniform1f(Loc, *(GLfloat*)(Iter.second->Bytes));
			break;

		case ParameterTypes::Vector2:
			glUniform2fv(Loc, 1, (GLfloat*)Iter.second->Bytes);
			break;

		case ParameterTypes::Vector3:
			glUniform3fv(Loc, 1, (GLfloat*)Iter.second->Bytes);
			break;

		case ParameterTypes::Vector4:
			glUniform4fv(Loc, 1, (GLfloat*)Iter.second->Bytes);
			break;

		case ParameterTypes::Mat4:
			glUniformMatrix4fv(Loc, 1, GL_FALSE, (GLfloat*)Iter.second->Bytes);
			break;
		}
	}

	BindTextures();
}

void UMaterial::Unbind()
{
	if (Shader == nullptr)
		return;

	glUseProgram(0);
	glm::mat4 emptyMatrix = glm::identity<glm::mat4>();

	for (auto Iter = Parameters.begin(); Iter != Parameters.end(); ++Iter)
	{
		CachedParameter* Parameter = Iter->second;
		int Loc = Shader->GetParameterLocation(Parameter->Name);

		switch (Parameter->Type)
		{
		case ParameterTypes::Int:
			glUniform1i(Loc, 0);
			break;

		case ParameterTypes::Float:
			glUniform1f(Loc, 0.0f);
			break;

		case ParameterTypes::Vector2:
			glUniform2f(Loc, 0.0f, 0.0f);
			break;

		case ParameterTypes::Vector3:
			glUniform3f(Loc, 0.0f, 0.0f, 0.0f);
			break;

		case ParameterTypes::Vector4:
			glUniform4f(Loc, 0.0f, 0.0f, 0.0f, 0.0f);
			break;

		case ParameterTypes::Mat4:
			glUniformMatrix4fv(Loc, 1, GL_FALSE, (GLfloat*)&emptyMatrix);
			break;
		}
	}

	UnbindTextures();
}

void UMaterial::BindTextures()
{
	for (unsigned int i = 0; i < NumTextureParams; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, Textures[i]->GetTextureID());

		unsigned int Loc = Shader->GetParameterLocation(TextureParamNames[i]);
		glUniform1i(Loc, i);
	}
}

void UMaterial::UnbindTextures()
{
	for (unsigned int i = 0; i < NumTextureParams; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);

		unsigned int Loc = Shader->GetParameterLocation(TextureParamNames[i]);
		glUniform1i(Loc, 0);
	}
}

void UMaterial::SetInt(const std::string& Name, const int Value)
{
	CachedParameter* Param = GetOrCreateCachedParameter(Name, sizeof(int));
	memcpy(Param->Bytes, &Value, sizeof(int));
	Param->Size = sizeof(int);
	Param->Type = ParameterTypes::Int;
}

void UMaterial::SetFloat(const std::string& Name, const float Value)
{
	CachedParameter* Param = GetOrCreateCachedParameter(Name, sizeof(float));
	memcpy(Param->Bytes, &Value, sizeof(float));
	Param->Size = sizeof(float);
	Param->Type = ParameterTypes::Float;
}

void UMaterial::SetVector2(const std::string& Name, const glm::vec2& Value)
{
	CachedParameter* Param = GetOrCreateCachedParameter(Name, sizeof(glm::vec2));
	memcpy(Param->Bytes, &Value, sizeof(glm::vec2));
	Param->Size = sizeof(glm::vec2);
	Param->Type = ParameterTypes::Vector2;
}

void UMaterial::SetVector3(const std::string& Name, const glm::vec3& Value)
{
	CachedParameter* Param = GetOrCreateCachedParameter(Name, sizeof(glm::vec3));
	memcpy(Param->Bytes, &Value, sizeof(glm::vec3));
	Param->Size = sizeof(glm::vec3);
	Param->Type = ParameterTypes::Vector3;
}

void UMaterial::SetVector4(const std::string& Name, const glm::vec4& Value)
{
	CachedParameter* Param = GetOrCreateCachedParameter(Name, sizeof(glm::vec4));
	memcpy(Param->Bytes, &Value, sizeof(glm::vec4));
	Param->Size = sizeof(glm::vec4);
	Param->Type = ParameterTypes::Vector4;
}

void UMaterial::SetMat4(const std::string& Name, const glm::mat4& Value)
{
	CachedParameter* Param = GetOrCreateCachedParameter(Name, sizeof(glm::mat4));
	memcpy(Param->Bytes, &Value, sizeof(glm::mat4));
	Param->Size = sizeof(glm::mat4);
	Param->Type = ParameterTypes::Mat4;
}

void UMaterial::SetTexture(const std::string& Name, UTexture* Value)
{
	int Index = FindTextureParamIndex(Name);
	if (Index != -1)
	{
		Textures[Index] = Textures[NumTextureParams - 1];
		TextureParamNames[Index] = TextureParamNames[NumTextureParams - 1];
		NumTextureParams--;
	}

	if (Value != nullptr)
	{
		Textures[NumTextureParams] = Value;
		TextureParamNames[NumTextureParams] = Name;
		NumTextureParams++;
	}
}

bool UMaterial::GetInt(const std::string& Name, int& Value) const
{
	if (CachedParameter* Param = GetCachedParameter(Name))
	{
		memcpy(&Value, Param->Bytes, sizeof(int));
		return true;
	}

	return false;
}

bool UMaterial::GetFloat(const std::string& Name, float& Value) const
{
	if (CachedParameter* Param = GetCachedParameter(Name))
	{
		memcpy(&Value, Param->Bytes, sizeof(float));
		return true;
	}

	return false;
}

bool UMaterial::GetVector2(const std::string& Name, glm::vec2& Value) const
{
	if (CachedParameter* Param = GetCachedParameter(Name))
	{
		memcpy(&Value, Param->Bytes, sizeof(glm::vec2));
		return true;
	}

	return false;
}

bool UMaterial::GetVector3(const std::string& Name, glm::vec3& Value) const
{
	if (CachedParameter* Param = GetCachedParameter(Name))
	{
		memcpy(&Value, Param->Bytes, sizeof(glm::vec3));
		return true;
	}

	return false;
}

bool UMaterial::GetVector4(const std::string& Name, glm::vec4& Value) const
{
	if (CachedParameter* Param = GetCachedParameter(Name))
	{
		memcpy(&Value, Param->Bytes, sizeof(glm::vec4));
		return true;
	}

	return false;
}

bool UMaterial::GetMat4(const std::string& Name, glm::mat4& Value) const
{
	if (CachedParameter* Param = GetCachedParameter(Name))
	{
		memcpy(&Value, Param->Bytes, sizeof(glm::mat4));
		return true;
	}

	return false;
}

bool UMaterial::GetTexture(const std::string& Name, UTexture*& OutValue) const
{
	int Index = FindTextureParamIndex(Name);

	if (Index != -1)
	{
		OutValue = Textures[Index];
	}

	return Index != -1;
}

void UMaterial::SetShader(UShader* NewShader)
{
	Shader = NewShader;
}

UShader* UMaterial::GetShader() const
{
	return Shader;
}

UMaterial::CachedParameter* UMaterial::GetCachedParameter(std::string Name) const
{
	auto Iter = Parameters.find(Name);
	return Iter != Parameters.end() ? (*Iter).second : nullptr;
}

UMaterial::CachedParameter* UMaterial::GetOrCreateCachedParameter(std::string Name, int SizeBytes)
{
	CachedParameter* ExistingParam = GetCachedParameter(Name);
	
	if (!ExistingParam)
	{
		ExistingParam = new CachedParameter();
		ExistingParam->Name = Name;
		ExistingParam->Bytes = new char[SizeBytes];
		Parameters[Name] = ExistingParam;
	}

	return ExistingParam;
}

int UMaterial::FindTextureParamIndex(const std::string& Name) const
{
	for (size_t i = 0; i < NumTextureParams; ++i)
	{
		if (Name == TextureParamNames[i])
		{
			return i;
		}
	}

	return -1;
}