#pragma once
typedef struct _YTAFsmMsg {
	void* param;
	uint32_t msg;
} YTAFsmMsg, *PYTAFsmMsg;

typedef const YTAFsmMsg *PCYTAFsmMsg;

struct _YTAFsm; // declare prototype

typedef enum {
	YTASHResult_Ignored = 0,
	YTASHResult_Transited = 1,
	YTASHResult_Handled = 2
} YTASHResult;

typedef YTASHResult (*YTAStateHandler)(struct _YTAFsm* fsm, PYTAFsmMsg msg);
typedef void (*YTAActionHandler)(struct _YTAFsm* fsm);
typedef struct _YTAState {
	YTAStateHandler sh;
	YTAActionHandler ahEnter, ahExit;
} YTAState, *PYTAState;

typedef const YTAState *PCYTAState;

typedef struct _YTAFsm {
	void* context;
	PYTAFsmMsg msg;
	PCYTAState state;
} YTAFsm, *PYTAFsm;

#if defined(__cplusplus)
extern "C" {
#endif
	void YTAFsm_InitialTransit(PYTAFsm fsm, PCYTAState s);
	YTASHResult YTAFsm_Transit(PYTAFsm fsm, PYTAFsmMsg msg);
	YTASHResult YTASHNone(PYTAFsm fsm, PYTAFsmMsg msg);
	void YTAAHNone(PYTAFsm fsm);
#if defined(__cplusplus)
}
#endif
