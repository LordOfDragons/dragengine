/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLPARAMETERLIST_H_
#define _DEOGLPARAMETERLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deoglParameter;



/**
 * Parameter list.
 */
class deoglParameterList{
private:
	decPointerList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter list. */
	deoglParameterList();
	
	/** Clean up parameter list. */
	~deoglParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of parameters. */
	int GetParameterCount() const;
	
	/** Index of named parameter or -1 if absent. */
	int IndexOfParameterNamed( const char *name ) const;
	
	/** Parameter at index. */
	deoglParameter &GetParameterAt( int index ) const;
	
	/** Named parameter. */
	deoglParameter &GetParameterNamed( const char *name ) const;
	
	/** Add parameter. */
	void AddParameter( deoglParameter *parameter );
	
	/** Remove all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif
