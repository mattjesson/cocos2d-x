// Application main file.
 
// Original file name: TestAudioEngineEntry.cpp
// Generated by TOPS Builder:Project wizard,Date:2010-9-29
 

 
#include  "TestAudioEngineEntry.h"
#include  "TestAudioEngineApp.h"
#include  "testaudioengine_res_c.h"

const  ResourceRegisterEntry		ResRegList_TestAudioEngine[]  =
{
	TG_RESOURCE_DEFINE
};

extern const  AppResourceEntry		TestAudioEngineResourceEntry  =
{
	(ResourceRegisterEntry*)ResRegList_TestAudioEngine,                    //  res  list  in  this  app
		sizeof(ResRegList_TestAudioEngine) /  sizeof(ResourceRegisterEntry),    //number  of  item  in  res
};

Int32 TG3AppMain(const TUChar * pAppID, UInt32 nCmd, void * pCmdParam)
{
	switch(nCmd)
	{
	case 0: // 主入口
		{
			// UI应用程序例子：
			TTestAudioEngineApp  *  pApp= new  TTestAudioEngineApp();
			pApp->WM_SetResourceEntry(&TestAudioEngineResourceEntry);
			pApp->Run();

			delete pApp;
			break;
		}
	}
	return 1;
}


