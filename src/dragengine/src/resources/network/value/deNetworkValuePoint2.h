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

#ifndef _DENETWORKVALUEPOINT2_H_
#define _DENETWORKVALUEPOINT2_H_

#include "../../../common/math/decMath.h"
#include "deNetworkValue.h"


/**
 * \brief Network Value Point-2 Class.
 *
 * Defines a point-2 network value. Points are tuples with integer
 * components. The Point-2 contains two components.
 */
class DE_DLL_EXPORT deNetworkValuePoint2 : public deNetworkValue{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deNetworkValuePoint2> Ref;
	
	
private:
	eValueFormats pFormat;
	decPoint pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value object. */
	deNetworkValuePoint2( eValueFormats format, const decPoint &value );
	
	/** \brief Clean up network value object. */
	virtual ~deNetworkValuePoint2();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Value format. */
	inline eValueFormats GetFormat() const{ return pFormat; }
	
	/** \brief Value. */
	inline const decPoint &GetPoint() const{ return pValue; }
	
	/** \brief Set value. */
	void SetPoint( const decPoint &value );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit network value. */
	virtual void Visit( deNetworkValueVisitor &visitor );
	/*@}*/
};

#endif
