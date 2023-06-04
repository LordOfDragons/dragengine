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
#ifndef _AEANIMATORLOCOMOTIONLEG_H_
#define _AEANIMATORLOCOMOTIONLEG_H_

// includes
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>
#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class aeAnimatorLocomotion;



/**
 * Animator Locomotion Leg.
 *
 * Keeps track of a leg in an animator locomotion. A leg stores various
 * parameters required for properly animating a leg on uneven terrain.
 */
class aeAnimatorLocomotionLeg{
private:
	aeAnimatorLocomotion *pLocomotion;
	
	igdeWDebugDrawerShape pDDSLocked;
	igdeWCoordSysArrows pDDSLockedCF;
	igdeWDebugDrawerShape pDDSPredict;
	
	decString pName;
	
	decVector pPutDownPosStand;
	decVector pPutDownPosWalk;
	decVector pPutDownPosRun;
	
	float pLiftOffTime;
	float pPutDownTime;
	
	bool pPositionLocked;
	decDVector pLockedPosition;
	decDVector pLockedRotation;
	decVector pLockedNormal;
	
	decDVector pPredictPosition;
	decDVector pPredictRotation;
	decVector pPredictNormal;
	
	decVector pGroundPosition;
	decVector pGroundNormal;
	
	float pIKInfluence;
	decVector pIKPosition;
	decVector pIKOrientation;
	
	decString pVisBoneName;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	aeAnimatorLocomotionLeg( aeAnimatorLocomotion *locomotion );
	/** Cleans up the object. */
	~aeAnimatorLocomotionLeg();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	
	/** Retrieves the stand put down position. */
	inline const decVector &GetPutDownPositionStand() const{ return pPutDownPosStand; }
	/** Sets the stand put down position. */
	void SetPutDownPositionStand( const decVector &position );
	/** Retrieves the walk put down position. */
	inline const decVector &GetPutDownPositionWalk() const{ return pPutDownPosWalk; }
	/** Sets the walk put down position. */
	void SetPutDownPositionWalk( const decVector &position );
	/** Retrieves the run put down position. */
	inline const decVector &GetPutDownPositionRun() const{ return pPutDownPosRun; }
	/** Sets the run put down position. */
	void SetPutDownPositionRun( const decVector &position );
	
	/** Retrieves the lift off time. */
	inline float GetLiftOffTime() const{ return pLiftOffTime; }
	/** Sets the lift off time. */
	void SetLiftOffTime( float time );
	/** Retrieves the put down time. */
	inline float GetPutDownTime() const{ return pPutDownTime; }
	/** Sets the put down time. */
	void SetPutDownTime( float time );
	
	/** Retrieves the ground position. */
	inline const decVector &GetGroundPosition() const{ return pGroundPosition; }
	/** Retrieves the ground normal. */
	inline const decVector &GetGroundNormal() const{ return pGroundNormal; }
	
	/** Retrieves the ik influence value. */
	inline float GetIKInfluence() const{ return pIKInfluence; }
	/** Retrieves the ik position. */
	inline const decVector &GetIKPosition() const{ return pIKPosition; }
	/** Retrieves the ik orientation. */
	inline const decVector &GetIKOrientation() const{ return pIKOrientation; }
	
	/** Retrieves the name of the visualization bone. */
	inline const decString &GetVisBoneName() const{ return pVisBoneName; }
	/** Sets the name of the visualization bone. */
	void SetVisBoneName( const char *name );
	
	/** Resets leg. */
	void Reset();
	/** Update leg. */
	void Update( float elapsed );
	/** Post update after the animator has been applied. */
	void PostUpdate();
	/** Update debug drawers before rendering. */
	void UpdateDebugDrawers();
	/** Updates the shapes. */
	void UpdateShapes();
	/** Reposition shapes. */
	void RepositionShapes();
	
	/** Sets the shapes visible or not. */
	void SetShapesVisible( bool visible );
	/*@}*/
};

// end of include only once
#endif
