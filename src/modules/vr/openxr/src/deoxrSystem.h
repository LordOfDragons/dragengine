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

#ifndef _DEOXRSYSTEM_H_
#define _DEOXRSYSTEM_H_

#include "deoxrBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoxrInstance;


/**
 * Oxr system.
 */
class deoxrSystem : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrSystem> Ref;
	
	/** System. */
	enum eSystem{
		/** Unknown system. */
		esUnknown,
		
		/** SteamVR. */
		esSteamVR
	};
	
	
	
private:
	deoxrInstance &pInstance;
	
	XrSystemId pSystemId;
	
	decString pSystemName;
	eSystem pSystem;
	
	decPoint pMaxRenderImageSize;
	int pMaxLayerCount;
	bool pSupportsOrientationTracking;
	bool pSupportsPositionTracking;
	
	decPoint pLeftEyeViewSize;
	decPoint pLeftEyeViewMaxSize;
	
	decPoint pRightEyeViewSize;
	decPoint pRightEyeViewMaxSize;
	
	decPoint pRenderSize;
	
	bool pSupportsHandTracking;
	bool pSupportsEyeGazeTracking;
	bool pSupportsFaceEyeTracking;
	bool pSupportsFaceLipTracking;
	bool pSupportsPassthrough;
	
	bool pSupportsEnvBlendModeAlphaBlend;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create system. */
	deoxrSystem( deoxrInstance &instance );
	
protected:
	/** Clean up system. */
	virtual ~deoxrSystem();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Instance. */
	inline deoxrInstance &GetInstance() const{ return pInstance; }
	
	/** System Id. */
	inline XrSystemId GetSystemId() const{ return pSystemId; }
	
	/** System name. */
	inline const decString &GetSystemName() const{ return pSystemName; }
	
	/** System. */
	inline eSystem GetSystem() const{ return pSystem; }
	
	/** Maximum render image size (swap chain image size). */
	inline const decPoint &GetMaxRenderImageSize() const{ return pMaxRenderImageSize; }
	
	/** Maximum count of composite layers. */
	inline int GetMaxLayerCount() const{ return pMaxLayerCount; }
	
	/** Supports orientation tracking. */
	inline bool GetSupportsOrientationTracking() const{ return pSupportsOrientationTracking; }
	
	/** Supports position tracking. */
	inline bool GetSupportsPositionTracking() const{ return pSupportsPositionTracking; }
	
	/** Left eye recommended view size. */
	inline const decPoint &GetLeftEyeViewSize() const{ return pLeftEyeViewSize; }
	
	/** Left eye maximum view size. */
	inline const decPoint &GetLeftEyeViewMaxSize() const{ return pLeftEyeViewMaxSize; }
	
	/** Right eye recommended view size. */
	inline const decPoint &GetRightEyeViewSize() const{ return pRightEyeViewSize; }
	
	/** Right eye maximum view size. */
	inline const decPoint &GetRightEyeViewMaxSize() const{ return pRightEyeViewMaxSize; }
	
	/** Render size. */
	inline const decPoint &GetRenderSize() const{ return pRenderSize; }
	
	/** Supports hand tracking. */
	inline bool GetSupportsHandTracking() const{ return pSupportsHandTracking; }
	
	/** Supports eye gaze tracking. */
	inline bool GetSupportsEyeGazeTracking() const{ return pSupportsEyeGazeTracking; }
	
	/** Supports face eye tracking. */
	inline bool GetSupportsFaceEyeTracking() const{ return pSupportsFaceEyeTracking; }
	
	/** Supports face mouth tracking. */
	inline bool GetSupportsFaceLipTracking() const{ return pSupportsFaceLipTracking; }
	
	/** Supports passthrough. */
	inline bool GetSupportsPassthrough() const{ return pSupportsPassthrough; }
	
	/** Supports environment blend mode alpha blend. */
	inline bool GetSupportsEnvBlendModeAlphaBlend() const{ return pSupportsEnvBlendModeAlphaBlend; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

