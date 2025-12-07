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

#ifndef _FBXPROPERTYARRAYFLOAT_H_
#define _FBXPROPERTYARRAYFLOAT_H_


#include "../fbxProperty.h"


/**
 * \brief FBX property array integer.
 */
class fbxPropertyArrayFloat : public fbxProperty{
private:
	float *pValues;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxPropertyArrayFloat();
	
	/** \brief Load property. */
	fbxPropertyArrayFloat(decBaseFileReader &reader);
	
protected:
	/** \brief Clean up property. */
	virtual ~fbxPropertyArrayFloat();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Value count. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Value at index. */
	float GetValueAt(int index) const;
	
	/** \brief Add value. */
	void AddValue(float value);
	
	/** \brief Casting throwing exception if wrong type. */
	virtual fbxPropertyArrayFloat &CastArrayFloat();
	
	/** \brief Get values as specific type if possible. */
	virtual int GetValueCount() const;
	virtual bool GetValueAtAsBool(int index) const;
	virtual int GetValueAtAsInt(int index) const;
	virtual int64_t GetValueAtAsLong(int index) const;
	virtual float GetValueAtAsFloat(int index) const;
	virtual double GetValueAtAsDouble(int index) const;
	
	/** \brief Read sequence of values as composed type. */
	virtual decVector2 GetValueAtAsVector2(int index) const;
	virtual decVector GetValueAtAsVector(int index) const;
	virtual decMatrix GetValueAtAsMatrix(int index) const;
	
	
	
	/** \brief Save to file. */
	virtual void Save(decBaseFileWriter &writer);
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure(deBaseModule &logger, const decString &prefix) const;
	/*@}*/
};

#endif
