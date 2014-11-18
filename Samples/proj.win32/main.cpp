#include "main.h"
#include "../Classes/AppDelegate.h"
#include "cocos2d.h"
#include "../Classes/ReadJSHelper.h"
#include <atlstr.h>


USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{

    // create the application instance
    AppDelegate app;
   
	CStringA cmdLine(lpCmdLine);
	ReadJSHelper::Instance()->DealCommandLine(cmdLine.GetBuffer());

	TCHAR szFilePath[MAX_PATH + 1]={0};     
	GetModuleFileName(NULL, szFilePath, MAX_PATH);     
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	CStringA strDefaultRes = szFilePath;
	strDefaultRes.Append("EditorDefaultRes");
	CCFileUtils::sharedFileUtils()->addSearchPath(strDefaultRes.GetBuffer());

	//GLView* glview = GLView::create("Simulator");
 //   glview->setFrameSize(ReadJSHelper::Instance()->GetWinSize().width, ReadJSHelper::Instance()->GetWinSize().height);

	int ret =  CCApplication::sharedApplication()->run();
	ReadJSHelper::Destroy();
	return ret;
}
