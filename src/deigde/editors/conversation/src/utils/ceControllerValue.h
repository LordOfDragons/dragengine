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
 * Controller Value.
 */
class ceControllerValue : public deObject{
private:
	int pControllerIndex;
	decString pController;
	float pValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create controller value. */
	ceControllerValue( int controller, float value );
	ceControllerValue( const char *controller, float value );
	
	/** Cleans up the controller value. */
	virtual ~ceControllerValue();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Controller. */
	inline int GetControllerIndex() const{ return pControllerIndex; }
	
	/** Set controller. */
	void SetControllerIndex( int controller );
	
	/** Controller. */
	inline const decString &GetController() const{ return pController; }
	
	/** Set controller. */
	void SetController( const char *controller );
	
	/** Value. */
	inline float GetValue() const{ return pValue; }
	
	/** Set value. */
	void SetValue( float value );
	/*@}*/
};

#endif
