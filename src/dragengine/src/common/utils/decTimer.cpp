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
#ifdef OS_W32
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency( &frequency );
	pInvFrequency = 1.0 / ( double )frequency.QuadPart;
#endif

	Reset();
}

decTimer::~decTimer(){
}



// management
///////////////

void decTimer::Reset(){
#if defined OS_UNIX
	timeval vTime;
	gettimeofday( &vTime, NULL );
	pLastSec = vTime.tv_sec;
	pLastUSec = vTime.tv_usec;
	
#elif defined OS_W32
	LARGE_INTEGER curTime;
	QueryPerformanceCounter( &curTime );
	pLastTime = curTime.QuadPart;
#endif
}

float decTimer::GetElapsedTime(){
#if defined OS_UNIX
	timeval vTime;
	gettimeofday( &vTime, NULL );
	const suseconds_t diff = ( int )( vTime.tv_sec - pLastSec ) * 1000000 + ( ( int )vTime.tv_usec - ( int )pLastUSec );
	pLastSec = vTime.tv_sec;
	pLastUSec = vTime.tv_usec;
	return ( float )diff * 1e-6f; // / 1000000.0f;
	
#elif defined OS_W32
	LARGE_INTEGER curTime;
	QueryPerformanceCounter( &curTime );
	const LONGLONG diffTime = curTime.QuadPart - pLastTime;
	pLastTime = curTime.QuadPart;

	return ( float )( pInvFrequency * diffTime );
#endif
}

float decTimer::PeekElapsedTime(){
#if defined OS_UNIX
	timeval vTime;
	gettimeofday( &vTime, NULL );
	const suseconds_t diff = ( int )( vTime.tv_sec - pLastSec ) * 1000000 + ( ( int )vTime.tv_usec - ( int )pLastUSec );
	return ( float )diff * 1e-6f; // / 1000000.0f;
	
#elif defined OS_W32
	LARGE_INTEGER curTime;
	QueryPerformanceCounter( &curTime );
	const LONGLONG diffTime = curTime.QuadPart - pLastTime;

	return ( float )( pInvFrequency * diffTime );
#endif
}
