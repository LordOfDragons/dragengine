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

#ifndef _CEPLAYBACKVARIABLE_H_
#define _CEPLAYBACKVARIABLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Playback Variable.
 */
class cePlaybackVariable : public deObject{
private:
	decString pName;
	int pValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new variable. */
	cePlaybackVariable( const char *name, int value );
	/** Cleans up the variable. */
	virtual ~cePlaybackVariable();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the value. */
	inline int GetValue() const{ return pValue; }
	/** Sets the value. */
	void SetValue( int value );
	/*@}*/
};

#endif
