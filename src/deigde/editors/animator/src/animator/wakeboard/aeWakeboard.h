/* 
 * Drag[en]gine IGDE Animator Editor
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
#ifndef _AEWAKEBOARD_H_
#define _AEWAKEBOARD_H_

// includes
#include <dragengine/common/math/decMath.h>

// predefinitions
class aeAnimator;
class deColliderVolume;
class deComponent;



/**
 * Animation Wakeboard.
 *
 * Animation wakeboard.
 */
class aeWakeboard{
private:
	aeAnimator *pAnimator;
	
	deComponent *pEngComponent;
	deColliderVolume *pEngCollider;
	
	bool pEnabled;
	
	decDVector pPosition;
	
	float pTiltUpDown;
	float pMaxTiltUpDown;
	float pTiltLeftRight;
	float pMaxTiltLeftRight;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new wakeboard object. */
	aeWakeboard( aeAnimator *animator );
	/** Cleans up the wakeboard object. */
	~aeWakeboard();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Resets the wakeboard. */
	void Reset();
	
	/** Retrieves the animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	/** Retrieves the component. */
	inline deComponent *GetComponent() const{ return pEngComponent; }
	/** Retrieves the collider. */
	inline deColliderVolume *GetCollider() const{ return pEngCollider; }
	
	/** Determines if the wakeboard is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	/** Sets if the wakeboard is enabled. */
	void SetEnabled( bool enabled );
	
	/** Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( const decDVector &position );
	
	/** Retrieves the up-down tilt in degrees. */
	inline float GetTiltUpDown() const{ return pTiltUpDown; }
	/** Sets the up-down tilt in degrees. */
	void SetTiltUpDown( float tilt );
	/** Retrieves the maximum up-down tilt in degrees. */
	inline float GetMaximumTiltUpDown() const{ return pMaxTiltUpDown; }
	/** Sets the matimum up-down tilt in degrees. */
	void SetMaximumTiltUpDown( float tilt );
	
	/** Retrieves the left-right tilt in degrees. */
	inline float GetTiltLeftRight() const{ return pTiltLeftRight; }
	/** Sets the left-right tilt in degrees. */
	void SetTiltLeftRight( float tilt );
	/** Retrieves the maximum left-right tilt in degrees. */
	inline float GetMaximumTiltLeftRight() const{ return pMaxTiltLeftRight; }
	/** Sets the matimum left-right tilt in degrees. */
	void SetMaximumTiltLeftRight( float tilt );
	
	/** Updates the orientation using the tilt values. */
	void UpdateOrientation();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
