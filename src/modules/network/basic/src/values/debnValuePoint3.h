/* 
 * Drag[en]gine Basic Network Module
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

#ifndef _DEBNVALUEPOINT3_H_
#define _DEBNVALUEPOINT3_H_

#include <dragengine/common/math/decMath.h>

#include "debnValue.h"

class deNetworkValuePoint3;


/**
 * \brief Network Value Point-3 Class.
 * 
 * Defines a point-3 network value. Points are tuples with integer components.
 * The Point-3 contains three components.
 */
class debnValuePoint3 : public debnValue{
private:
	deNetworkValuePoint3 &pValuePoint3;
	decPoint3 pLastValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value. */
	debnValuePoint3( deNetworkValuePoint3 &valuePoint3 );
	
	/** \brief Clean up network value. */
	virtual ~debnValuePoint3();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Update value.
	 * \returns true if value needs to by synchronized otherwise false if not changed enough.
	 */
	virtual bool UpdateValue( bool force );
	
	/** \brief Read value from message. */
	virtual void ReadValue( decBaseFileReader &reader );
	
	/** \brief Write value to message. */
	virtual void WriteValue( decBaseFileWriter &writer );
	/*@}*/
};

#endif
