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

#ifndef _DECXMLCHARREFERENCE_H_
#define _DECXMLCHARREFERENCE_H_

#include "decXmlElement.h"
#include "../string/decString.h"



/**
 * \brief XML Character Reference.
 */
class decXmlCharReference : public decXmlElement{
public:
	/** \brief Radix enumeration. */
	enum eRadices{
		/** \brief Decimal radix. */
		erDecimal,
		
		/** \brief Hexadecimal radix. */
		erHexadecimal
	};
	
	
	
private:
	decString pData;
	eRadices pRadix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create xml character reference wwith the given data and radix.
	 * \param data Character Data
	 * \param radix Value from eRadices
	 */
	decXmlCharReference(const char *data, eRadices radix);
	
protected:
	/** \brief Clean up xml character data. */
	~decXmlCharReference() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Data. */
	inline const decString &GetData() const{ return pData; }
	
	/** \brief Radix. */
	inline eRadices GetRadix() const{ return pRadix; }
	
	/** \brief Is decimal. */
	inline bool IsDecimal() const{ return pRadix == erDecimal; }
	
	/** \brief Is hexadecimal. */
	inline bool IsHexadecimal() const{ return pRadix == erHexadecimal; }
	
	/** \brief Set data. */
	void SetData(const char *data);
	
	/** \brief Set radix. */
	void SetRadix(eRadices radix);
	/*@}*/
	
	
	
	/** \name Visiting */
	void Visit(decXmlVisitor &visitor) override;
	/*@}*/
	
	
	
	/** \name Casting */
	/*@{*/
	bool CanCastToCharReference() const override;
	decXmlCharReference *CastToCharReference() override;
	/*@}*/
};

#endif
