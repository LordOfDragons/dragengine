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

#ifndef _DEPROPFIELDBENDSTATE_H_
#define _DEPROPFIELDBENDSTATE_H_

#include "../../common/math/decMath.h"


/**
 * \brief Prop Field Bend State.
 *
 * Stores the bending state of one or more instances.
 */
class DE_DLL_EXPORT dePropFieldBendState{
private:
	float pBendX;
	float pBendZ;
	float pVeloX;
	float pVeloZ;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new bend state. */
	dePropFieldBendState();
	
	/** \brief Clean up prop bend state. */
	~dePropFieldBendState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bending around the X axis. */
	inline float GetBendX() const{ return pBendX; }
	
	/** \brief Set bending around the X axis. */
	void SetBendX( float bend );
	
	/** \brief Bending around the Z axis. */
	inline float GetBendZ() const{ return pBendZ; }
	
	/** \brief Set bending around the Z axis. */
	void SetBendZ( float bend );
	
	/** \brief Bending velocity around the X axis. */
	inline float GetVelocityX() const{ return pVeloX; }
	
	/** \brief Set bending velocity around the X axis. */
	void SetVelocityX( float bend );
	
	/** \brief Bending velocity around the Z axis. */
	inline float GetVelocityZ() const{ return pVeloZ; }
	
	/** \brief Set bending velocity around the Z axis. */
	void SetVelocityZ( float bend );
	/*@}*/
};

#endif
