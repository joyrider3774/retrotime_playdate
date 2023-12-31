#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <pd_api.h>
#include <stdbool.h>

#define ShowFPS false
#define debugInfoStats false
#define debugInfoFonts false
#define debugInfoImages false
#define debugInfoAudio false
#define debugShowCollisionShapes false
#define dumpScaledBitmaps false
#define useDefaultColorAssets false
#define loadDumpedScaledBitmaps true
#define BatteryMonitoring false
#define AlphaEnabled true

#undef DrawTextColorBitmapCacheCount
#define DrawTextColorBitmapCacheCount 20

#define epsilion 0.0001f

#define MaxAlpha 245
#define AlphaIncrease 5
#define MaxAlphaTime 500

#define ScreenWidth 400
#define ScreenHeight 240
#define WindowWidth 400
#define WindowHeight 240
#define xscale (ScreenWidth / 1280.0f)
#define yscale (ScreenHeight / 720.0f)

#define DesiredFps 30
#define FPSScale (60.0f / (DesiredFps == 0 ? 60:DesiredFps))

#define ScoreBarHeight (int)(39*yscale)

//GameStates
#define initDiff 50

#define GSIntro 1
#define GSGame 2
#define GSTitleScreen 3
#define GSRamIt 4
#define GSSubScore 5
#define GSSpaceInvaders 6
#define GSBreakout 7
#define GSFrog 8
#define GSSnake 9
#define GSPang 10
#define GSTetris 11
#define GSEddy 12

#define GSIntroInit GSIntro + initDiff
#define GSGameInit GSGame + initDiff
#define GSTitleScreenInit GSTitleScreen + initDiff
#define GSRamItInit GSRamIt + initDiff
#define GSSubScoreInit GSSubScore + initDiff

#define GSSpaceInvadersInit GSSpaceInvaders + initDiff
#define GSBreakoutInit GSBreakout + initDiff
#define GSFrogInit GSFrog + initDiff
#define GSSnakeInit GSSnake + initDiff
#define GSPangInit GSPang + initDiff
#define GSTetrisInit GSTetris + initDiff
#define GSEddyInit GSEddy + initDiff

//SubGameStates
#define SGNone 0
#define SGReadyGo 1
#define SGGame 2
#define SGTimeUp 3
#define SGFrame 4
#define SGCalcScore 5
#define SGFadeIn 6
#define SGPauseMenu 7
#define SGGameHelp 8

//GameModes
#define Modes 3
#define GMRetroCarousel 0
#define GMGame 1
#define GMTimeAttack 2

#define Games 8

//PAUSEMENUS
#define PMNone -1
#define PMContinue 0
#define PMQuit 1
#define PMMusicVol 2
#define PMSoundVol 3
#define PMGameHelp 4


#define PauseMenus 10

//main menus
#define MMStart 0
#define MMHighScores 1
#define MMHelp 2
#define MMOptions 3
#define MMCredits 4
//Sub Main Menus
#define MMSelectGameMode 5
#define MMSelectGame 6
//6 because only fist 5 are real main menu
#define MainMenus 5

//options menu
#define OMMusicVol 0
#define OMSoundVol 1
#define OMResetHighScores 2
#define OMBack 3
#define OptionMenus 4

struct ModeDesc {
	int mode;
	char* name;
	char* description;
};
typedef struct ModeDesc ModeDesc;

struct GameDesc {
	int state;
	char* name;
	char* description;
	char* shortdescription;
};
typedef struct GameDesc GameDesc;

struct PauseMenuDesc {
	int menu;
	char* name;
};
typedef struct PauseMenuDesc PauseMenuDesc;

struct GPGamePauseMenusDesc {
	int game;
	int menus[5];
};
typedef struct GPGamePauseMenusDesc GPGamePauseMenusDesc;

struct MainMenusDesc {
	int menu;
	char* name;
};
typedef struct MainMenusDesc MainMenusDesc;

struct OptionMenusDesc {
	int menu;
	char* name;
};
typedef struct OptionMenusDesc OptionMenusDesc;

extern unsigned int prevLogTime;
extern unsigned int FrameTime, Frames;
extern float CurrentMs;

extern GameDesc GSGames[Games];
extern ModeDesc GMModes[Modes];
extern MainMenusDesc MMMainMenus[MainMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];
extern GPGamePauseMenusDesc GPGamePauseMenus[Games];
extern PauseMenuDesc PMPauseMenus[PauseMenus];
extern OptionMenusDesc OMOptionMenus[OptionMenus];

#endif