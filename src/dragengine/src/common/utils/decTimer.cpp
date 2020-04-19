/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	gettimeofday( &vTime, NULL );
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
	gettimeofday( &vTime, NULL );
	diff = ( int )( vTime.tv_sec - pLastSec ) * 1000000 + ( ( int )vTime.tv_usec - ( int )pLastUSec );
	pLastSec = vTime.tv_sec;
	pLastUSec = vTime.tv_usec;
	return ( float )diff * 1e-6f; // / 1000000.0f;
#endif

#ifdef OS_W32
	DWORD curTime = timeGetTime();
	DWORD diff = curTime - pLastTime;
	pLastTime = curTime;
	return ( float )diff * 1e-3f; // / 1000.0f;
#endif
}
