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

#ifndef _DEOXRFACETRACKER_H_
#define _DEOXRFACETRACKER_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputDevice.h>

class deoxrSession;


/**
 * Oxr hand tracker.
 */
class deoxrFaceTracker : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrFaceTracker> Ref;
	
	
	
private:
	struct sMapping{
		float *faceExpression;
		float *weight;
	};
	
	deoxrSession &pSession;
	
	XrFacialTrackerHTC pEyeTracker;
	XrFacialTrackerHTC pLipTracker;
	
	float *pEyeWeights;
	float *pLipWeights;
	
	XrFacialExpressionsHTC pEyeExpressionInfo;
	XrFacialExpressionsHTC pLipExpressionInfo;
	
	float *pFaceExpressions;
	int pFaceExpressionCount;
	
	sMapping *pMapEyeXrToDe;
	int pMapEyeXrToDeCount;
	
	sMapping *pMapLipXrToDe;
	int pMapLipXrToDeCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create face tracker. */
	deoxrFaceTracker( deoxrSession &session );
	
protected:
	/** Clean up face tracker. */
	virtual ~deoxrFaceTracker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Eye tracker or nullptr. */
	inline XrFacialTrackerHTC GetEyeTracker() const{ return pEyeTracker; }
	
	/** Lip tracker or nullptr. */
	inline XrFacialTrackerHTC GetLipTracker() const{ return pLipTracker; }
	
	/** Update weights. */
	void Update();
	
	/** Count of face expressions. */
	inline int GetFaceExpressionCount() const{ return pFaceExpressionCount; }
	
	/** Face expression at index. */
	float GetFaceExpressionAt( int index );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pSetEyeMapping( int index, deInputDevice::eFaceExpressions to, XrEyeExpressionHTC from );
	void pSetLipMapping( int index, deInputDevice::eFaceExpressions to, XrLipExpressionHTC from );
};

#endif

