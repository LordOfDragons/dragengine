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

#ifndef _DEPROPFIELDBENDSTATE_H_
#define _DEPROPFIELDBENDSTATE_H_

#include "../../common/math/decMath.h"


/**
 * \brief Prop Field Bend State.
 *
 * Stores the bending state of one or more instances.
 */
class dePropFieldBendState{
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
