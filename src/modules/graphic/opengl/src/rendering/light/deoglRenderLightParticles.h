/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRENDERLIGHTPARTICLES_H_
#define _DEOGLRENDERLIGHTPARTICLES_H_

#include "deoglRenderLightBase.h"

class deoglLightShader;
class deoglRenderPlan;
class deoglRParticleEmitter;
class deoglRParticleEmitterType;
class deoglRParticleEmitterInstanceType;
class deoglSPBlockUBO;



/**
 * Render Particle Lights.
 * Renders small lights around emissive particles. Particle lights do not cast
 * shadows nor do they affect transparent materials. This way the emissive
 * light of particles is visible without reducing performance too much.
 */
class deoglRenderLightParticles : public deoglRenderLightBase{
private:
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderLightParticles( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	~deoglRenderLightParticles();
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/** Render lights. */
	void RenderLights( deoglRenderPlan &plan );
	
	/** Render particle emitter instance lights. */
	void RenderParticleEmitterInstance( deoglRenderPlan &plan, deoglRParticleEmitterInstance &instance );
	
	/** Render particle emitter instance type lights. */
	void RenderParticleEmitterInstanceType( deoglRenderPlan &plan, deoglRParticleEmitterInstance &instance,
		deoglRParticleEmitterInstanceType &itype, deoglRParticleEmitterType &etype );
	
	/** Updates the light shader parameter block. */
	void UpdateLightParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlan &plan, deoglRParticleEmitter &emitter, deoglRParticleEmitterType &type );
	
	/** Updates the instance shader parameter block. */
	void UpdateInstanceParamBlock( deoglLightShader &lightShader, deoglSPBlockUBO &paramBlock,
		deoglRenderPlan &plan, deoglRParticleEmitterInstance &instance, deoglRParticleEmitterInstanceType &type );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
