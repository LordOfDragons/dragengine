/* 
 * Drag[en]gine Bullet Physics Module
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
#ifndef _DEBPFFVORTEX_H_
#define _DEBPFFVORTEX_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions



/**
 * @brief Force Field Vortex.
 *
 * A vortex in a force field to create more interesting waving behavior
 * of prop field instances. To describe a vortex a few parameters are
 * used avoiding complicated calculations. The velocity defines the
 * magnitude of the velocity vectory on the rings close to the center
 * of the vortex. The rotation speed of objects depending on the distance
 * to the center of the vortex is calculated directly from the velocity
 * parameter. The vortex itself is considered to be a 2D rotational
 * force field with an orientation in the world.
 */
class debpFFVortex{
private:
	decVector pPosition;
	decVector pView;
	float pSize;
	
	float pFullVelocity;
	float pCurrentVelocity;
	
	float pTimeToLive;
	float pLivedTime;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new vortex. */
	debpFFVortex();
	/** Cleans up the vortex. */
	~debpFFVortex();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decVector &position );
	/** Retrieves the view direction. */
	inline const decVector &GetView() const{ return pView; }
	/** Sets the view direction. */
	void SetView( const decVector &view );
	/** Retrieves the size. */
	inline float GetSize() const{ return pSize; }
	/** Sets the size. */
	void SetSize( float size );
	
	/** Retrieves the full velocity. */
	inline float GetFullVelocity() const{ return pFullVelocity; }
	/** Sets the full velocity. */
	void SetFullVelocity( float velocity );
	/** Retrieves the current velocity. */
	inline float GetCurrentVelocity() const{ return pCurrentVelocity; }
	/** Sets the current velocity. */
	void SetCurrentVelocity( float velocity );
	
	/** Retrieves the time to live. */
	inline float GetTimeToLive() const{ return pTimeToLive; }
	/** Sets the time to live. */
	void SetTimeToLive( float time );
	/** Retrieves the lived time. */
	inline float GetLivedTime() const{ return pLivedTime; }
	/** Sets the lived time. */
	void SetLivedTime( float time );
	/** Increases the lived time. */
	void IncreaseLivedTime( float time );
	/** Determines if the vortex is still living. */
	inline bool IsLiving() const{ return pLivedTime < pTimeToLive; }
	/** Determines if the vortex has died. */
	inline bool HasDied() const{ return pLivedTime >= pTimeToLive; }
	
	/** Updates the parameters depending on the lived time. */
	void Update();
	/*@}*/
};

// end of include only once
#endif
