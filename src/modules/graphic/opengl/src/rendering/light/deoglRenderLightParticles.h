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
