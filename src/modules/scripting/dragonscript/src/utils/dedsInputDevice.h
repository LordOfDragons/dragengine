/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSINPUTDEVICE_H_
#define _DEDSINPUTDEVICE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDevicePose.h>
#include <dragengine/input/deInputEvent.h>

class deScriptingDragonScript;
class deBaseInputModule;
class deBaseVRModule;


/**
 * Extends deInputDevice.
 * 
 * Caches poses.
 */
class dedsInputDevice : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dedsInputDevice> Ref;
	
	
	
private:
	deScriptingDragonScript &pDS;
	
	deInputEvent::eSources pDeviceSource;
	int pDeviceIndex;
	deInputDevice::Ref pDevice;
	
	deInputDevicePose pDevicePose;
	
	deInputDevicePose *pBonePoses;
	int pBonePoseCount;
	
	float *pFaceExpressions;
	int pFaceExpressionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new input device. */
	dedsInputDevice( deScriptingDragonScript &ds, deBaseInputModule &module, int deviceIndex );
	
	/** \brief Create new input device. */
	dedsInputDevice( deScriptingDragonScript &ds, deBaseVRModule &module, int deviceIndex );
	
protected:
	/** \brief Clean up the input device. */
	virtual ~dedsInputDevice();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** DragonScript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Device source. */
	inline deInputEvent::eSources GetDeviceSource() const{ return pDeviceSource; }
	
	/** Device index. */
	inline int GetDeviceIndex() const{ return pDeviceIndex; }
	
	/** Device. */
	inline deInputDevice *GetDevice() const{ return pDevice; }
	
	/** Device pose. */
	inline const deInputDevicePose &GetDevicePose() const{ return pDevicePose; }
	
	/** Count of bone poses. */
	inline int GetBonePoseCount() const{ return pBonePoseCount; }
	
	/** Bone pose at index. */
	const deInputDevicePose &GetBonePoseAt( int index, bool withController ) const;
	
	/** Count of face expressions. */
	inline int GetFaceExpressionCount() const{ return pFaceExpressionCount; }
	
	/** Face expression at index. */
	float GetFaceExpressionAt( int index ) const;
	
	/** Update poses. */
	void OnFrameUpdate();
	
	/**
	 * Update device information. Only allowed to be called if device is from the same
	 * module and the device ID is the same.
	 */
	void Update( const dedsInputDevice &device );
	/*@}*/
};

#endif
