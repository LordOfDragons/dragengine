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

#ifndef _DEDSVRPLACEHOLDER_H_
#define _DEDSVRPLACEHOLDER_H_

#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/world/deWorld.h>

class deScriptingDragonScript;


/**
 * VR placeholder shown on platforms VR is required while the application has no VR enabled.
 */
class dedsVRPlaceholder : public deObject{
public:
	typedef deTObjectReference<dedsVRPlaceholder> Ref;
	
private:
	deScriptingDragonScript &pDS;
	deWorld::Ref pWorld;
	deComponent::Ref pComponent;
	deCamera::Ref pCamera, pGameCamera;
	int pDeviceIndexHmd;
	bool pEnabled, pVRWorking;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create VR placeholder. */
	dedsVRPlaceholder(deScriptingDragonScript &ds);
	
protected:
	/** Clean up VR placeholder. */
	~dedsVRPlaceholder() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update VR placeholder. */
	void Update();
	
	/** Camera. */
	inline const deCamera::Ref &GetCamera() const{ return pCamera; }
	
	/** Enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Set enabled. */
	void SetEnabled(bool enabled);
	
	/** Get game camera. */
	inline const deCamera::Ref &GetCameCamera() const{ return pGameCamera; }
	
	/** Set game camera. */
	void SetGameCamera(deCamera *camera);
	
	/** Event received. */
	void EventReceived(const deInputEvent &event);
	/*@}*/
	
	
private:
	void pCreateWorld();
	void pCleanUp();
	void pFindHmdDevice();
};

#endif
