#pragma once
#include "Core/Object.h"
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

class UShader;
class UTexture;

class UMaterial : public UObject
{
public:
	enum class ParameterTypes
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Mat4
	};

	struct CachedParameter
	{
		std::string Name;
		char* Bytes;
		unsigned int Size;
		ParameterTypes Type;
	};
	typedef std::unordered_map<std::string, UMaterial::CachedParameter*> MaterialParameterMap;

public:
	static UMaterial* Create(UShader* Shader);

protected:
	IMPLEMENT_CONSTRUCTOR(UMaterial, UObject);

	virtual void End() override;

public:
	void Bind();
	void Unbind();

	void SetInt(const std::string& Name, const int Value);
	void SetFloat(const std::string& Name, const float Value);
	void SetVector2(const std::string& Name, const glm::vec2& Value);
	void SetVector3(const std::string& Name, const glm::vec3& Value);
	void SetVector4(const std::string& Name, const glm::vec4& Value);
	void SetMat4(const std::string& Name, const glm::mat4& Value);
	void SetTexture(const std::string& Name, UTexture* Value);

	bool GetInt(const std::string& Name, int& Value) const;
	bool GetFloat(const std::string& Name, float& Value) const;
	bool GetVector2(const std::string& Name, glm::vec2& Value) const;
	bool GetVector3(const std::string& Name, glm::vec3& Value) const;
	bool GetVector4(const std::string& Name, glm::vec4& Value) const;
	bool GetMat4(const std::string& Name, glm::mat4& Value) const;
	bool GetTexture(const std::string& Name, UTexture*& OutValue) const;

	void SetShader(UShader* NewShader);
	UShader* GetShader() const;

private:
	CachedParameter* GetCachedParameter(std::string Name) const;
	CachedParameter* GetOrCreateCachedParameter(std::string Name, int SizeBytes);

	int FindTextureParamIndex(const std::string& Name) const;
	void BindTextures();
	void UnbindTextures();

	UTexture* Textures[32];
	std::string TextureParamNames[32];
	unsigned int NumTextureParams = 0;

	UShader* Shader;
	MaterialParameterMap Parameters;
};