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

#ifndef _DEBNPRELIABLETIMEOUT_H_
#define _DEBNPRELIABLETIMEOUT_H_

#include "../debnParameterFloat.h"


/**
 * Reliable timeout parameter.
 */
class debnPReliableTimeout : public debnParameterFloat{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates parameter. */
	debnPReliableTimeout( deNetworkBasic &network );
	/*@}*/
	
	
	
	/** \name Parameter Value */
	/*@{*/
	/** Current value. */
	virtual float GetParameterFloat();
	
	/** Set current value. */
	virtual void SetParameterFloat( float value );
	/*@}*/
};

#endif
