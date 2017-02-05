// ================================================================================
// ==      This file is a part of Turbo Badger. (C) 2011-2014, Emil Segerås      ==
// ==                     See tb_core.h for more information.                    ==
// ================================================================================

#include "tb_system.h"
#include <iostream>

#include <chrono>

#include "tb_msg.h"
#include "tb_types.h"
#include "tb_str.h"

#include <IWDebug.h>
#include <s3e.h>
#include <s3eTimer.h>

#ifdef TB_RUNTIME_DEBUG_INFO

void TBDebugOut(const char *str)
{
	//SDL_Log("%s", str);
	s3eDebugOutputString(str);
}

#endif // TB_RUNTIME_DEBUG_INFO

namespace tb {

// == TBSystem ========================================

double TBSystem::GetTimeMS()
{
	return (double)std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

}

static uint32 tb_timer_id = 0;
static uint32 tb_timer_callback(uint32 interval, void *param)
{
	tb_timer_id = *((uint32*)&param);
	double next_fire_time = TBMessageHandler::GetNextMessageFireTime();
	double now = TBSystem::GetTimeMS();
	if (next_fire_time != TB_NOT_SOON && (next_fire_time - now) > 1.0)
	{
		// We timed out *before* we were supposed to (the OS is not playing nice).
		// Calling ProcessMessages now won't achieve a thing so force a reschedule
		// of the platform timer again with the same time.
		return (uint32)(next_fire_time - now);
	}

	TBMessageHandler::ProcessMessages();

	// If we still have things to do (because we didn't process all messages,
	// or because there are new messages), we need to rescedule, so call RescheduleTimer.
	next_fire_time = TBMessageHandler::GetNextMessageFireTime();
	if (next_fire_time == TB_NOT_SOON)
	{
		tb_timer_id = 0;
		return 0; // never - no longer scheduled
	}
	next_fire_time -= TBSystem::GetTimeMS();
	return (uint32)MAX(next_fire_time, 1.); // asap
}

/** Reschedule the platform timer, or cancel it if fire_time is TB_NOT_SOON.
	If fire_time is 0, it should be fired ASAP.
	If force is true, it will ask the platform to schedule it again, even if
	the fire_time is the same as last time. */
void TBSystem::RescheduleTimer(double fire_time)
{
	// cancel existing timer
	if(tb_timer_id)
	{
		s3eTimerCancelTimer((s3eCallback)tb_timer_callback, &tb_timer_id);
		tb_timer_id = 0;
	}

	// set new timer
	if(fire_time != TB_NOT_SOON)
	{
		double now = TBSystem::GetTimeMS();
		double delay = fire_time - now;
		s3eResult result = s3eResult::S3E_RESULT_ERROR;
		result = s3eTimerSetTimer((uint32)MAX(delay, 1.), (s3eCallback)tb_timer_callback, &tb_timer_id);
		if(result != s3eResult::S3E_RESULT_SUCCESS)
			TBDebugOut("ERROR: RescheduleTimer failed to s3eTimerSetTimer\n");
	}
}

int TBSystem::GetLongClickDelayMS()
{
	return 500;
}

int TBSystem::GetPanThreshold()
{
	return 5 * GetDPI() / 96;
}

int TBSystem::GetPixelsPerLine()
{
	return 40 * GetDPI() / 96;
}

int TBSystem::GetDPI()
{
	return 96;
	//return 262;
}

} // namespace tb

