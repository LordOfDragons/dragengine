/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLEMPARAMETER_H_
#define _DECLEMPARAMETER_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>



/**
 * @brief Game Engine Module Parameter.
 */
class declEMParameter : public deObject{
private:
	int pIndex;
	decString pName;
	decString pDescription;
	decString pValue;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine module parameter. */
	declEMParameter();
	/** Cleans up the engine module parameter. */
	virtual ~declEMParameter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parameter index. */
	inline int GetIndex() const{ return pIndex; }
	/** Sets the parameter index. */
	void SetIndex( int index );
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the description. */
	inline const decString &GetDescription () const{ return pDescription; }
	/** Sets the description. */
	void SetDescription( const char *description );
	/** Retrieves the value. */
	inline const decString &GetValue() const{ return pValue; }
	/** Sets the value. */
	void SetValue( const char *value );
	/*@}*/
};

#endif
