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

#ifndef _DEBNPARAMETERINT_H_
#define _DEBNPARAMETERINT_H_

#include "debnParameter.h"


/**
 * Base class for integer parameters. Every parameter stores information about
 * the parameter itself and provides methods to retrieves or alter the current value.
 */
class debnParameterInt : public debnParameter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter. */
	debnParameterInt( deNetworkBasic &network );
	/*@}*/
	
	
	
	/** \name Parameter Value */
	/*@{*/
	/** Current value. */
	virtual decString GetParameterValue();
	
	/** Set current value. */
	virtual void SetParameterValue( const char *value );
	
	/** Current value. */
	virtual int GetParameterInt() = 0;
	
	/** Set current value. */
	virtual void SetParameterInt( int value ) = 0;
	/*@}*/
};

#endif
