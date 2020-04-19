/* 
 * Drag[en]gine Game Engine
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
class dePropFieldInstance{
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
	void SetPosition( const decVector &position );
	
	/** \brief Initial rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	
	/** \brief Set initial rotation. */
	void SetRotation( const decVector &rotation );
	
	/** \brief Scaling. */
	inline float GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( float scaling );
	
	/** \brief Bend state index or -1 if not assigned. */
	inline int GetBendState() const{ return pBendState; }
	
	/** \brief Set bend state index or -1 if not assigned. */
	void SetBendState( int state );
	/*@}*/
};

#endif
