#include "ReadJSHelper.h"
#if defined(WIN32) && defined(_WINDOWS)
#include <io.h>
#endif
#include <algorithm>

#include "tinyxml2/tinyxml2.h"

using namespace std;
using namespace cocos2d;

ReadJSHelper* ReadJSHelper::s_pHelper = NULL;

ReadJSHelper::ReadJSHelper()
{
    m_Resources = "";
    m_Json = "";
    m_CSDType = "";
}

ReadJSHelper::~ReadJSHelper()
{

}

ReadJSHelper* ReadJSHelper::Instance()
{
    if (NULL == s_pHelper)
    {
        s_pHelper = new ReadJSHelper() ;
    }
    return s_pHelper ;
}

void ReadJSHelper::Destroy()
{
    if( s_pHelper )
    {
        delete s_pHelper;
        s_pHelper = NULL;
    }
}


cocos2d::CCSize ReadJSHelper::GetWinSize()
{
    return m_WinSize;
}

std::string ReadJSHelper::GetJsonPath()
{
    return m_Json;
}

FlushType ReadJSHelper::GetRunType()
{
    return m_FT;
}

std::string ReadJSHelper::GetResources()
{
    return m_Resources;
}

std::string ReadJSHelper::GetCSDResPath()
{
    return m_CSDPath;
}

std::string ReadJSHelper::GetCSDType()
{
    return m_CSDType;
}

void ReadJSHelper::Read()
{
    if(m_XmlFullPath == "")
        return;

    if(m_FT == 4)
    {
        parseSLN();
        return;
    }

    ssize_t _size;
    const char *_pFileContent = (char*)(cocos2d::CCFileUtils::getInstance()->getFileData(m_XmlFullPath.c_str() , "r", &_size));
    if (_pFileContent == NULL) {
        return;
    }

    std::string strContent(_pFileContent);

    tinyxml2::XMLDocument	_document;
    _document.Parse(strContent.c_str());

    tinyxml2::XMLElement	*_root = _document.RootElement();
    if (_root == NULL)
    {
        return;
    }
    tinyxml2::XMLElement *CanvasSize = _root->FirstChildElement("CanvasSize");
    tinyxml2::XMLElement *width = CanvasSize->FirstChildElement("Width");
    tinyxml2::XMLElement *height = CanvasSize->FirstChildElement("Height");
    m_WinSize.width = atof(width->FirstChild()->Value());
    m_WinSize.height = atof(height->FirstChild()->Value());

    tinyxml2::XMLElement *resRelativepath = _root->FirstChildElement("ResRelativePath");
    std::string version2 = "";
    std::string version1;

    if (m_FT == 1)
    {
        version1.assign(resRelativepath->FirstChild()->Value());
        if (version2 != version1)
        {
            int nPos = m_XmlFullPath.find("CocoStudio\\ccsprojs");
            if (nPos < 0)
            {
                return;
            }
            m_Resources = m_XmlFullPath.substr(0, nPos);//m_XmlFullPath.Left(nPos).Trim();
            m_Resources += "CocoStudio\\assets";
#if defined(WIN32) && defined(_WINDOWS)
            SetCurrentDirectoryA(m_Resources.c_str());
#endif
            cocos2d::CCFileUtils::getInstance()->addSearchPath(m_Resources.c_str());
            std::string dir(m_Resources);

            tinyxml2::XMLElement *SceneJson = _root->FirstChildElement("Name");
            m_Json.assign(m_Resources);
            m_Json.append("/publish/");
            m_Json.append(SceneJson->FirstChild()->Value());
        }
        else
        {
            tinyxml2::XMLElement *Resources = _root->FirstChildElement("Resources");
            m_Resources.assign(Resources->FirstChild()->Value());
            m_Resources.append("/");
#if defined(WIN32) && defined(_WINDOWS)
            SetCurrentDirectoryA(m_Resources.c_str());
#endif
            cocos2d::CCFileUtils::getInstance()->addSearchPath(m_Resources.c_str());

            tinyxml2::XMLElement *SceneJson = _root->FirstChildElement("Name");
            m_Json.assign(m_Resources);
            m_Json.append(SceneJson->FirstChild()->Value());
            m_Json.append(".json");
        }
    }
    else if(m_FT == 2)
    {
        tinyxml2::XMLElement *Resources = _root->FirstChildElement("Resources");
        m_Resources.assign(Resources->FirstChild()->Value());
        m_Resources.append("/");
#if defined(WIN32) && defined(_WINDOWS) 
        SetCurrentDirectoryA(m_Resources.c_str());
#endif
        cocos2d::CCFileUtils::getInstance()->addSearchPath(m_Resources.c_str());

        tinyxml2::XMLElement *uiJson = _root->FirstChildElement("CurrentUIJson");
        int nPos = m_Resources.find("Resources");
        nPos -= 1;
        if (nPos > 0 && nPos != string::npos)
        {
            m_Json = m_Resources.substr(0, nPos);
            m_Json.append("/Json/");
            m_Json.append(uiJson->FirstChild()->Value());

        }
    }
    else if(m_FT == 3)
    {
        tinyxml2::XMLElement *Resources = _root->FirstChildElement("Resources");
        m_Resources.assign(Resources->FirstChild()->Value());
        m_Resources.append("/");
#if defined(WIN32) && defined(_WINDOWS) 
        SetCurrentDirectoryA(m_Resources.c_str());
#endif
        cocos2d::CCFileUtils::getInstance()->addSearchPath(m_Resources.c_str());

        tinyxml2::XMLElement *uiJson = _root->FirstChildElement("CurrentUIJson");
        int nPos = m_Resources.find("Resources");
        nPos -= 1;
        if (nPos > 0 && nPos != string::npos)
        {
            m_Json = m_Resources.substr(0, nPos);
            m_Json.append("/Json/");
            m_Json.append(uiJson->FirstChild()->Value());
            string src = ".json";
            string dsc = ".csb";
            String_replace(m_Json, src, dsc);

        }
    }


#if defined(WIN32) && defined(_WINDOWS)
    for (std::vector<std::string>::iterator iter = m_DllFullPath.begin(); iter != m_DllFullPath.end(); ++iter)
    {
        if ((*iter) != "" && FileIsExist((*iter).c_str()))
        {
            std::wstring dllPath = c2w((*iter).c_str());
            LoadLibrary(dllPath.c_str());
        }
    }
#endif
}

