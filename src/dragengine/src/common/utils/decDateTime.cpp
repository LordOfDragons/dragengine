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

#include "../../dragengine_configuration.h"

#ifdef OS_UNIX
#include <sys/time.h>
#endif

#ifdef OS_W32
#include "../../app/include_windows.h"
#include <stdint.h>
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
	SetFrom(GetSystemTime());
}

decDateTime::decDateTime(const TIME_SYSTEM systemTime){
	SetFrom(systemTime);
}

decDateTime::decDateTime(const decDateTime &dateTime){
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

void decDateTime::SetYear(int year){
	pYear = year;
}

void decDateTime::SetMonth(int month){
	pMonth = month;
}

void decDateTime::SetDay(int day){
	pDay = day;
}

void decDateTime::SetHour(int hour){
	pHour = hour;
}

void decDateTime::SetMinute(int minute){
	pMinute = minute;
}

void decDateTime::SetSecond(int second){
	pSecond = second;
}



TIME_SYSTEM decDateTime::ToSystemTime() const{
	tm tmval;
	
	memset(&tmval, '\0', sizeof(tmval));
	tmval.tm_year = pYear - 1900;
	tmval.tm_mon = pMonth;
	tmval.tm_mday = pDay + 1;
	tmval.tm_hour = pHour;
	tmval.tm_min = pMinute;
	tmval.tm_sec = pSecond;
	
	const time_t mtime = mktime(&tmval);
	if(mtime == -1){
		DETHROW(deeInvalidParam);
	}
	
	return (TIME_SYSTEM)mtime;
}

void decDateTime::SetFrom(const TIME_SYSTEM systemTime){
	const time_t stime = (time_t)systemTime;
	tm tmval;
	
#ifdef OS_W32
	#ifdef OS_W32_VS
		localtime_s(&tmval, &stime);
	#else
		tmval = *localtime(&stime);
	#endif
#else
	if(!localtime_r(&stime, &tmval)){
		DETHROW(deeInvalidParam);
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
	if(gettimeofday(&tval, NULL) == -1){
		DETHROW(deeInvalidParam);
	}
	return (TIME_SYSTEM)tval.tv_sec;
#endif
	
#ifdef OS_W32
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 
	static const uint64_t epoch = (uint64_t)116444736000000000ULL;
	
	SYSTEMTIME systemTime;
	FILETIME fileTime;
	uint64_t time;
	
	::GetSystemTime(&systemTime);
	SystemTimeToFileTime(&systemTime, &fileTime);
	time = (uint64_t)fileTime.dwLowDateTime + ((uint64_t)fileTime.dwHighDateTime << 32);
	
	return (TIME_SYSTEM)((time - epoch) / 10000000L);
#endif
}
