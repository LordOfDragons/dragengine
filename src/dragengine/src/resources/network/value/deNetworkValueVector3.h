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

#ifndef _DENETWORKVALUEVECTOR3_H_
#define _DENETWORKVALUEVECTOR3_H_

#include "../../../common/math/decMath.h"
#include "deNetworkValue.h"


/**
 * \brief Network Value Vector-3 Class.
 *
 * Defines a vector-3 network value. Vectors are tuples with float
 * components. The Vector-3 contains three components.
 */
class deNetworkValueVector3 : public deNetworkValue{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValueVector3> Ref;
	
	
	
private:
	eValueFormats pFormat;
	decDVector pValue;
	double pPrecision;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValueVector3( eValueFormats format, const decDVector &value );
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValueVector3();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value format. */
	inline eValueFormats GetFormat() const{ return pFormat; }
	
	/** \brief Value. */
	inline const decDVector &GetVector() const{ return pValue; }
	
	/** \brief Set value. */
	void SetVector( const decDVector &value );
	
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
