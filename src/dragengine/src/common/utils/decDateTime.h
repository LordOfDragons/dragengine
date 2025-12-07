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

#ifndef _DECDATETIME_H_
#define _DECDATETIME_H_

#include "../../dragengine_export.h"


/** \brief System time value. */
typedef long long TIME_SYSTEM;


/**
 * \brief Timestamp with date.
 */
class DE_DLL_EXPORT decDateTime{
private:
	int pYear;
	int pMonth;
	int pDay;
	int pHour;
	int pMinute;
	int pSecond;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty date time object. */
	decDateTime();
	
	/** \brief Create date time object set to system time. */
	decDateTime(const TIME_SYSTEM systemTime);
	
	/** \brief Create copy of date time object. */
	decDateTime(const decDateTime &dateTime);
	
	/** \brief Clean up date time object. */
	~decDateTime();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Year. */
	inline int GetYear() const{ return pYear; }
	
	/** \brief Set year. */
	void SetYear(int year);
	
	/** \brief Month starting with 0 for january. */
	inline int GetMonth() const{ return pMonth; }
	
	/** \brief Set month starting with 0 for january. */
	void SetMonth(int month);
	
	/** \brief Day starting with 0 for the first day in the month. */
	inline int GetDay() const{ return pDay; }
	
	/** \brief Set day starting with 0 for the first day in the month. */
	void SetDay(int day);
	
	/** \brief Hour. */
	inline int GetHour() const{ return pHour; }
	
	/** \brief Set hour. */
	void SetHour(int hour);
	
	/** \brief Minute. */
	inline int GetMinute() const{ return pMinute; }
	
	/** \brief Set minute. */
	void SetMinute(int minute);
	
	/** \brief Second. */
	inline int GetSecond() const{ return pSecond; }
	
	/** \brief Set second. */
	void SetSecond(int second);
	
	
	
	/** \brief Convert to system time. */
	TIME_SYSTEM ToSystemTime() const;
	
	/** \brief Set from system time. */
	void SetFrom(const TIME_SYSTEM systemTime);
	
	
	
	/** \brief Current system time. */
	static TIME_SYSTEM GetSystemTime();
	/*@}*/
};

#endif
