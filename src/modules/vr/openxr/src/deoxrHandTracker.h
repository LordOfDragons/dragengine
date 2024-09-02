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
	deoxrDevice &pDevice;
	
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
	deoxrHandTracker( deoxrSession &session, deoxrDevice &device, XrHandEXT hand );
	
protected:
	/** Clean up space. */
	virtual ~deoxrHandTracker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Hand. */
	inline const XrHandEXT GetHand() const{ return pHand; }
	
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

