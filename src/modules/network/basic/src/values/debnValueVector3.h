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
	debnValueVector3(deNetworkValueVector3 &valueVector3);
	
	/** \brief Clean up network value. */
	virtual ~debnValueVector3();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Update value.
	 * \returns true if value needs to by synchronized otherwise false if not changed enough.
	 */
	virtual bool UpdateValue(bool force);
	
	/** \brief Read value from message. */
	virtual void ReadValue(decBaseFileReader &reader);
	
	/** \brief Write value to message. */
	virtual void WriteValue(decBaseFileWriter &writer);
	/*@}*/
};

#endif
