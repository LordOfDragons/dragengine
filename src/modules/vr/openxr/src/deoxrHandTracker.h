/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOXRHANDTRACKER_H_
#define _DEOXRHANDTRACKER_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDevicePose.h>

class deoxrSession;
class deoxrAction;


/**
 * Oxr hand tracker.
 */
class deoxrHandTracker : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrHandTracker> Ref;
	
	
	
private:
	struct sBoneMapping{
		deInputDevicePose *bone;
		XrHandJointLocationEXT *location;
		XrHandJointVelocityEXT *velocity;
	};
	
	struct sFingerBending{
		deInputDevice::eHandBones base;
		deInputDevice::eHandBones tip;
		float angleLower;
		float angleUpper;
		float mapValueLower;
		float mapValueUpper;
		float value;
		
		void Init( deInputDevice::eHandBones base,
			deInputDevice::eHandBones tip, float angle0, float angle1 );
	};
	
	deoxrSession &pSession;
	
	const XrHandEXT pHand;
	XrHandTrackerEXT pHandTracker;
	
	XrHandJointLocationEXT *pJointLocations;
	XrHandJointVelocityEXT *pJointVelocities;
	int pJointCount;
	
	XrHandJointsLocateInfoEXT pLocateInfo;
	XrHandJointLocationsEXT pLocations;
	XrHandJointVelocitiesEXT pVelocities;
	XrHandJointsMotionRangeInfoEXT pMotionRange;
	
	deInputDevicePose *pPoseBones;
	int pPoseBoneCount;
	
	sFingerBending pBendFinger[ 5 ];
	float pSpreadFinger[ 4 ];
	
	sBoneMapping *pMapBoneXrToDe;
	int pMapBoneXrToDeCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create hand tracker. */
	deoxrHandTracker( deoxrSession &session, XrHandEXT hand, deoxrSpace &space );
	
protected:
	/** Clean up space. */
	virtual ~deoxrHandTracker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** HandTracker. */
	inline XrHandTrackerEXT GetHandTracker() const{ return pHandTracker; }
	
	/** Locate hand. */
	void Locate();
	
	/** Count of pose bones. */
	inline int GetPoseBoneCount() const{ return pPoseBoneCount; }
	
	/** Pose bone at index. */
	deInputDevicePose &GetPoseBoneAt( int index );
	const deInputDevicePose &GetPoseBoneAt( int index ) const;
	
	/** Bending value for finger. */
	float GetBendFingerAt( int index ) const;
	
	/** Spread finger value for finger. */
	float GetSpreadFingerAt( int index ) const;
	
	/** Log bone poses. */
	void LogPoseBones( const char *prefix ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSetBoneMapping( int index, deInputDevice::eHandBones to, XrHandJointEXT from );
	void pCalcFingerBending();
	void pCalcFingerSpreading();
};

#endif

