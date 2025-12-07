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
	dedsInputDevice(deScriptingDragonScript &ds, deBaseInputModule &module, int deviceIndex);
	
	/** \brief Create new input device. */
	dedsInputDevice(deScriptingDragonScript &ds, deBaseVRModule &module, int deviceIndex);
	
protected:
	/** \brief Clean up the input device. */
	virtual ~dedsInputDevice();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** DragonScript module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** Device source. */
	inline deInputEvent::eSources GetDeviceSource() const{return pDeviceSource;}
	
	/** Device index. */
	inline int GetDeviceIndex() const{return pDeviceIndex;}
	
	/** Device. */
	inline deInputDevice *GetDevice() const{return pDevice;}
	
	/** Device pose. */
	inline const deInputDevicePose &GetDevicePose() const{return pDevicePose;}
	
	/** Count of bone poses. */
	inline int GetBonePoseCount() const{return pBonePoseCount;}
	
	/** Bone pose at index. */
	const deInputDevicePose &GetBonePoseAt(int index, bool withController) const;
	
	/** Count of face expressions. */
	inline int GetFaceExpressionCount() const{return pFaceExpressionCount;}
	
	/** Face expression at index. */
	float GetFaceExpressionAt(int index) const;
	
	/** Update poses. */
	void OnFrameUpdate();
	
	/**
	 * Update device information. Only allowed to be called if device is from the same
	 * module and the device ID is the same.
	 */
	void Update(const dedsInputDevice &device);
	/*@}*/
};

#endif
