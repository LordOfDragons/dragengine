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

