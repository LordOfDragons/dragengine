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

#ifndef _FBXPROPERTYARRAYINTEGER_H_
#define _FBXPROPERTYARRAYINTEGER_H_


#include "../fbxProperty.h"


/**
 * \brief FBX property array integer.
 */
class fbxPropertyArrayInteger : public fbxProperty{
public:
	typedef deTObjectReference<fbxPropertyArrayInteger> Ref;
	
	
private:
	int *pValues;
	int pCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property. */
	fbxPropertyArrayInteger();
	
	/** \brief Load property. */
	explicit fbxPropertyArrayInteger(decBaseFileReader &reader);
	
protected:
	/** \brief Clean up property. */
	~fbxPropertyArrayInteger() override;
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Value count. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Value at index. */
	int GetValueAt(int index) const;
	
	/** \brief Add value. */
	void AddValue(int value);
	
	/** \brief Casting throwing exception if wrong type. */
	fbxPropertyArrayInteger &CastArrayInteger() override;
	
	/** \brief Get values as specific type if possible. */
	int GetValueCount() const override;
	bool GetValueAtAsBool(int index) const override;
	int GetValueAtAsInt(int index) const override;
	int64_t GetValueAtAsLong(int index) const override;
	float GetValueAtAsFloat(int index) const override;
	double GetValueAtAsDouble(int index) const override;
	
	
	
	/** \brief Save to file. */
	void Save(decBaseFileWriter &writer) override;
	
	/** \brief Debug print property structure. */
	virtual void DebugPrintStructure(deBaseModule &logger, const decString &prefix) const;
	/*@}*/
};

#endif
