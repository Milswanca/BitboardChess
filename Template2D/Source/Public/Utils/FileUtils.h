#pragma once
#include <string>
#include "Core/Object.h"

class UFileUtils : public UObject
{
public:
    static bool DoesFileExist(const std::string& File);
    static bool LoadFileToString(const std::string& File, std::string& Dest);
    static std::string GetPathExtension(const std::string& File, bool bIncludeDot = false);
    static std::string GetDirectory(const std::string& Path);

    static bool FindRelativePath(const std::string& BaseDir, const std::string& AbsolutePath, std::string& OutPath);
};
