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
	~debnVisitorValueCreate() override;
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
	void VisitValue(deNetworkValue *value) override;
	/** Visits an integer network value. */
	void VisitInteger(deNetworkValueInteger *value) override;
	/** Visits a float network value. */
	void VisitFloat(deNetworkValueFloat *value) override;
	/** Visits a string network value. */
	void VisitString(deNetworkValueString *value) override;
	/** Visits a data network value. */
	void VisitData(deNetworkValueData *value) override;
	/** Visits a point-2 network value. */
	void VisitPoint2(deNetworkValuePoint2 *value) override;
	/** Visits a point-3 network value. */
	void VisitPoint3(deNetworkValuePoint3 *value) override;
	/** Visits a vector-2 network value. */
	void VisitVector2(deNetworkValueVector2 *value) override;
	/** Visits a vector-3 network value. */
	void VisitVector3(deNetworkValueVector3 *value) override;
	/** Visits a quaternion network value. */
	void VisitQuaternion(deNetworkValueQuaternion *value) override;
	/*@}*/
};

// end of include only once
#endif
