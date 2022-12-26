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

#ifndef _DEOGLLIGHTPIPELINESSKY_H_
#define _DEOGLLIGHTPIPELINESSKY_H_

#include "deoglLightPipelines.h"

class deoglRSkyInstanceLayer;
class deoglLightShaderConfig;


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelinesSky : public deoglLightPipelines{
private:
	const deoglRSkyInstanceLayer &pLayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelinesSky( const deoglRSkyInstanceLayer &layer );
	
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelinesSky();
	/*@}*/
	
	
	
protected:
	virtual void pPreparePipelines();
	
	
	
private:
	void pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareAmbient( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pSetGI( deoglLightShaderConfig &shaconf );
	void pSetNonGI( deoglLightShaderConfig &shaconf );
};

#endif
