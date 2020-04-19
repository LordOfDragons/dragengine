/* 
 * Drag[en]gine Android Launcher
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

#include "../../dragengine_configuration.h"

#ifdef OS_UNIX
#include <sys/time.h>
#endif

#ifdef OS_W32
#include "../../app/include_windows.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "decDateTime.h"
#include "../exceptions.h"



// class decDateTime
//////////////////////

// constructor, destructor
////////////////////////////

decDateTime::decDateTime(){
	SetFrom( GetSystemTime() );
}

decDateTime::decDateTime( const TIME_SYSTEM systemTime ){
	SetFrom( systemTime );
}

decDateTime::decDateTime( const decDateTime &dateTime ){
	pYear = dateTime.pYear;
	pMonth = dateTime.pMonth;
	pDay = dateTime.pDay;
	pHour = dateTime.pHour;
	pMinute = dateTime.pMinute;
	pSecond = dateTime.pSecond;
}

decDateTime::~decDateTime(){
}



// management
///////////////

void decDateTime::SetYear( int year ){
	pYear = year;
}

void decDateTime::SetMonth( int month ){
	pMonth = month;
}

void decDateTime::SetDay( int day ){
	pDay = day;
}

void decDateTime::SetHour( int hour ){
	pHour = hour;
}

void decDateTime::SetMinute( int minute ){
	pMinute = minute;
}

void decDateTime::SetSecond( int second ){
	pSecond = second;
}



TIME_SYSTEM decDateTime::ToSystemTime() const{
	tm tmval;
	
	memset( &tmval, '\0', sizeof( tmval ) );
	tmval.tm_year = pYear - 1900;
	tmval.tm_mon = pMonth;
	tmval.tm_mday = pDay + 1;
	tmval.tm_hour = pHour;
	tmval.tm_min = pMinute;
	tmval.tm_sec = pSecond;
	
	const time_t mtime = mktime( &tmval );
	if( mtime == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	return ( TIME_SYSTEM )mtime;
}

void decDateTime::SetFrom( const TIME_SYSTEM systemTime ){
	const time_t stime = ( time_t )systemTime;
	tm tmval;
	
#ifdef OS_W32
	tmval = *localtime( &stime );
#else
	if( ! localtime_r( &stime, &tmval ) ){
		DETHROW( deeInvalidParam );
	}
#endif // OS_W32
	
	pYear = 1900 + tmval.tm_year;
	pMonth = tmval.tm_mon;
	pDay = tmval.tm_mday - 1;
	pHour = tmval.tm_hour;
	pMinute = tmval.tm_min;
	pSecond = tmval.tm_sec;
}



TIME_SYSTEM decDateTime::GetSystemTime(){
#ifdef OS_UNIX
	timeval tval;
	if( gettimeofday( &tval, NULL ) == -1 ){
		DETHROW( deeInvalidParam );
	}
	return ( TIME_SYSTEM )tval.tv_sec;
#endif
	
#ifdef OS_W32
	return ( TIME_SYSTEM )timeGetTime();
#endif
}
