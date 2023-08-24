#include <pd_api.h>
#include <stdbool.h>
#include "CInput.h"
#include "pd_helperfuncs.h"


PDButtons ButtonsPressed, ButtonsReleased;
SButtons CInput_Buttons, CInput_PrevButtons;

void CInput_Init()
{
	CInput_ResetButtons();
}

void CInput_DeInit()
{
}

void CInput_Update()
{
	CInput_PrevButtons = CInput_Buttons;
	pd->system->getButtonState(NULL, &ButtonsPressed, &ButtonsReleased);
	if (ButtonsPressed & kButtonA)
		CInput_Buttons.ButA = true;
	if (ButtonsPressed & kButtonB)
		CInput_Buttons.ButBack = true;
	if (ButtonsPressed & kButtonDown)
		CInput_Buttons.ButDown = true;
	if (ButtonsPressed & kButtonLeft)
		CInput_Buttons.ButLeft = true;
	if (ButtonsPressed & kButtonUp)
		CInput_Buttons.ButUp = true;
	if (ButtonsPressed & kButtonRight)
		CInput_Buttons.ButRight = true;

	if (ButtonsReleased & kButtonA)
		CInput_Buttons.ButA = false;
	if (ButtonsReleased & kButtonB)
		CInput_Buttons.ButBack = false;
	if (ButtonsReleased & kButtonDown)
		CInput_Buttons.ButDown = false;
	if (ButtonsReleased & kButtonLeft)
		CInput_Buttons.ButLeft = false;
	if (ButtonsReleased & kButtonUp)
		CInput_Buttons.ButUp = false;
	if (ButtonsReleased & kButtonRight)
		CInput_Buttons.ButRight = false;
	//assign crankc to certain buttons ?

}

void CInput_ResetButtons()
{
	CInput_Buttons.ButLeft = false;
	CInput_Buttons.ButRight = false;
	CInput_Buttons.ButUp = false;
	CInput_Buttons.ButDown = false;
	CInput_Buttons.ButLB = false;
	CInput_Buttons.ButRB = false;
	CInput_Buttons.ButLT = false;
	CInput_Buttons.ButRT = false;
	CInput_Buttons.ButBack = false;
	CInput_Buttons.ButA = false;
	CInput_Buttons.ButB = false;
	CInput_Buttons.ButX = false;
	CInput_Buttons.ButY = false;
	CInput_Buttons.ButStart = false;
	CInput_Buttons.ButDpadLeft = false;
	CInput_Buttons.ButDpadRight = false;
	CInput_Buttons.ButDpadUp = false;
	CInput_Buttons.ButDpadDown = false;
	CInput_Buttons.ButLeft2 = false;
	CInput_Buttons.ButRight2 = false;
	CInput_Buttons.ButUp2 = false;
	CInput_Buttons.ButDown2 = false;
	CInput_PrevButtons = CInput_Buttons;
}