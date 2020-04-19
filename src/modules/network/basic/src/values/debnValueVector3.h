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

#ifndef _DEBNVALUEVECTOR3_H_
#define _DEBNVALUEVECTOR3_H_

#include <dragengine/common/math/decMath.h>

#include "debnValue.h"

class deNetworkValueVector3;


/**
 * \brief Network Value Vector-3 Class.
 * 
 * Defines a vector-3 network value. Vectors are tuples with float components.
 * The Vector-3 contains three components.
 */
class debnValueVector3 : public debnValue{
private:
	deNetworkValueVector3 &pValueVector3;
	decDVector pLastValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value. */
	debnValueVector3( deNetworkValueVector3 &valueVector3 );
	
	/** \brief Clean up network value. */
	virtual ~debnValueVector3();
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
