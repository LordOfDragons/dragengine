/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new instance. */
	meHTVInstance();
	/** Cleans up the instance. */
	~meHTVInstance();
	/*@}*/
	
	/** @name Management */
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
