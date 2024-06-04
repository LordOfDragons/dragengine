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
#ifndef _MEHTVINSTANCE_H_
#define _MEHTVINSTANCE_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions



/**
 * @brief Height Terrain Vegetation Instance.
 *
 * Instance of a vegetation prop to be used later on in a prop field.
 */
class meHTVInstance{
private:
	short pVLayer;
	short pVariation;
	decVector pPosition;
	decVector pRotation;
	float pScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new instance. */
	meHTVInstance();
	/** Cleans up the instance. */
	~meHTVInstance();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the vegetation layer index. */
	inline short GetVLayer() const{ return pVLayer; }
	/** Retrieves the vegetation variation index. */
	inline short GetVariation() const{ return pVariation; }
	/** Sets the vegetation layer and variation index. */
	void SetVLayer( int vlayer, int variation );
	
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decVector &position );
	/** Retrieves the rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	/** Sets the rotation. */
	void SetRotation( const decVector &rotation );
	/** Retrieves the scaling. */
	inline float GetScaling() const{ return pScaling; }
	/** Sets the scaling. */
	void SetScaling( float scaling );
	/** Sets all parameters at once. */
	void SetParameters( int vlayer, int variation, const decVector &position, const decVector &rotation, float scaling );
	/*@}*/
};

// end of include only once
#endif
