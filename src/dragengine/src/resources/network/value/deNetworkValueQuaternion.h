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

#ifndef _DENETWORKVALUEQUATERNION_H_
#define _DENETWORKVALUEQUATERNION_H_

#include "../../../common/math/decMath.h"
#include "deNetworkValue.h"


/**
 * \brief Network Value Quaternion Class.
 *
 * Defines a quaternion network value. Quaternions are tuples with float
 * components. The quaternion contains four components.
 */
class deNetworkValueQuaternion : public deNetworkValue{
private:
	eValueFormats pFormat;
	decQuaternion pValue;
	double pPrecision;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValueQuaternion( eValueFormats format, const decQuaternion &value );
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValueQuaternion();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value format. */
	inline eValueFormats GetFormat() const{ return pFormat; }
	
	/** \brief Value. */
	inline const decQuaternion &GetQuaternion() const{ return pValue; }
	
	/** \brief Set value. */
	void SetQuaternion( const decQuaternion &value );
	
	/** \brief Precision. */
	inline double GetPrecision() const{ return pPrecision; }
	
	/** \brief Set precision. */
	void SetPrecision( double precision );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void Visit( deNetworkValueVisitor &visitor );
	/*@}*/
};

#endif
