//
//  ReadJSHelper.h
//  Samples
//
//

#ifndef __Samples__ReadJSHelper__
#define __Samples__ReadJSHelper__

#include "cocos2d.h"
#include <string>
#include <list>
#include <fstream>

//e_FlushOnly 
enum FlushType
{
    e_FlushOnly = 0,
    e_FlushSceneRun,
    e_FlushUIRun,
    e_FlushCSBRun,
    e_FlushCSDRun
};
class ReadJSHelper
{
public:
    ReadJSHelper();
    ~ReadJSHelper();
private:
    void Read();
    void GetCommandInfo(std::string strCmdLine);
    void GetCommand(std::string strCmdLine);
    bool FileIsExist(const char *filePath);
    std::wstring c2w(const char *pc);
    std::vector<std::string> split(std::string str,std::string pattern);
public:
    void DealCommandLine(std::string cmdLine);
    void String_replace(std::string&s1,const std::string&s2,const std::string&s3);
    static ReadJSHelper* Instance();
    static void Destroy();
    cocos2d::CCSize GetWinSize();
    std::string GetJsonPath();
    FlushType GetRunType();
    std::string GetResources();
    std::string GetCSDResPath();
    std::string GetCSDType();
    void parseSLN();
private:
    cocos2d::CCSize m_WinSize;
    std::string m_Resources;
    std::string m_SlnPath;
    std::string m_CSDPath;
    std::string m_CSDType;
    std::string m_Json;
    FlushType m_FT;
    std::string m_XmlFullPath;
    std::vector<std::string> m_DllFullPath;
private:
    static ReadJSHelper *s_pHelper ;
};


#endif 