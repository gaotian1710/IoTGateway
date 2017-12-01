#include "stdafx.h"
#include "Fsm.h"

/*!
\brief transition [*] --> first state forcefully.
*/
void YTAFsm_InitialTransit(PYTAFsm fsm, PCYTAState s)
{
	fsm->state = s;
}

/*!
\brief usuall transition
\param fms [in,out] state machine
\param msg [in,out] message to the state machine
\return 
*/
YTASHResult YTAFsm_Transit(PYTAFsm fsm, PYTAFsmMsg msg)
{
	PCYTAState currentState = fsm->state;
	YTASHResult result = fsm->state->sh(fsm, msg);
	if (result & YTASHResult_Transited)
	{
		currentState->ahExit(fsm);
		fsm->state->ahEnter(fsm);
	}
	return result;
}

YTASHResult YTASHNone(PYTAFsm fsm, PYTAFsmMsg msg)
{
	return YTASHResult_Ignored;
}

void YTAAHNone(PYTAFsm fsm)
{
}
