#ifndef CINPUT_H
#define CINPUT_H

#include <pd_api.h>
#include <stdbool.h>

struct SButtons {
	bool ButLeft, ButRight, ButUp, ButDown,
		ButDpadLeft, ButDpadRight, ButDpadUp, ButDpadDown,
		ButLeft2, ButRight2, ButUp2, ButDown2,
		ButBack, ButStart, ButA, ButB,
		ButX, ButY, ButLB, ButRB, ButRT, ButLT;
};
typedef struct SButtons SButtons;

extern SButtons CInput_Buttons, CInput_PrevButtons;
void CInput_Init();
void CInput_DeInit();
void CInput_Update();
void CInput_ResetButtons();

#endif