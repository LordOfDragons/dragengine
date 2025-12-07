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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "decTimer.h"



// class decTimer
///////////////////

// constructor, destructor
////////////////////////////

decTimer::decTimer(){
	Reset();
}

decTimer::~decTimer(){
}



// management
///////////////

void decTimer::Reset(){
#ifdef OS_UNIX
	timeval vTime;
	gettimeofday(&vTime, NULL);
	pLastSec = vTime.tv_sec;
	pLastUSec = vTime.tv_usec;
#endif

#ifdef OS_W32
	pLastTime = timeGetTime();
#endif
}

float decTimer::GetElapsedTime(){
#ifdef OS_UNIX
	timeval vTime;
	suseconds_t diff;
	gettimeofday(&vTime, NULL);
	diff = (int)(vTime.tv_sec - pLastSec) * 1000000 + ((int)vTime.tv_usec - (int)pLastUSec);
	pLastSec = vTime.tv_sec;
	pLastUSec = vTime.tv_usec;
	return (float)diff * 1e-6f; // / 1000000.0f;
#endif

#ifdef OS_W32
	DWORD curTime = timeGetTime();
	DWORD diff = curTime - pLastTime;
	pLastTime = curTime;
	return (float)diff * 1e-3f; // / 1000.0f;
#endif
}
