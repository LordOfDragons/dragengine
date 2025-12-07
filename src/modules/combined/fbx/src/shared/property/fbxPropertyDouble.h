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

#ifndef _FBXPROPERTYDOUBLE_H_
#define _FBXPROPERTYDOUBLE_H_


#include "../fbxProperty.h"


/**
 * \brief FBX property double.
 */
class fbxPropertyDouble : public fbxProperty{
private:
	double pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxPropertyDouble();
	
	/** \brief Load property. */
	fbxPropertyDouble(decBaseFileReader &reader);
	
protected:
	/** \brief Clean up property. */
	virtual ~fbxPropertyDouble();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Value. */
	inline double GetValue() const{ return pValue; }
	
	/** \brief Set value. */
	void SetValue(double value);
	
	/** \brief Casting throwing exception if wrong type. */
	virtual fbxPropertyDouble &CastDouble();
	
	/** \brief Get values as specific type if possible. */
	virtual bool GetValueAsBool() const;
	virtual int GetValueAsInt() const;
	virtual int64_t GetValueAsLong() const;
	virtual float GetValueAsFloat() const;
	virtual double GetValueAsDouble() const;
	
	
	
	/** \brief Save to file. */
	virtual void Save(decBaseFileWriter &writer);
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure(deBaseModule &logger, const decString &prefix) const;
	/*@}*/
};

#endif
