/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CESTRIP_H_
#define _CESTRIP_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * \brief Conversation action strip.
 */
class ceStrip : public deObject{
private:
	decString pID;
	float pPause;
	float pDuration;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new empty strip. */
	ceStrip();
	/** \brief Creates a new strip. */
	ceStrip( const char *id, float duration, float pause );
	/** \brief Creates a new strip. */
	ceStrip( const ceStrip &strip );
	/** \brief Cleans up the strip. */
	virtual ~ceStrip();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the identifier. */
	inline const decString &GetID() const{ return pID; }
	/** \brief Sets the identifier. */
	void SetID( const char *id );
	/** \brief Retrieves the pause in seconds. */
	inline float GetPause() const{ return pPause; }
	/** \brief Sets the pause in seconds. */
	void SetPause( float pause );
	/** \brief Retrieves the duration in seconds. */
	inline float GetDuration() const{ return pDuration; }
	/** \brief Sets the duration in seconds. */
	void SetDuration( float duration );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Set strip from another strip. */
	ceStrip &operator=( const ceStrip &strip );
	/** \brief Determines if two strips are equal. */
	bool operator==( const ceStrip &strip ) const;
	/** \brief Determines if two strips are not equal. */
	bool operator!=( const ceStrip &strip ) const;
	/*@}*/
};

#endif
