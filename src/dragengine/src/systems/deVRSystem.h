/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEVRSYSTEM_H_
#define _DEVRSYSTEM_H_

#include "deBaseSystem.h"
#include "../input/deInputEventQueue.h"
#include "../resources/camera/deCamera.h"

class deBaseVRModule;


/**
 * \brief VR system.
 * \version 1.6
 */
class DE_DLL_EXPORT deVRSystem : public deBaseSystem{
private:
	deBaseVRModule *pActiveModule;
	deInputEventQueue pEventQueue;
	deCamera::Ref pCamera;
	bool pEnablePassthrough;
	float pPassthroughTransparency;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create vr system. */
	deVRSystem( deEngine *engine );
	
	/** \brief Clean up vr system. */
	virtual ~deVRSystem();
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
	
	/** \brief Camera to render on head mounted display or nullptr. */
	inline deCamera *GetCamera() const{ return pCamera; }
	
	/**
	 * \brief Set camera to render on head mounted display.
	 * 
	 * If set to nullptr fades back to safe scene as defined by VR Runtime.
	 */
	void SetCamera( deCamera *camera );
	
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
	void SetEnablePassthrough( bool enable );
	
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
	void SetPassthroughTransparency( float transparency );
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule( deLoadableModule *module );
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
};

#endif
