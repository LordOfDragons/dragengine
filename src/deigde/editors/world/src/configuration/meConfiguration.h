/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MECONFIGURATION_H_
#define _MECONFIGURATION_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/input/deInputEvent.h>

class meWindowMain;



/**
 * \brief Editor configuration.
 */
class meConfiguration{
public:
	/** \brief Hot Keys. */
	enum eHotKeys{
		/** \brief Select work mode. */
		ehkSelectWorkMode,
		
		/** \brief Select element mode. */
		ehkSelectElementMode,
		
		/** \brief World mode, move element. */
		ehkWorldMove,
		
		/** \brief World mode, rotate element. */
		ehkWorldRotate,
		
		/** \brief World mode, scale element. */
		ehkWorldScale,
		
		/** \brief Edit mode, lock x axis. */
		ehkEditLockX,
		
		/** \brief Edit mode, lock y axis. */
		ehkEditLockY,
		
		/** \brief Edit mode, lock z axis. */
		ehkEditLockZ,
		
		/** \brief Edit mode, local coordinate frame. */
		ehkEditLocalCFrame,
		
		/** \brief Edit mode, snap to snap points. */
		ehkEditSnapToSnapPoints,
		
		/** \brief Dummy, count of hot keys. */
		EHK_COUNT
	};
	
	
	
private:
	meWindowMain &pWindowMain;
	
	float pMoveStep;
	bool pMoveSnap;
	
	float pRotStep;
	bool pRotSnap;
	
	float pScaleStep;
	bool pScaleSnap;
	
	bool pAutoUpdate;
	float pSensitivity;
	
	deInputEvent::eKeyCodes pHotKeys[ EHK_COUNT ];
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	meConfiguration( meWindowMain &windowMain );
	
	/** \brief Clean up configuration. */
	~meConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** \brief Move step size. */
	inline float GetMoveStep() const{ return pMoveStep; }
	
	/** \brief Set move step size. */
	void SetMoveStep( float step );
	
	/** \brief Movement has to be snapped to the nearest step distance. */
	inline bool GetMoveSnap() const{ return pMoveSnap; }
	
	/** \brief Set if movement has to be snapped to the nearest step distance. */
	void SetMoveSnap( bool snap );
	
	
	
	/** \brief Rotate step size. */
	inline float GetRotateStep() const{ return pRotStep; }
	
	/** \brief Set rotate step size. */
	void SetRotateStep( float step );
	
	/** \brief Rotation has to be snapped to the nearest step angle. */
	inline bool GetRotateSnap() const{ return pRotSnap; }
	
	/** \brief Set if rotation has to be snapped to the nearest step angle. */
	void SetRotateSnap( bool snap );
	
	
	
	/** \brief Scale step size. */
	inline float GetScaleStep() const{ return pScaleStep; }
	
	/** \brief Set scale step size. */
	void SetScaleStep( float step );
	
	/** \brief Scaling has to be snapped to the nearest step factor. */
	inline bool GetScaleSnap() const{ return pScaleSnap; }
	
	/** \brief Set if scaling has to be snapped to the nearest step factor. */
	void SetScaleSnap( bool snap );
	
	
	
	/** \brief View is updated on a regular basis. */
	inline bool GetAutoUpdate() const{ return pAutoUpdate; }
	
	/** \brief Set if view is updated on a regular basis. */
	void SetAutoUpdate( bool autoUpdate );
	
	/** \brief Mouse sensitivity. */
	inline float GetSensitivity() const{ return pSensitivity; }
	
	/** \brief Set mouse sensitivity. */
	void SetSensitivity( float sensitivity );
	
	
	
	/** \brief Given hot key. */
	deInputEvent::eKeyCodes GetHotKeyAt( int hotkey ) const;
	
	/** \brief Set given hot key. */
	void SetHotKeyAt( int hotkey, deInputEvent::eKeyCodes key );
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** \brief Set prevent saving. */
	void SetPreventSaving( bool preventSaving );
	
	/**
	 * \brief Load the configuration.
	 * 
	 * Tries to locate the configuration file in the known places reading first the system
	 * wide configuration and then the user configuration if found. For all not found
	 * configuration values the default value is used.
	 */
	void LoadConfiguration();
	
	/**
	 * \brief Save configuration to the user configuration directory.
	 * 
	 * If the directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pReset();
};

#endif
