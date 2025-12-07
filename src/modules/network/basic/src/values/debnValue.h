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

#ifndef _DEBNVALUE_H_
#define _DEBNVALUE_H_

#include "../deNetworkBasic.h"

class deNetworkMessage;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Network value.
 */
class debnValue{
private:
	eValueTypes pDataType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create network value. */
	debnValue();
	
	/** \brief Clean up network value. */
	virtual ~debnValue();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Data  type. */
	inline eValueTypes GetDataType() const{return pDataType;}
	
	/** \brief Set data type. */
	void SetDataType(eValueTypes dataType);
	
	/**
	 * \brief Update value.
	 * \returns true if value needs to by synchronized otherwise false if not changed enough.
	 */
	virtual bool UpdateValue(bool force) = 0;
	
	/** \brief Read value from message. */
	virtual void ReadValue(decBaseFileReader &reader) = 0;
	
	/** \brief Write value to message. */
	virtual void WriteValue(decBaseFileWriter &writer) = 0;
	/*@}*/
};

#endif
