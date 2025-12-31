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
#ifndef _RERIGSHAPECAPSULE_H_
#define _RERIGSHAPECAPSULE_H_

// includes
#include "reRigShape.h"



/**
 * @brief Rig Capsule Shape.
 * Working object for rig capsule shape.
 */
class reRigShapeCapsule : public reRigShape{
public:
	typedef deTObjectReference<reRigShapeCapsule> Ref;
	
private:
	float pTopRadius;
	float pBottomRadius;
	float pHalfHeight;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig capsule shape. */
	reRigShapeCapsule(deEngine *engine);
	/** Cleans up the rig shape shape. */
protected:
	~reRigShapeCapsule() override;
public:
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the half height. */
	inline float GetHalfHeight() const{ return pHalfHeight; }
	/** Sets the half height. */
	void SetHalfHeight(float halfHeight);
	/** Retrieves the top radius. */
	inline float GetTopRadius() const{ return pTopRadius; }
	/** Sets the top radius. */
	void SetTopRadius(float radius);
	/** Retrieves the bottom radius. */
	inline float GetBottomRadius() const{ return pBottomRadius; }
	/** Sets the bottom radius. */
	void SetBottomRadius(float radius);
	/** Sets both radi to the same value. */
	void SetRadius(float radius);
	
	/** Creates a copy of this shape. */
	reRigShape::Ref Duplicate() const override;
	/** Uniformly scale shape. */
	void Scale(float scale) override;
	
	/** Creates shape. */
	decShape *CreateShape() override;
	/*@}*/
};

// end of include only once
#endif
