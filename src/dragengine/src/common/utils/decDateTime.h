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
	decDateTime( const TIME_SYSTEM systemTime );
	
	/** \brief Create copy of date time object. */
	decDateTime( const decDateTime &dateTime );
	
	/** \brief Clean up date time object. */
	~decDateTime();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Year. */
	inline int GetYear() const{ return pYear; }
	
	/** \brief Set year. */
	void SetYear( int year );
	
	/** \brief Month starting with 0 for january. */
	inline int GetMonth() const{ return pMonth; }
	
	/** \brief Set month starting with 0 for january. */
	void SetMonth( int month );
	
	/** \brief Day starting with 0 for the first day in the month. */
	inline int GetDay() const{ return pDay; }
	
	/** \brief Set day starting with 0 for the first day in the month. */
	void SetDay( int day );
	
	/** \brief Hour. */
	inline int GetHour() const{ return pHour; }
	
	/** \brief Set hour. */
	void SetHour( int hour );
	
	/** \brief Minute. */
	inline int GetMinute() const{ return pMinute; }
	
	/** \brief Set minute. */
	void SetMinute( int minute );
	
	/** \brief Second. */
	inline int GetSecond() const{ return pSecond; }
	
	/** \brief Set second. */
	void SetSecond( int second );
	
	
	
	/** \brief Convert to system time. */
	TIME_SYSTEM ToSystemTime() const;
	
	/** \brief Set from system time. */
	void SetFrom( const TIME_SYSTEM systemTime );
	
	
	
	/** \brief Current system time. */
	static TIME_SYSTEM GetSystemTime();
	/*@}*/
};

#endif
