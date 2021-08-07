/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEINPUTSYSTEM_H_
#define _DEINPUTSYSTEM_H_

#include "deBaseSystem.h"

class deInputEvent;
class deInputEventQueue;
class deBaseInputModule;


/**
 * \brief Input system acquiring input from the user.
 * 
 * Supported input devices are all devices found on the host system which can
 * be used to provide digital or analog input.
 */
class deInputSystem : public deBaseSystem{
private:
	deBaseInputModule *pActiveModule;
	deInputEventQueue *pEventQueue;
	bool pCaptureInputDevices;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input system. */
	deInputSystem( deEngine *engine );
	
	/** \brief Clean up input system. */
	virtual ~deInputSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active module or NULL if none is active. */
	inline deBaseInputModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Input queue. */
	inline deInputEventQueue &GetEventQueue() const{ return *pEventQueue; }
	
	/** \brief Clear event queues. */
	void ClearEventQueues();
	
	/** \brief Notify active input module screen size changed. */
	void ScreenSizeChanged();
	
	/** \brief Notify active input module Application activated or deactivated. */
	void AppActivationChanged();
	
	/** \brief Input devices are captured by the active input module if supported. */
	inline bool GetCaptureInputDevices() const{ return pCaptureInputDevices; }
	
	/** \brief Set if input devices are captured by the active input module if supported. */
	void SetCaptureInputDevices( bool captureInputDevices );
	
	/**
	 * \brief Notify active module global engine property changed.
	 * \version 1.6
	 */
	void NotifyGlobalPropertyChanged();
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
