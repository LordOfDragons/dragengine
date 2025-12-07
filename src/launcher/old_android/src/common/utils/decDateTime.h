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


/** \brief System time value. */
typedef long long TIME_SYSTEM;



/**
 * \brief Timestamp with date.
 */
class decDateTime{
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
	/** \brief Creates an empty date time object. */
	decDateTime();
	/** \brief Creates a date time object set to a system time. */
	decDateTime(const TIME_SYSTEM systemTime);
	/** \brief Creates a copy of a date time object. */
	decDateTime(const decDateTime &dateTime);
	/** \brief Cleans up a date time object. */
	~decDateTime();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the year. */
	inline int GetYear() const{return pYear;}
	/** \brief Sets the year. */
	void SetYear(int year);
	/** \brief Retrieves the month starting with 0 for january. */
	inline int GetMonth() const{return pMonth;}
	/** \brief Sets the month starting with 0 for january. */
	void SetMonth(int month);
	/** \brief Retrieves the day starting with 0 for the first day in the month. */
	inline int GetDay() const{return pDay;}
	/** \brief Sets the day starting with 0 for the first day in the month. */
	void SetDay(int day);
	/** \brief Retrieves the hour. */
	inline int GetHour() const{return pHour;}
	/** \brief Sets the hour. */
	void SetHour(int hour);
	/** \brief Retrieves the minute. */
	inline int GetMinute() const{return pMinute;}
	/** \brief Sets the minute. */
	void SetMinute(int minute);
	/** \brief Retrieves the second. */
	inline int GetSecond() const{return pSecond;}
	/** \brief Sets the second. */
	void SetSecond(int second);
	
	/** \brief Retrieves the system time for this date and time. */
	TIME_SYSTEM ToSystemTime() const;
	/** \brief Sets the date time from a system time. */
	void SetFrom(const TIME_SYSTEM systemTime);
	
	/** \brief Retrieves the current system time. */
	static TIME_SYSTEM GetSystemTime();
	/*@}*/
};

#endif
