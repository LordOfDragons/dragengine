/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
class DE_DLL_EXPORT deNetworkValueQuaternion : public deNetworkValue{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValueQuaternion> Ref;
	
	
	
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
