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

#ifndef _DENETWORKVALUEDATA_H_
#define _DENETWORKVALUEDATA_H_

#include <stdint.h>

#include "deNetworkValue.h"


/**
 * \brief Network Value Data Class.
 *
 * Defines a data network value. Stores in contrary to the string network
 * value a data buffer of fixed length and is not zero padded. This kind
 * of network value is useful for arbitrary data typically of files or
 * other complex data.
 */
class deNetworkValueData : public deNetworkValue{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValueData> Ref;
	
	
	
private:
	uint8_t *pData;
	int pLength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValueData( int length );
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValueData();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Data pointer. */
	inline uint8_t *GetData() const{ return pData; }
	
	/** \brief Length of the data. */
	inline int GetLength() const{ return pLength; }
	
	/**
	 * \brief Set length of data.
	 * 
	 * After changing the size the data is undefined. Only values greater than 0 are allowed.
	 */
	void SetLength( int length );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void Visit( deNetworkValueVisitor &visitor );
	/*@}*/
};

#endif
