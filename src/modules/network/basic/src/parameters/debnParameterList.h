/* 
 * Drag[en]gine Basic Network Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEBNPARAMETERLIST_H_
#define _DEBNPARAMETERLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class debnParameter;


/**
 * Parameter list.
 */
class debnParameterList{
private:
	decPointerList pParameters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter list. */
	debnParameterList();
	
	/** Clean up parameter list. */
	~debnParameterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of parameters. */
	int GetParameterCount() const;
	
	/** Index of named parameter or -1 if absent. */
	int IndexOfParameterNamed( const char *name ) const;
	
	/** Parameter at index. */
	debnParameter &GetParameterAt( int index ) const;
	
	/** Named parameter. */
	debnParameter &GetParameterNamed( const char *name ) const;
	
	/** Add parameter. */
	void AddParameter( debnParameter *parameter );
	
	/** Remove all parameters. */
	void RemoveAllParameters();
	/*@}*/
};

#endif