void ReadJSHelper::GetCommandInfo(string strCmdLine)
{
    string strParamFlag;		
    string strParamContent;	
    int nPos = strCmdLine.find(' ');
    if (nPos != string::npos && nPos != string::npos)
    {
        strParamFlag = strCmdLine.substr(0, nPos);
        strCmdLine = strCmdLine.substr(nPos + 1, strCmdLine.length() - 1);
        std::transform(strParamFlag.begin(),strParamFlag.end(),strParamFlag.begin(), (int(*)(int))toupper);
        if (strParamFlag == "-FLUSHTYPE")
        {
            if (strCmdLine.empty())
            {
                return;
            }
            nPos = strCmdLine.find("-XMLPATH");
            nPos -= 1;
            if ( nPos > 0 && nPos != string::npos)
            {
                strParamContent = strCmdLine.substr(0, nPos); //strCmdLine.Left(nPos).Trim();
                strCmdLine = strCmdLine.substr(nPos + 1, strCmdLine.length() - 1); //strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
            }
            else
            {
                //strParamContent = strCmdLine.Trim();
                strCmdLine.clear();
            }
            m_FT = (FlushType)(atoi(strParamContent.c_str()));
        }
        else if (strParamFlag == "-XMLPATH")
        {
            if (strCmdLine.empty())
            {
                return;
            }
            nPos = strCmdLine.find("-RES");
            nPos -= 1;
            if ( nPos > 0 && nPos != string::npos )
            {
                strParamContent = strCmdLine.substr(0, nPos); //strCmdLine.Left(nPos).Trim();
                strCmdLine = strCmdLine.substr(nPos + 1, strCmdLine.length() - 1);//strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
            }
            else
            {
                //strParamContent = strCmdLine.Trim();
                strCmdLine.clear();
            }
            m_XmlFullPath = strParamContent;
        }
        else if(strParamFlag == "-RES")
        {
            if (strCmdLine.empty())
            {
                return;
            }
            nPos = strCmdLine.find("-WIDTH");
            nPos -= 1;
            if ( nPos > 0 && nPos != string::npos )
            {
                strParamContent = strCmdLine.substr(0, nPos); //strCmdLine.Left(nPos).Trim();
                strCmdLine = strCmdLine.substr(nPos + 1, strCmdLine.length() - 1);//strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
            }
            else
            {
                //strParamContent = strCmdLine.Trim();
                strCmdLine.clear();
            }
            m_Resources = strParamContent;
        }
        else if(strParamFlag == "-WIDTH")
        {
            if (strCmdLine.empty())
            {
                return;
            }
            nPos = strCmdLine.find("-HEIGHT");
            nPos -= 1;
            if ( nPos > 0 && nPos != string::npos )
            {
                strParamContent = strCmdLine.substr(0, nPos); //strCmdLine.Left(nPos).Trim();
                strCmdLine = strCmdLine.substr(nPos + 1, strCmdLine.length() - 1);//strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
            }
            else
            {
                //strParamContent = strCmdLine.Trim();
                strCmdLine.clear();
            }
            m_WinSize.width = atoi(strParamContent.c_str());
        }
        else if(strParamFlag == "-HEIGHT")
        {
            if (strCmdLine.empty())
            {
                return;
            }
            nPos = strCmdLine.find("-DLLPATH");
            nPos -= 1;
            if ( nPos > 0 && nPos != string::npos )
            {
                strParamContent = strCmdLine.substr(0, nPos); //strCmdLine.Left(nPos).Trim();
                strCmdLine = strCmdLine.substr(nPos + 1, strCmdLine.length() - 1);//strCmdLine.Right( strCmdLine.GetLength() - nPos - 1 ).Trim();
            }
            else
            {
                //strParamContent = strCmdLine.Trim();
                strCmdLine.clear();
            }
            m_WinSize.height = atoi(strParamContent.c_str());;
        }
        else if (strParamFlag == "-DLLPATH")
        {
            if (strCmdLine.empty())
            {
                return;
            }
            m_DllFullPath = split(strCmdLine, "|");
        }
        GetCommandInfo(strCmdLine);
    }
    else
    {
    }
}


