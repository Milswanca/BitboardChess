#pragma once
#include "Core/Object.h"
#include "Rendering/Material.h"
#include <glad/glad.h>
#include <unordered_map>

enum class EShaderTypes
{
	Unknown,
	Vertex,
	Fragment,
	TessControl,
	TessEval,
	COUNT
};

class UShader : public UObject
{
public:
	struct FShaderParameterCache
	{
		std::string ParameterName;
		int ParameterLocation;
	};
	typedef std::unordered_map<std::string, FShaderParameterCache> FShaderParameterMap;

public:
	struct FCreateShaderParams
	{
		FCreateShaderParams()
		{
			VertexShader = "";
			FragmentShader = "";
			TessControlShader = "";
			TessEvalShader = "";
		}

		std::string VertexShader;
		std::string FragmentShader;
		std::string TessControlShader;
		std::string TessEvalShader;
	};

	static UShader* Create(const FCreateShaderParams& Params);

protected:
	IMPLEMENT_CONSTRUCTOR(UShader, UObject);

	virtual void Begin() override;
	virtual void End() override;

public:
	void AddShader(EShaderTypes Type, const std::string& Source);
	void RemoveShader(EShaderTypes Type);
	void Compile();
	bool HasShaderType(EShaderTypes Type) const;

	unsigned int GetProgramID() const;
	int GetParameterLocation(const std::string& Name) const;

private:
	bool LoadShaderProgram(const std::string& Source, std::string& Contents) const;
	void CacheParameters();

	int ShaderFlags;

	unsigned int ShaderIDs[static_cast<int>(EShaderTypes::COUNT)];
	std::string ShaderSources[static_cast<int>(EShaderTypes::COUNT)];

	unsigned int ProgramID;
	FShaderParameterMap Parameters;
};