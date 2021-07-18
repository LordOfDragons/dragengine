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

#ifndef _DENETWORKVALUE_H_
#define _DENETWORKVALUE_H_

class deNetworkValueVisitor;

#include "../../../deObject.h"


/**
 * \brief Network Value Class.
 *
 * Defines a value in a network state. Network state properties are
 * defined by a name that is unique inside the parent network state.
 * The type indicates what kind of value is stored in the value. A
 * set of typical types is provided which should be enough to represent
 * states across the network. In addtion each network state value
 * has also a predictor assigned. Predictors are used to cut down on
 * the number of packets send over the network by predicting the upcoming
 * value of a value according to rules.
 */
class deNetworkValue : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValue> Ref;
	
	
	
public:
	/** \brief Value formats. */
	enum eValueFormats{
		/** \brief 8-Bit signed integer. */
		evfSInt8,
		
		/** \brief 8-Bit unsigned integer. */
		evfUInt8,
		
		/** \brief 16-Bit signed integer. */
		evfSInt16,
		
		/** \brief 16-Bit unsigned integer. */
		evfUInt16,
		
		/** \brief 32-Bit signed integer. */
		evfSInt32,
		
		/** \brief 32-Bit unsigned integer. */
		evfUInt32,
		
		/** \brief 64-Bit signed integer. */
		evfSInt64,
		
		/** \brief 64-Bit unsigned integer. */
		evfUInt64,
		
		/** \brief 16-Bit float. */
		evfFloat16,
		
		/** \brief 32-Bit float. */
		evfFloat32,
		
		/** \brief 64-Bit float. */
		evfFloat64
	};
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValue();
	
protected:
	/**
	 * \brief Clean up network value object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deNetworkValue();
	/*@}*/
	
	
	
public:
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void Visit( deNetworkValueVisitor &visitor );
	/*@}*/
};

#endif
