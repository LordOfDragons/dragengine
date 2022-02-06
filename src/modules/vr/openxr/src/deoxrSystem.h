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
	
	
	
private:
	deoxrInstance &pInstance;
	
	XrSystemId pSystemId;
	
	decPoint pMaxRenderImageSize;
	int pMaxLayerCount;
	bool pSupportsOrientationTracking;
	bool pSupportsPositionTracking;
	
	
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
	
	/** Maximum render image size (swap chain image size). */
	inline const decPoint &GetMaxRenderImageSize() const{ return pMaxRenderImageSize; }
	
	/** Maximum count of composite layers. */
	inline int GetMaxLayerCount() const{ return pMaxLayerCount; }
	
	/** Supports orientation tracking. */
	inline bool GetSupportsOrientationTracking() const{ return pSupportsOrientationTracking; }
	
	/** Supports position tracking. */
	inline bool GetSupportsPositionTracking() const{ return pSupportsPositionTracking; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

