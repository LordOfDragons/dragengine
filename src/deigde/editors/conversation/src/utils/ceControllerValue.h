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

#ifndef _CECONTROLLERVALUE_H_
#define _CECONTROLLERVALUE_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * \brief Controller Value.
 */
class ceControllerValue : public deObject{
private:
	int pController;
	float pValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new controller value. */
	ceControllerValue( int controller, float value );
	/** Cleans up the controller value. */
	virtual ~ceControllerValue();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the controller. */
	inline int GetController() const{ return pController; }
	/** Sets the controller. */
	void SetController( int controller );
	/** Retrieves the value. */
	inline float GetValue() const{ return pValue; }
	/** Sets the value. */
	void SetValue( float value );
	/*@}*/
};

#endif
