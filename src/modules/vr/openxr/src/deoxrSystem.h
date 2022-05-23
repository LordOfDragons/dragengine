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
	bool pSupportsEyeTracking;
	
	
	
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
	
	/** Supports eye tracking. */
	inline bool GetSupportsEyeTracking() const{ return pSupportsEyeTracking; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

