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
	decDateTime( const TIME_SYSTEM systemTime );
	/** \brief Creates a copy of a date time object. */
	decDateTime( const decDateTime &dateTime );
	/** \brief Cleans up a date time object. */
	~decDateTime();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the year. */
	inline int GetYear() const{ return pYear; }
	/** \brief Sets the year. */
	void SetYear( int year );
	/** \brief Retrieves the month starting with 0 for january. */
	inline int GetMonth() const{ return pMonth; }
	/** \brief Sets the month starting with 0 for january. */
	void SetMonth( int month );
	/** \brief Retrieves the day starting with 0 for the first day in the month. */
	inline int GetDay() const{ return pDay; }
	/** \brief Sets the day starting with 0 for the first day in the month. */
	void SetDay( int day );
	/** \brief Retrieves the hour. */
	inline int GetHour() const{ return pHour; }
	/** \brief Sets the hour. */
	void SetHour( int hour );
	/** \brief Retrieves the minute. */
	inline int GetMinute() const{ return pMinute; }
	/** \brief Sets the minute. */
	void SetMinute( int minute );
	/** \brief Retrieves the second. */
	inline int GetSecond() const{ return pSecond; }
	/** \brief Sets the second. */
	void SetSecond( int second );
	
	/** \brief Retrieves the system time for this date and time. */
	TIME_SYSTEM ToSystemTime() const;
	/** \brief Sets the date time from a system time. */
	void SetFrom( const TIME_SYSTEM systemTime );
	
	/** \brief Retrieves the current system time. */
	static TIME_SYSTEM GetSystemTime();
	/*@}*/
};

#endif
