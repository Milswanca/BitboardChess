#include "Rendering/Shader.h"
#include "Core/Engine.h"
#include "Utils/FileUtils.h"
#include <fstream>
#include <iostream>

UShader* UShader::Create(const FCreateShaderParams& Params)
{
	UShader* NewShader = UEngine::Get()->NewObject<UShader>();

	if (Params.VertexShader.size() > 0) NewShader->AddShader(EShaderTypes::Vertex, Params.VertexShader);
	if (Params.FragmentShader.size() > 0) NewShader->AddShader(EShaderTypes::Fragment, Params.FragmentShader);
	if (Params.TessControlShader.size() > 0) NewShader->AddShader(EShaderTypes::TessControl, Params.TessControlShader);
	if (Params.TessEvalShader.size() > 0) NewShader->AddShader(EShaderTypes::TessEval, Params.TessEvalShader);

	NewShader->Compile();
	return NewShader;
}

void UShader::Begin()
{
	ShaderFlags = 0;
	ProgramID = glCreateProgram();
	ShaderIDs[static_cast<int>(EShaderTypes::Vertex)] = glCreateShader(GL_VERTEX_SHADER);
	ShaderIDs[static_cast<int>(EShaderTypes::Fragment)] = glCreateShader(GL_FRAGMENT_SHADER);
	ShaderIDs[static_cast<int>(EShaderTypes::TessControl)] = glCreateShader(GL_TESS_CONTROL_SHADER);
	ShaderIDs[static_cast<int>(EShaderTypes::TessEval)] = glCreateShader(GL_TESS_EVALUATION_SHADER);
}

void UShader::End()
{
	glDeleteProgram(ProgramID);
	glDeleteShader(ShaderIDs[static_cast<int>(EShaderTypes::Vertex)]);
	glDeleteShader(ShaderIDs[static_cast<int>(EShaderTypes::Fragment)]);
	glDeleteShader(ShaderIDs[static_cast<int>(EShaderTypes::TessControl)]);
	glDeleteShader(ShaderIDs[static_cast<int>(EShaderTypes::TessEval)]);
}

void UShader::AddShader(EShaderTypes Type, const std::string& Source)
{
	std::string File;

	if (LoadShaderProgram(Source, File))
	{
		ShaderSources[static_cast<unsigned int>(Type)] = Source;
		const char* CharPtr = File.c_str();
		ShaderFlags |= (1 << static_cast<int>(Type));
		glShaderSource(ShaderIDs[static_cast<int>(Type)], 1, &CharPtr, 0);
	}
	else
	{
		// Throw invalid file error
		std::cerr << "Failed to load shader program " << Source;
	}
}

void UShader::RemoveShader(EShaderTypes Type)
{
	glShaderSource(ShaderIDs[static_cast<int>(Type)], 1, nullptr, 0);
	ShaderFlags &= ~(1 << static_cast<int>(Type));
}

bool UShader::LoadShaderProgram(const std::string& File, std::string& Contents) const
{
	std::ifstream stream(File.c_str());

	if (!stream.is_open())
	{
		std::cerr << "Failed to open shader file " << File;
		return false;
	}

	Contents = "";
	std::string includeIdentifier = "#include";
	std::string lineBuffer;

	while (std::getline(stream, lineBuffer))
	{
		if (lineBuffer.find(includeIdentifier) != lineBuffer.npos)
		{
			// Remove the include identifier, this will cause the path to remain
			// +1 to remove the space
			lineBuffer.erase(0, includeIdentifier.size() + 1);

			// The include path is relative to the current shader file path
			std::string pathOfThisFile = UFileUtils::GetDirectory(File);
			lineBuffer.insert(0, pathOfThisFile);

			std::string includeContents;
			if (!LoadShaderProgram(lineBuffer, includeContents))
				return false;

			Contents += includeContents;

			// Remove the null terminator from the include
			Contents.erase(Contents.length() - 1);

			// Do not add this line to the shader source code, as the include
			// path would generate a compilation issue in the final source code
			continue;
		}

		Contents += lineBuffer + '\n';
	}

	// Only add the null terminator at the end of the complete file,
	// essentially skipping recursive function calls this way
	Contents += '\0';

	stream.close();
	return true;
}

void UShader::Compile()
{
	GLint Count;
	GLuint Shaders[5];
	glGetAttachedShaders(ProgramID, 5, &Count, Shaders);

	for (int i = 0; i < Count; ++i)
	{
		glDetachShader(ProgramID, Shaders[i]);
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(EShaderTypes::COUNT); ++i)
	{
		if (HasShaderType(static_cast<EShaderTypes>(i)))
		{
			glCompileShader(ShaderIDs[i]);

			int Success;
			char InfoLog[512];

			// print compile errors if any
			glGetShaderiv(ShaderIDs[i], GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				glGetShaderInfoLog(ShaderIDs[i], 512, NULL, InfoLog);
				std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << InfoLog << std::endl;
			};

			glAttachShader(ProgramID, ShaderIDs[i]);
		}
	}

	glLinkProgram(ProgramID);
	CacheParameters();
}

void UShader::CacheParameters()
{
	Parameters.empty();

	glUseProgram(GetProgramID());

	GLint count;
	glGetProgramiv(GetProgramID(), GL_ACTIVE_UNIFORMS, &count);

	for (GLint i = 0; i < count; i++)
	{
		FShaderParameterCache Param;
		GLint Length;
		GLint Size;
		GLenum Type;
		GLchar Name[24];
		glGetActiveUniform(GetProgramID(), (GLuint)i, 24, &Length, &Size, &Type, Name);
		Param.ParameterLocation = glGetUniformLocation(GetProgramID(), Name);
		Param.ParameterName = std::string(Name);

		Parameters[Param.ParameterName] = Param;
	}
}

bool UShader::HasShaderType(EShaderTypes Type) const
{
	return (ShaderFlags & (1 << (static_cast<int>(Type)))) != 0;
}

unsigned int UShader::GetProgramID() const
{
	return ProgramID;
}

int UShader::GetParameterLocation(const std::string& Name) const
{
	auto iter = Parameters.find(Name);

	if (iter == Parameters.end())
	{
		return -1;
	}

	return iter->second.ParameterLocation;
}