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

#ifndef _DENETWORKVALUEVECTOR2_H_
#define _DENETWORKVALUEVECTOR2_H_

#include "../../../common/math/decMath.h"
#include "deNetworkValue.h"


/**
 * \brief Network Value Vector-2 Class.
 *
 * Defines a vector-2 network value. Vectors are tuples with float
 * components. The Vector-2 contains two components.
 */
class DE_DLL_EXPORT deNetworkValueVector2 : public deNetworkValue{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValueVector2> Ref;
	
	
private:
	eValueFormats pFormat;
	decVector2 pValue;
	double pPrecision;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValueVector2( eValueFormats format, const decVector2 &value );
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValueVector2();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value format. */
	inline eValueFormats GetFormat() const{ return pFormat; }
	
	/** \brief Value. */
	inline const decVector2 &GetVector() const{ return pValue; }
	
	/** \brief Set value. */
	void SetVector( const decVector2 &value );
	
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
