/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLGPMPARAMETERLIST_H_
#define _DEGLGPMPARAMETERLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class deglGPMParameter;



/**
 * @brief Game Profile Module Parameter List.
 */
class deglGPMParameterList{
private:
	decObjectList pParameters;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game profile parameter list. */
	deglGPMParameterList();
	/** Cleans up the game profile parameter list. */
	~deglGPMParameterList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of parameters. */
	int GetParameterCount() const;
	/** Retrieves the parameter at the given position. */
	deglGPMParameter *GetParameterAt( int index ) const;
	/** Retrieves the parameter with the given name or NULL if not found. */
	deglGPMParameter *GetParameterNamed( const char *name ) const;
	/** Determines if a parameter exists. */
	bool HasParameter( deglGPMParameter *parameter ) const;
	/** Determines if a parameter with the given name exists. */
	bool HasParameterNamed( const char *name ) const;
	/** Retrieves the index of a parameter or -1 if not found. */
	int IndexOfParameter( deglGPMParameter *parameter ) const;
	/** Retrieves the index of a parameter with the given name or -1 if not found. */
	int IndexOfParameterNamed( const char *name ) const;
	/** Adds a parameter. */
	void AddParameter( deglGPMParameter *parameter );
	/** Removes a parameter. */
	void RemoveParameter( deglGPMParameter *parameter );
	/** Removes all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif // _DEGLGAME_H_
