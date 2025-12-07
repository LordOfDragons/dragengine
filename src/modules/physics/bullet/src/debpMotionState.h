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
#ifndef _DEBPMOTIONSTATE_H_
#define _DEBPMOTIONSTATE_H_

// includes
#include "LinearMath/btMotionState.h"
#include "dragengine/common/math/decMath.h"

// predefintions



/**
 * @brief Bullet Motion State.
 * Provides an implementation of the MotionState.
 */
class debpMotionState : public btMotionState{
private:
	decDVector pPosition;
	decVector pScaling;
	decQuaternion pOrientation;
	
public:
	// constructor, destructor
	debpMotionState();
	~debpMotionState();
	// management
	inline const decDVector &GetPosition() const{ return pPosition; }
	inline const decVector &GetScaling() const{ return pScaling; }
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	void SetPosition(const decDVector &position);
	void SetScaling(const decVector &scaling);
	void SetOrientation(const decQuaternion &orientation);
	// bullet management
	virtual void getWorldTransform(btTransform &centerOfMassWorldTrans) const;
	virtual void setWorldTransform(const btTransform &centerOfMassWorldTrans);
	virtual bool deactivationCallback(void *userPointer);
private:
	void pCleanUp();
};

// end of include only once
#endif
