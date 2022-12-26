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

#ifndef _DEOGLLIGHTPIPELINESPOINT_H_
#define _DEOGLLIGHTPIPELINESPOINT_H_

#include "deoglLightPipelinesRanged.h"


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelinesPoint : public deoglLightPipelinesRanged{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelinesPoint( const deoglRLight &light );
	
protected:
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelinesPoint();
	/*@}*/
	
	
	
protected:
	virtual void pPreparePipelines();
	
	
	
private:
	void pPrepareNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareSolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareSolid1Transp1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareSolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareSolid2Transp1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareSolid2Transp2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareLumSolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareLumSolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareGIRayNoShadow( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareGIRaySolid1( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pPrepareGIRaySolid2( deoglPipelineConfiguration &basePipelineConfig,
		deoglLightShaderConfig &baseShaderConfig );
	
	void pSetPoint( deoglLightShaderConfig &shaconf );
};

#endif
