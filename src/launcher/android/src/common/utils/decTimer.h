/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// include only once
#ifndef _DECTIMER_H_
#define _DECTIMER_H_

#include "../../dragengine_configuration.h"

#ifdef OS_UNIX
#include <sys/time.h>
#ifndef suseconds_t
#define suseconds_t time_t
#endif
#endif

#ifdef OS_W32
#include "../../app/include_windows.h"
#endif


/**
 * @brief Simple timer.
 *
 */
class decTimer{
private:
#if defined( OS_UNIX )
	time_t pLastSec;
	suseconds_t pLastUSec;
#elif defined( OS_W32 )
	DWORD pLastTime;
#endif
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new timer object. */
	decTimer();
	/** Cleans up timer object. */
	~decTimer();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Resets the timer. */
	void Reset();
	/** Retrieves the time elapsed in seconds since the last reset. */
	float GetElapsedTime();
	/*@}*/
};

// end of include only once
#endif
