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
	
	bool pEnableGI;
	bool pEnableAuralization;
	
	deInputEvent::eKeyCodes pHotKeys[EHK_COUNT];
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	meConfiguration(meWindowMain &windowMain);
	
	/** \brief Clean up configuration. */
	~meConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{return pWindowMain;}
	
	
	
	/** \brief Move step size. */
	inline float GetMoveStep() const{return pMoveStep;}
	
	/** \brief Set move step size. */
	void SetMoveStep(float step);
	
	/** \brief Movement has to be snapped to the nearest step distance. */
	inline bool GetMoveSnap() const{return pMoveSnap;}
	
	/** \brief Set if movement has to be snapped to the nearest step distance. */
	void SetMoveSnap(bool snap);
	
	
	
	/** \brief Rotate step size. */
	inline float GetRotateStep() const{return pRotStep;}
	
	/** \brief Set rotate step size. */
	void SetRotateStep(float step);
	
	/** \brief Rotation has to be snapped to the nearest step angle. */
	inline bool GetRotateSnap() const{return pRotSnap;}
	
	/** \brief Set if rotation has to be snapped to the nearest step angle. */
	void SetRotateSnap(bool snap);
	
	
	
	/** \brief Scale step size. */
	inline float GetScaleStep() const{return pScaleStep;}
	
	/** \brief Set scale step size. */
	void SetScaleStep(float step);
	
	/** \brief Scaling has to be snapped to the nearest step factor. */
	inline bool GetScaleSnap() const{return pScaleSnap;}
	
	/** \brief Set if scaling has to be snapped to the nearest step factor. */
	void SetScaleSnap(bool snap);
	
	
	
	/** \brief View is updated on a regular basis. */
	inline bool GetAutoUpdate() const{return pAutoUpdate;}
	
	/** \brief Set if view is updated on a regular basis. */
	void SetAutoUpdate(bool autoUpdate);
	
	/** \brief Mouse sensitivity. */
	inline float GetSensitivity() const{return pSensitivity;}
	
	/** \brief Set mouse sensitivity. */
	void SetSensitivity(float sensitivity);
	
	
	
	/** \brief Enable GI in cameras. */
	inline bool GetEnableGI() const{return pEnableGI;}
	
	/** \brief Set enable GI in cameras. */
	void SetEnableGI(bool enable);
	
	/** \brief Enable auralization in microphones. */
	inline bool GetEnableAuralization() const{return pEnableAuralization;}
	
	/** \brief Set enable auralization in microphones. */
	void SetEnableAuralization(bool enable);
	
	
	
	/** \brief Given hot key. */
	deInputEvent::eKeyCodes GetHotKeyAt(int hotkey) const;
	
	/** \brief Set given hot key. */
	void SetHotKeyAt(int hotkey, deInputEvent::eKeyCodes key);
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{return pPreventSaving;}
	
	/** \brief Set prevent saving. */
	void SetPreventSaving(bool preventSaving);
	
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
