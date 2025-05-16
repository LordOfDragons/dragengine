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
	decColor pColorWorld;
	decColor pColorWorldActive;
	
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
	
	/** \brief Color of world. */
	inline const decColor &GetColorWorld() const{ return pColorWorld; }
	
	/** \brief Color of active world. */
	inline const decColor &GetColorWorldActive() const{ return pColorWorldActive; }
	
	
	
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
	void pReset();
};

#endif
