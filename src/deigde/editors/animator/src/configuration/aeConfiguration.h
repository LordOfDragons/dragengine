/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AECONFIGURATION_H_
#define _AECONFIGURATION_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputEvent.h>

class aeWindowMain;



/**
 * \brief Editor Configuration.
 */
class aeConfiguration{
private:
	aeWindowMain &pWindowMain;
	
	deInputEvent::eKeyCodes pLocoKeyForward;
	deInputEvent::eKeyCodes pLocoKeyBackwards;
	deInputEvent::eKeyCodes pLocoKeyLeft;
	deInputEvent::eKeyCodes pLocoKeyRight;
	deInputEvent::eKeyCodes pLocoKeyCrouch;
	deInputEvent::eKeyCodes pLocoKeyRun;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	aeConfiguration( aeWindowMain &windowMain );
	
	/** \brief Clean up configuration. */
	~aeConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline aeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** \brief Locomotion testing forward key. */
	inline deInputEvent::eKeyCodes GetLocoKeyForward() const{ return pLocoKeyForward; }
	
	/** \brief Set locomotion testing forward key. */
	void SetLocoKeyForward( deInputEvent::eKeyCodes key );
	
	/** \brief Locomotion testing backwards key. */
	inline deInputEvent::eKeyCodes GetLocoKeyBackwards() const{ return pLocoKeyBackwards; }
	
	/** \brief Set locomotion testing backwards key. */
	void SetLocoKeyBackwards( deInputEvent::eKeyCodes key );
	
	/** \brief Locomotion testing left key. */
	inline deInputEvent::eKeyCodes GetLocoKeyLeft() const{ return pLocoKeyLeft; }
	
	/** \brief Set locomotion testing left key. */
	void SetLocoKeyLeft( deInputEvent::eKeyCodes key );
	
	/** \brief Locomotion testing right key. */
	inline deInputEvent::eKeyCodes GetLocoKeyRight() const{ return pLocoKeyRight; }
	
	/** \brief Set locomotion testing right key. */
	void SetLocoKeyRight( deInputEvent::eKeyCodes key );
	
	/** \brief Locomotion testing crouch key. */
	inline deInputEvent::eKeyCodes GetLocoKeyCrouch() const{ return pLocoKeyCrouch; }
	
	/** \brief Set locomotion testing crouch key. */
	void SetLocoKeyCrouch( deInputEvent::eKeyCodes key );
	
	/** \brief Locomotion testing run key. */
	inline deInputEvent::eKeyCodes GetLocoKeyRun() const{ return pLocoKeyRun; }
	
	/** \brief Set locomotion testing run key. */
	void SetLocoKeyRun( deInputEvent::eKeyCodes key );
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** \brief Set prevent saving. */
	void SetPreventSaving( bool preventSaving );
	
	
	
	/** \brief Load configuration. */
	void LoadConfiguration();
	
	/** \brief Save configuration. */
	void SaveConfiguration();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pReset();
};

#endif
