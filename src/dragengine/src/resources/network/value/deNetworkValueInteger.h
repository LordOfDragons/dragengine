/* 
 * Drag[en]gine Game Engine
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

#ifndef _DENETWORKVALUEINTEGER_H_
#define _DENETWORKVALUEINTEGER_H_

#include <stdint.h>

#include "deNetworkValue.h"


/**
 * \brief Network Value Integer Class.
 *
 * Defines an integer network value.
 */
class deNetworkValueInteger : public deNetworkValue{
private:
	eValueFormats pFormat;
	int64_t pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValueInteger( eValueFormats format, int64_t value );
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValueInteger();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value format. */
	inline eValueFormats GetFormat() const{ return pFormat; }
	
	/** \brief Value. */
	inline int64_t GetInt() const{ return pValue; }
	
	/** \brief Set value. */
	void SetInt( int64_t value );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void Visit( deNetworkValueVisitor &visitor );
	/*@}*/
};

#endif
