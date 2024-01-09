#include "main.h"
#include "pd_api.h"
#include "CGame.h"
#include "pd_helperfuncs.h"
#include "Common.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	if ( event == kEventInit )
	{
		setPDPtr(playdate);
		playdate->display->setRefreshRate(DesiredFps);
		playdate->system->setUpdateCallback(CGame_MainLoop, NULL);
		CGame_Init();
	}

	if (event == kEventTerminate)
	{
		CGame_DeInit();
	}
	return 0;
}
