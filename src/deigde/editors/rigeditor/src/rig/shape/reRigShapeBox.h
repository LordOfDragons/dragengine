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
#ifndef _RERIGSHAPEBOX_H_
#define _RERIGSHAPEBOX_H_

// includes
#include "reRigShape.h"



/**
 * @brief Rig Box Shape.
 * Working object for rig box shape.
 */
class reRigShapeBox : public reRigShape{
public:
	typedef deTObjectReference<reRigShapeBox> Ref;
	
private:
	decVector pHalfExtends;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig box shape. */
	reRigShapeBox(deEngine *engine);
	/** Cleans up the rig shape shape. */
protected:
	~reRigShapeBox() override;
public:
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the half extends. */
	inline const decVector &GetHalfExtends() const{ return pHalfExtends; }
	/** Sets the half extends. */
	void SetHalfExtends(const decVector &halfExtends);
	
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
