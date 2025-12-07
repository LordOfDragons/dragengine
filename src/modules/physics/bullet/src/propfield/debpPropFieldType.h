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

// include only once
#ifndef _DEBPPROPFIELDTYPE_H_
#define _DEBPPROPFIELDTYPE_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class debpPropField;

class dePropFieldType;

// definitions



/** Prop Field Bend State. */
struct debpPropFieldBendState{
	// this is now a bit crappy here. the random implementation of c++
	// left us behind. currently this is a hacked together and slightly
	// randomized ( due to the offset parameter ) sine curve. better
	// solution would be to use a global flucuation table and using one
	// single value to iterate through the table. this should provide
	// random enough behavior without costing too much time.
	decVector position;
	float flucDir;
	float flucStr;
};

/**
 * @brief Prop Field Type.
 *
 * Type in a prop field peer.
 */
class debpPropFieldType{
private:
	debpPropField *pPropField;
	dePropFieldType *pType;
	
	debpPropFieldBendState *pBendStates;
	int pBendStateCount;
	int pBendStateSize;
	
	float pFlucTimer;
	bool pDirty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new type. */
	debpPropFieldType(debpPropField *propField, dePropFieldType *type);
	/** Cleans up the type. */
	~debpPropFieldType();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of bend states. */
	inline int GetBendStateCount() const{ return pBendStateCount; }
	/** Sets the size of states keeping the state values intact where possible. */
	void SetBendStateSize(int count);
	/** Retrieves the instances. */
	inline debpPropFieldBendState *GetBendStates() const{ return pBendStates; }
	
	/** Marks the type dirty. */
	void MarkDirty();
	
	/** Updates the bend states. */
	void Update(float elapsed);
	/*@}*/
};

// end of include only once
#endif
