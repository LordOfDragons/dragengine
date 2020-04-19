/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDECONFIGURATION_H_
#define _GDECONFIGURATION_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class gdeWindowMain;



/**
 * \brief Editor configuration.
 */
class gdeConfiguration{
private:
	gdeWindowMain &pWindowMain;
	
	decColor pColorBoundingBox;
	
	decColor pColorCamera;
	decColor pColorCameraActive;
	decColor pColorEnvMapProbe;
	decColor pColorEnvMapProbeReflection;
	decColor pColorEnvMapProbeInfluence;
	decColor pColorEnvMapProbeMask;
	decColor pColorEnvMapProbeActive;
	decColor pColorEnvMapProbeActiveReflection;
	decColor pColorEnvMapProbeActiveInfluence;
	decColor pColorEnvMapProbeActiveMask;
	decColor pColorLight;
	decColor pColorLightActive;
	decColor pColorNavigationSpace;
	decColor pColorNavigationSpaceActive;
	decColor pColorNavigationBlocker;
	decColor pColorNavigationBlockerActive;
	decColor pColorParticleEmitter;
	decColor pColorParticleEmitterActive;
	decColor pColorForceField;
	decColor pColorForceFieldActive;
	decColor pColorSnapPoint;
	decColor pColorSnapPointActive;
	decColor pColorSpeaker;
	decColor pColorSpeakerActive;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	gdeConfiguration( gdeWindowMain &windowMain );
	
	/** \brief Clean up configuration. */
	~gdeConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline gdeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** \brief Color of bounding box. */
	inline const decColor &GetColorBoundingBox() const{ return pColorBoundingBox; }
	
	
	
	/** \brief Color of camera. */
	inline const decColor &GetColorCamera() const{ return pColorCamera; }
	
	/** \brief Color of active camera. */
	inline const decColor &GetColorCameraActive() const{ return pColorCameraActive; }
	
	/** \brief Color of environment map probes. */
	inline const decColor &GetColorEnvMapProbe() const{ return pColorEnvMapProbe; }
	
	/** \brief Color of environment map probes. */
	inline const decColor &GetColorEnvMapProbeReflection() const{
		return pColorEnvMapProbeReflection; }
	
	/** \brief Color of environment map probes. */
	inline const decColor &GetColorEnvMapProbeInfluence() const{
		return pColorEnvMapProbeInfluence; }
	
	/** \brief Color of environment map probes. */
	inline const decColor &GetColorEnvMapProbeMask() const{ return pColorEnvMapProbeMask; }
	
	/** \brief Color of active environment map probes. */
	inline const decColor &GetColorEnvMapProbeActive() const{ return pColorEnvMapProbeActive; }
	
	/** \brief Color of active environment map probes. */
	inline const decColor &GetColorEnvMapProbeActiveReflection() const{
		return pColorEnvMapProbeActiveReflection; }
	
	/** \brief Color of active environment map probes. */
	inline const decColor &GetColorEnvMapProbeActiveInfluence() const{
		return pColorEnvMapProbeActiveInfluence; }
	
	/** \brief Color of active environment map probes. */
	inline const decColor &GetColorEnvMapProbeActiveMask() const{
		return pColorEnvMapProbeActiveMask; }
	
	/** \brief Color of light. */
	inline const decColor &GetColorLight() const{ return pColorLight; }
	
	/** \brief Color of active light. */
	inline const decColor &GetColorLightActive() const{ return pColorLightActive; }
	
	/** \brief Color of navigation space. */
	inline const decColor &GetColorNavigationSpace() const{ return pColorNavigationSpace; }
	
	/** \brief Color of active navigation space. */
	inline const decColor &GetColorNavigationSpaceActive() const{ return pColorNavigationSpaceActive; }
	
	/** \brief Color of navigation blocker. */
	inline const decColor &GetColorNavigationBlocker() const{ return pColorNavigationBlocker; }
	
	/** \brief Color of active navigation blocker. */
	inline const decColor &GetColorNavigationBlockerActive() const{ return pColorNavigationBlockerActive; }
	
	/** \brief Color of particle emitter. */
	inline const decColor &GetColorParticleEmitter() const{ return pColorParticleEmitter; }
	
	/** \brief Color of active particle emitter. */
	inline const decColor &GetColorParticleEmitterActive() const{ return pColorParticleEmitterActive; }
	
	/** \brief Color of force field. */
	inline const decColor &GetColorForceField() const{ return pColorForceField; }
	
	/** \brief Color of active force field. */
	inline const decColor &GetColorForceFieldActive() const{ return pColorForceFieldActive; }
	
	/** \brief Color of snap point. */
	inline const decColor &GetColorSnapPoint() const{ return pColorSnapPoint; }
	
	/** \brief Color of active snap point. */
	inline const decColor &GetColorSnapPointActive() const{ return pColorSnapPointActive; }
	
	/** \brief Color of speaker. */
	inline const decColor &GetColorSpeaker() const{ return pColorSpeaker; }
	
	/** \brief Color of active speaker. */
	inline const decColor &GetColorSpeakerActive() const{ return pColorSpeakerActive; }
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** \brief Set prevent saving. */
	void SetPreventSaving( bool preventSaving );
	
	/**
	 * \brief Load configuration.
	 * \details Tries to locate the configuration file in the known places reading first the
	 *          system wide configuration and then the user configuration if found. For all
	 *          not found configuration values the default value is used.
	 */
	void LoadConfiguration();
	
	/**
	 * \brief Save configuration to user configuration directory.
	 * \details If the directory or files do not exist they are created.
	 */
	void SaveConfiguration();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
