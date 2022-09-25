#include "Utils/FileUtils.h"
#include "Utils/StringUtils.h"

#include <fstream>
#include <sstream>
#include <vector>

bool UFileUtils::DoesFileExist(const std::string& File)
{
    struct stat buffer;   
    return (stat (File.c_str(), &buffer) == 0); 
}

bool UFileUtils::LoadFileToString(const std::string& File, std::string& Dest)
{
    std::ifstream f(File);
    if(f)
    {
        std::ostringstream ss;
        ss << f.rdbuf();
        Dest = ss.str();
        return true;
    }
    
    return false;
}

std::string UFileUtils::GetPathExtension(const std::string& File, bool bIncludeDot)
{
    if(File.find_last_of(".") != std::string::npos)
    {
        return File.substr(File.find_last_of('.') + (bIncludeDot ? 0 : 1));
    }

    return "";
}

std::string UFileUtils::GetDirectory(const std::string& Path)
{
    size_t pos = Path.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : Path.substr(0, pos + 1);
}

bool UFileUtils::FindRelativePath(const std::string& BaseDir, const std::string& AbsolutePath, std::string& OutPath)
{
    std::string SAbsolutePath = StringUtils::Replace(AbsolutePath, '/', '\\');
    std::string SBaseDir = StringUtils::Replace(BaseDir, '/', '\\');
    
    if(DoesFileExist(SAbsolutePath))
    {
        OutPath = SAbsolutePath;
        return true;
    }

    std::vector<std::string> Split = StringUtils::Split(SAbsolutePath, '\\');
    for(int i = Split.size() - 1; i >= 0; --i)
    {
        std::string TestPath = SBaseDir + Split[i];

        int CurrIdx = i + 1;
        while(DoesFileExist(TestPath))
        {
            if(CurrIdx >= Split.size())
            {
                OutPath = TestPath;
                return true;
            }

            TestPath += "\\" + Split[CurrIdx];
            CurrIdx++;
        }
    }

    return false;
}
