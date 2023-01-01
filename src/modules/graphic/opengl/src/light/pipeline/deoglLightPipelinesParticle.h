/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLLIGHTPIPELINESPARTICLE_H_
#define _DEOGLLIGHTPIPELINESPARTICLE_H_

#include "deoglLightPipelines.h"

class deoglRParticleEmitterType;
class deoglLightShaderConfig;


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelinesParticle : public deoglLightPipelines{
private:
	const deoglRParticleEmitterType &pEmitter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelinesParticle( const deoglRParticleEmitterType &emitter );
	
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelinesParticle();
	/*@}*/
	
	
	
public:
	/** Debug name. */
	virtual const char *GetDebugName() const;
	
	
	
protected:
	virtual void pPreparePipelines();
	
	
	
private:
	void pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pSetNonGI( deoglLightShaderConfig &shaconf );
};

#endif