//字符串分割函数
std::vector<std::string> ReadJSHelper::split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

void ReadJSHelper::GetCommand(string strCmdLine)
{
    GetCommandInfo(strCmdLine);
}

bool ReadJSHelper::FileIsExist(const char *filePath)
{
#if defined(WIN32) && defined(_WINDOWS)
    //#define F_OK 0 /* Test for existence. */
    return (access(filePath, 0) == 0);
#else
    return false;
#endif
}

std::wstring ReadJSHelper::c2w(const char *pc)
{
    std::wstring val = L"";  

    if(NULL == pc)  
    {  
        return val;  
    }  
    //size_t size_of_ch = strlen(pc)*sizeof(char);  
    //size_t size_of_wc = get_wchar_size(pc);  
    size_t size_of_wc;  
    size_t destlen = mbstowcs(0,pc,0);  
    if (destlen ==(size_t)(-1))  
    {  
        return val;  
    }  
    size_of_wc = destlen+1;  
    wchar_t * pw  = new wchar_t[size_of_wc];  
    mbstowcs(pw,pc,size_of_wc);  
    val = pw;  
    delete pw;  
    return val; 
}

void ReadJSHelper::DealCommandLine(std::string cmdLine)
{
    GetCommand(cmdLine);
    Read();
    cocos2d::CCFileUtils::getInstance()->addSearchPath(m_Resources.c_str());
}

void ReadJSHelper::String_replace(string&s1,const string&s2,const string&s3)
{
    string::size_type pos=0;
    string::size_type a=s2.size();
    string::size_type b=s3.size();
    while((pos=s1.find(s2,pos))!=string::npos)
    {
        s1.replace(pos,a,s3);
        pos+=b;
    }
}

void ReadJSHelper::parseSLN()
{
    m_Json.assign(m_XmlFullPath);
    string src = ".csd";
    string dsc = ".csb";
    m_CSDPath = m_Json;
    //	String_replace(m_CSDPath, src, dsc);
    cocos2d::CCFileUtils::getInstance()->addSearchPath(m_Resources.c_str());

    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(m_XmlFullPath).c_str();
    ssize_t size;
    std::string content =(char*)FileUtils::getInstance()->getFileData(fullpath, "r", &size);
}



