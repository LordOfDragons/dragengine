/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPDEBUGINFORMATION_H_
#define _DEBPDEBUGINFORMATION_H_

#include "debpDebugInformationList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Debug information for performance profiling.
 * 
 * Stores an elapsed time in seconds and a counter value. A name is used to display
 * the information on screen. The font and background color can be set to help identify
 * the information on screen quickly. Debug information can be disabled in which case
 * the font is half-way faded to the background and the values set to dashes. Information
 * can be groups containing child information. If child information exist and an elapsed
 * time is present the child information calculate a ratio of their elapsed time compared
 * to this elapsed time to find better bottle necks.
 * 
 * Debug information be be set invisible in which case they are not drawn. Avoids the
 * need to add and remove debug information all time.
 */
class debpDebugInformation : public deObject{
private:
	decString pName;
	float pElapsedTime;
	float pElapsedTimeAccum;
	int pCounter;
	bool pHasElapsedTime;
	bool pHasCounter;
	decString pText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create debug information. */
	debpDebugInformation( const char *name );
	
	/** \brief Clean up debug information. */
	virtual ~debpDebugInformation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Clear elapsed time and counter. */
	void Clear();
	
	
	
	/** \brief Elapsed time. */
	inline float GetElapsedTime() const{ return pElapsedTime; }
	
	/** \brief Set elapsed time. */
	void SetElapsedTime( float time );
	
	/** \brief Increment elapsed time. */
	void IncrementElapsedTime( float time );
	
	/** \brief Clear elapsed time. */
	void ClearElapsedTime();
	
	/** \brief Has elapsed time. */
	inline bool HasElapsedTime() const{ return pHasElapsedTime; }
	
	
	
	/** \brief Counter. */
	inline int GetCounter() const{ return pCounter; }
	
	/** \brief Set counter. */
	void SetCounter( int counter );
	
	/** \brief Increment counter by one. */
	void IncrementCounter();
	
	/** \brief Increment counter. */
	void IncrementCounter( int count );
	
	/** \brief Clear counter. */
	void ClearCounter();
	
	/** \brief Has counter. */
	inline bool HasCounter() const{ return pHasCounter; }
	
	
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText( const char *text );
	/*@}*/
};

#endif
