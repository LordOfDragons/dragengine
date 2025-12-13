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
#ifndef _DEBNVISITORVALUECREATE_H_
#define _DEBNVISITORVALUECREATE_H_

// includes
#include <dragengine/resources/network/value/deNetworkValueVisitor.h>

// predefinitions
class debnValue;



/**
 * @brief Value Create Visitor.
 * Visitor to create a worker value able to track a value in a network
 * state. The created value is not held by the visitor.
 */
class debnVisitorValueCreate : public deNetworkValueVisitor{
private:
	debnValue *pValue;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new visitor. */
	debnVisitorValueCreate();
	/** Cleans up the visitor. */
	virtual ~debnVisitorValueCreate();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the created value. */
	inline debnValue *GetValue() const{ return pValue; }
	/** Resets the visitor. */
	void Reset();
	/*@}*/
	
	/** @name Visiting */
	/*@{*/
	/** Visits a network value. */
	virtual void VisitValue(deNetworkValue *value);
	/** Visits an integer network value. */
	virtual void VisitInteger(deNetworkValueInteger *value);
	/** Visits a float network value. */
	virtual void VisitFloat(deNetworkValueFloat *value);
	/** Visits a string network value. */
	virtual void VisitString(deNetworkValueString *value);
	/** Visits a data network value. */
	virtual void VisitData(deNetworkValueData *value);
	/** Visits a point-2 network value. */
	virtual void VisitPoint2(deNetworkValuePoint2 *value);
	/** Visits a point-3 network value. */
	virtual void VisitPoint3(deNetworkValuePoint3 *value);
	/** Visits a vector-2 network value. */
	virtual void VisitVector2(deNetworkValueVector2 *value);
	/** Visits a vector-3 network value. */
	virtual void VisitVector3(deNetworkValueVector3 *value);
	/** Visits a quaternion network value. */
	virtual void VisitQuaternion(deNetworkValueQuaternion *value);
	/*@}*/
};

// end of include only once
#endif
