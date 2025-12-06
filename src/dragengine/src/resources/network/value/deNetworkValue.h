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
class DE_DLL_EXPORT deNetworkValue : public deObject{
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
