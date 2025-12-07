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

#ifndef _DEPROPFIELDINSTANCE_H_
#define _DEPROPFIELDINSTANCE_H_

#include "../../common/math/decMath.h"


/**
 * \brief Prop Field Instance.
 *
 * Describes an instance of a prop in a prop field. These are blue
 * prints of the model of the parent type. Only the initial state
 * without influence from any forces is stored. The bend state
 * parameter points into the list of bend state where the actual
 * bend state is stored. More than one instance can use the same
 * bend state which allows the physics module to improve performance
 * avoiding costly calculations and lots of data transfer in memory.
 */
class DE_DLL_EXPORT dePropFieldInstance{
private:
	decVector pPosition;
	decVector pRotation;
	float pScaling;
	int pBendState;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new prop field instance. */
	dePropFieldInstance();
	
	/** \brief Clean up prop field instance. */
	~dePropFieldInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Initial position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set initial position. */
	void SetPosition(const decVector &position);
	
	/** \brief Initial rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set initial rotation. */
	void SetRotation(const decVector &rotation);
	
	/** \brief Scaling. */
	inline float GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling(float scaling);
	
	/** \brief Bend state index or -1 if not assigned. */
	inline int GetBendState() const{ return pBendState; }
	
	/** \brief Set bend state index or -1 if not assigned. */
	void SetBendState(int state);
	/*@}*/
};

#endif
