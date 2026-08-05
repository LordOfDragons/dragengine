/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEBPSHAPEHULL_H_
#define _DEBPSHAPEHULL_H_

#include "debpShape.h"
#include "../coldet/collision/debpDCollisionHull.h"

class decShapeHull;


/**
 * Bullet counterpart of hull shape.
 */
class debpShapeHull : public debpShape{
public:
	using Ref = deTObjectReference<debpShapeHull>;
	
	
private:
	decShapeHull *pSHull;
	debpDCollisionHull pCHull;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shape. */
	debpShapeHull(decShapeHull *shape);
	
protected:
	/** Clean up shape. */
	~debpShapeHull() override;
	/*@}*/
	
	
public:
	/** @name Management */
	/*@{*/
	/** Hull shape. */
	inline decShapeHull *GetShapeHull() const{ return pSHull; }
	
	/** Collision hull. */
	inline debpDCollisionHull &GetCollisionHull(){ return pCHull; }
	inline const debpDCollisionHull &GetCollisionHull() const{ return pCHull; }
	
	/** Update collision volume using transformation matrix. */
	void UpdateWithMatrix(const decDMatrix &transformation, const decDVector &scale) override;
	
	/** Print out on console some debugging information about shape. */
	void PrintDebug(dePhysicsBullet &module) override;
	/*@}*/
};

#endif
