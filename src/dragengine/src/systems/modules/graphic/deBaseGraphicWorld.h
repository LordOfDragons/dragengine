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

#ifndef _DEBASEGRAPHICWORLD_H_
#define _DEBASEGRAPHICWORLD_H_

#include "../../../dragengine_export.h"

class deBillboard;
class deCamera;
class deComponent;
class deDebugDrawer;
class deEnvMapProbe;
class deLight;
class deLumimeter;
class dePropField;
class deParticleEmitterInstance;
class dePortalSystem;
class deSmokeEmitter;
class deSkyInstance;


/**
 * \brief Graphic Module World Peer.
 *
 */
class DE_DLL_EXPORT deBaseGraphicWorld{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicWorld();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicWorld();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Update dynamic parts of scene like the skins.
	 * \param elapsed Seconds elapsed since last update
	 */
	virtual void Update( float elapsed );
	
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	/** \brief Height Terrain has changed. */
	virtual void HeightTerrainChanged();
	
	/** \brief Global lighting parameters have changed. */
	virtual void LightingChanged();
	
	/** \brief Sky has been added. */
	virtual void SkyAdded( deSkyInstance *sky );
	
	/** \brief Sky has been removed. */
	virtual void SkyRemoved( deSkyInstance *sky );
	
	/** \brief All skies have been removed. */
	virtual void AllSkiesRemoved();
	
	/** \brief Billboard has been added. */
	virtual void BillboardAdded( deBillboard *billboard );
	
	/** \brief Billboard has been removed. */
	virtual void BillboardRemoved( deBillboard *billboard );
	
	/** \brief All billboards have been removed. */
	virtual void AllBillboardsRemoved();
	
	/** \brief Camera has been added. */
	virtual void CameraAdded( deCamera *camera );
	
	/** \brief Camera has been removed. */
	virtual void CameraRemoved( deCamera *camera );
	
	/** \brief All cameras have been removed. */
	virtual void AllCamerasRemoved();
	
	/** \brief Component has been added. */
	virtual void ComponentAdded( deComponent *component );
	
	/** \brief Component has been removed. */
	virtual void ComponentRemoved( deComponent *component );
	
	/** \brief All components have been removed. */
	virtual void AllComponentsRemoved();
	
	/** \brief DebugDrawer has been added. */
	virtual void DebugDrawerAdded( deDebugDrawer *debugDrawer );
	
	/** \brief DebugDrawer has been removed. */
	virtual void DebugDrawerRemoved( deDebugDrawer *debugDrawer );
	
	/** \brief All debugDrawers have been removed. */
	virtual void AllDebugDrawersRemoved();
	
	/** \brief Environment map probe has been added. */
	virtual void EnvMapProbeAdded( deEnvMapProbe *envMapProbe );
	
	/** \brief Environment map probe has been removed. */
	virtual void EnvMapProbeRemoved( deEnvMapProbe *envMapProbe );
	
	/** \brief All environment map probes have been removed. */
	virtual void AllEnvMapProbesRemoved();
	
	/** \brief Light has been added. */
	virtual void LightAdded( deLight *light );
	
	/** \brief Light has been removed. */
	virtual void LightRemoved( deLight *light );
	
	/** \brief All lights have been removed. */
	virtual void AllLightsRemoved();
	
	/** \brief Lumimeter has been added. */
	virtual void LumimeterAdded( deLumimeter *lumimeter );
	
	/** \brief Lumimeter has been removed. */
	virtual void LumimeterRemoved( deLumimeter *lumimeter );
	
	/** \brief All lumimeters have been removed. */
	virtual void AllLumimetersRemoved();
	
	/** \brief Particle emitter has been added. */
	virtual void ParticleEmitterAdded( deParticleEmitterInstance *emitter );
	
	/** \brief Particle emitter has been removed. */
	virtual void ParticleEmitterRemoved( deParticleEmitterInstance *emitter );
	
	/** \brief All particle emitters have been removed. */
	virtual void AllParticleEmittersRemoved();
	
	/** \brief Prop Field has been added. */
	virtual void PropFieldAdded( dePropField *propField );
	
	/** \brief Prop Field has been removed. */
	virtual void PropFieldRemoved( dePropField *propField );
	
	/** \brief All Prop Fields have been removed. */
	virtual void AllPropFieldsRemoved();
	
	/** \brief Smoke Emitter has been added. */
	virtual void SmokeEmitterAdded( deSmokeEmitter *smokeEmitter );
	
	/** \brief Smoke Emitter has been removed. */
	virtual void SmokeEmitterRemoved( deSmokeEmitter *smokeEmitter );
	
	/** \brief All Smoke Emitters have been removed. */
	virtual void AllSmokeEmittersRemoved();
	/*@}*/
};

#endif
