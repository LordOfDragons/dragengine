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

#ifndef _DEOGLADDTORENDERTASKPARTICLES_H_
#define _DEOGLADDTORENDERTASKPARTICLES_H_

#include "../../deoglBasics.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../skin/deoglSkinTexture.h"

#include <dragengine/common/math/decMath.h>

class deoglParticleEmitterInstanceList;
class deoglRenderTaskParticles;
class deoglRenderThread;


/**
 * Add To Render Task Particles.
 */
class deoglAddToRenderTaskParticles{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderTaskParticles *pRenderTask;
	
	deoglSkinTexturePipelines::eTypes pSkinPipelineType;
	int pSkinPipelineModifier;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create add to render task particles object. */
	deoglAddToRenderTaskParticles(deoglRenderThread &renderThread, deoglRenderTaskParticles *renderTask);
	
	/** Clean up add to render task particles object. */
	~deoglAddToRenderTaskParticles();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Pipeline type. */
	inline deoglSkinTexturePipelines::eTypes GetSkinPipelineType() const{return pSkinPipelineType;}
	
	/** Set pipeline type. */
	void SetSkinPipelineType(deoglSkinTexturePipelines::eTypes type);
	
	/** Pipeline modifier. */
	inline int GetSkinPipelineModifier() const{return pSkinPipelineModifier;}
	
	/** Set pipeline modifier. */
	void SetSkinPipelineModifier(int modifier);
	
	
	
	/** Add particle emitter particless from all emitters in a list to the render task. */
	void AddParticles(const deoglParticleEmitterInstanceList &list);
	
	/** Add a particle emitter particles to the render task. */
	void AddParticle(deoglRParticleEmitterInstance &emitter, const deoglRParticleEmitterInstance::sParticle *particle);
	/*@}*/
};

#endif
