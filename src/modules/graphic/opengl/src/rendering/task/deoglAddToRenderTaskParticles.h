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
 * \brief Add To Render Task Particles.
 */
class deoglAddToRenderTaskParticles{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRenderTaskParticles *pRenderTask;
	
	deoglSkinTexture::eShaderTypes pSkinShaderType;
	deoglSkinTexture::eShaderTypes pSkinShaderTypeRibbon;
	deoglSkinTexture::eShaderTypes pSkinShaderTypeBeam;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create add to render task particles object. */
	deoglAddToRenderTaskParticles( deoglRenderThread &renderThread, deoglRenderTaskParticles *renderTask );
	
	/** \brief Clean up add to render task particles object. */
	~deoglAddToRenderTaskParticles();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Shader type to be used for skin shaders. */
	inline deoglSkinTexture::eShaderTypes GetSkinShaderType() const{ return pSkinShaderType; }
	
	/** \brief Set shader type to be used for skin shaders. */
	void SetSkinShaderType( deoglSkinTexture::eShaderTypes shaderType );
	
	/** \brief Shader type to be used for ribbon skin shaders. */
	inline deoglSkinTexture::eShaderTypes GetSkinShaderTypeRibbon() const{ return pSkinShaderTypeRibbon; }
	
	/** \brief Set shader type to be used for ribbon skin shaders. */
	void SetSkinShaderTypeRibbon( deoglSkinTexture::eShaderTypes shaderType );
	
	/** \brief Shader type to be used for beam skin shaders. */
	inline deoglSkinTexture::eShaderTypes GetSkinShaderTypeBeam() const{ return pSkinShaderTypeBeam; }
	
	/** \brief Set shader type to be used for beam skin shaders. */
	void SetSkinShaderTypeBeam( deoglSkinTexture::eShaderTypes shaderType );
	
	
	
	/** \brief Add particle emitter particless from all emitters in a list to the render task. */
	void AddParticles( const deoglParticleEmitterInstanceList &list );
	
	/** \brief Add a particle emitter particles to the render task. */
	void AddParticle( deoglRParticleEmitterInstance &emitter, const deoglRParticleEmitterInstance::sParticle *particle );
	/*@}*/
};

#endif
