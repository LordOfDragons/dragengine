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

#ifndef _DEVRSYSTEM_H_
#define _DEVRSYSTEM_H_

#include "deBaseSystem.h"
#include "modules/vr/deBaseVRModule.h"
#include "../input/deInputEventQueue.h"
#include "../resources/camera/deCamera.h"


/**
 * \brief VR system.
 * \version 1.6
 */
class DE_DLL_EXPORT deVRSystem : public deBaseSystem{
private:
	deBaseVRModule *pActiveModule;
	deInputEventQueue pEventQueue;
	deCamera::Ref pCamera;
	deBaseVRModule::eFeatureSupportLevel pRequestFeatureEyeGazeTracking;
	deBaseVRModule::eFeatureSupportLevel pRequestFeatureFacialTracking;
	bool pEnablePassthrough;
	float pPassthroughTransparency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create vr system. */
	deVRSystem(deEngine *engine);
	
	/** \brief Clean up vr system. */
	~deVRSystem() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active module or nullptr if none is active. */
	inline deBaseVRModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief VR queue. */
	inline deInputEventQueue &GetEventQueue(){ return pEventQueue; }
	inline const deInputEventQueue &GetEventQueue() const{ return pEventQueue; }
	
	/**
	 * \brief Add events to the VR System event queue.
	 * 
	 * This function is called before any other frame related tasks are carried out.
	 * Record changes in devices states have to be recored into a game event
	 * reported as engine input events You are expected to check message and event
	 * queues to deliver system notification (like quitting the game) to the game
	 * engine.
	 */
	void ProcessEvents();
	
	/**
	 * \brief VR Runtime is usable.
	 * 
	 * Returns true if a call to StartRuntime() is likely to succeed or not.
	 */
	bool RuntimeUsable();
	
	/**
	 * \brief Set feature request level for eye gaze tracking.
	 * \version 1.17
	 * 
	 * Tracking eye gaze features is consider a high privacy operation by most VR environments.
	 * Enabling this feature usually requires the user to explicitely agree. Furthermore eye
	 * gaze tracking is not required for the majority of games and applications. For these
	 * reasons eye gaze tracking is disabled by default. To enable set the feature request
	 * level to optional or required.
	 * 
	 * The set feature request level takes effect only the next time StartRuntime() is
	 * called. It has no effect while the VR runtime is running.
	 * 
	 * If the VR module does not support eye gaze tracking and sFeatureSupport::efslRequired
	 * is specified an exception is thrown.
	 */
	void RequestFeatureEyeGazeTracking(deBaseVRModule::eFeatureSupportLevel level);
	
	/**
	 * \brief Set feature request level for facial tracking.
	 * \version 1.17
	 * 
	 * Facial tracking includes eye tracking and mouth tracking. Tracking facial features
	 * is consider a high privacy operation by most VR environments. Enabling this feature
	 * usually requires the user to explicitely agree. Furthermore facial tracking typically
	 * is an expensive operation and can degrade performance. Last but not least facial
	 * tracking is not required for the majority of games and applications. For this reason
	 * facial tracking is disabled by default. To enable set the feature request level to
	 * optional or required.
	 * 
	 * The set feature request level takes effect only the next time StartRuntime() is
	 * called. It has no effect while the VR runtime is running.
	 * 
	 * If the VR module does not support facial tracking and sFeatureSupport::efslRequired
	 * is specified an exception is thrown.
	 */
	void RequestFeatureFacialTracking(deBaseVRModule::eFeatureSupportLevel level);
	
	/**
	 * \brief Start VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various modules. VR mode can be started and
	 * stopped at any time.
	 */
	void StartRuntime();
	
	/**
	 * \brief Stop VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various modules. VR mode can be started and
	 * stopped at any time.
	 */
	void StopRuntime();
	
	/**
	 * \brief VR runtime is running.
	 * \version 1.26
	 */
	bool IsRuntimeRunning();
	
	/** \brief Camera to render on head mounted display or nullptr. */
	inline const deCamera::Ref &GetCamera() const{ return pCamera; }
	
	/**
	 * \brief Set camera to render on head mounted display.
	 * 
	 * If set to nullptr fades back to safe scene as defined by VR Runtime.
	 */
	void SetCamera(deCamera *camera);
	
	/**
	 * \brief VR Runtime supports presenting user environment inside the rendered world.
	 * \version 1.12
	 */
	bool SupportsPassthrough();
	
	/**
	 * \brief Presenting user environment inside the rendered world is enabled.
	 * \version 1.12
	 * 
	 * Has no effect if SupportsPassthrough() returns false.
	 */
	inline bool GetEnablePassthrough() const{ return pEnablePassthrough; }
	
	/**
	 * \brief Enable presenting user environment inside the rendered world.
	 * \version 1.12
	 * 
	 * Has no effect if SupportsPassthrough() returns false.
	 */
	void SetEnablePassthrough(bool enable);
	
	/**
	 * \brief Transparency of user environment presented inside the rendered world.
	 * \version 1.12
	 * 
	 * Has no effect if SupportsPassthrough() returns false. A value of 0 hides the
	 * environment. A value of 1 shows the environment. Values in between blend over.
	 */
	inline float GetPassthroughTransparency() const{ return pPassthroughTransparency; }
	
	/**
	 * \brief Set transparency of user environment presented inside the rendered world.
	 * \version 1.12
	 * 
	 * Has no effect if SupportsPassthrough() returns false. A value of 0 hides the
	 * environment. A value of 1 shows the environment. Values in between blend over.
	 */
	void SetPassthroughTransparency(float transparency);
	
	/**
	 * \brief Center playspace with forward direction matching looking direction.
	 * \version 1.28
	 * 
	 * Playspace is not automatically centered after starting the VR runtime. Call this function
	 * any time later to center the playspace, for example if the player adjusted seating position
	 * or if the VR runtime uses a broken playspace orientation.
	 */
	void CenterPlayspace();
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	void SetActiveModule(deLoadableModule *module) override;
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	void ClearPermanents() override;
	
	/** \brief Carry out here actions right after the system started up. */
	void PostStart() override;
	
	/** \brief Carry out here actions right before the system shuts down. */
	void PreStop() override;
	/*@}*/
};

#endif
