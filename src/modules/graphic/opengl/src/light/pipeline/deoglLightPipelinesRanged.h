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

#ifndef _DEOGLLIGHTPIPELINESRANGED_H_
#define _DEOGLLIGHTPIPELINESRANGED_H_

#include "deoglLightPipelines.h"

class deoglRLight;
class deoglLightShaderConfig;


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelinesRanged : public deoglLightPipelines{
protected:
	const deoglRLight &pLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelinesRanged( const deoglRLight &light );
	
protected:
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelinesRanged();
	/*@}*/
	
	
	
protected:
	void pSetBaseNoShadow( deoglLightShaderConfig &shaconf );
	void pSetBaseSolid1( deoglLightShaderConfig &shaconf );
	void pSetBaseSolid1Transp1( deoglLightShaderConfig &shaconf );
	void pSetBaseSolid2( deoglLightShaderConfig &shaconf );
	void pSetBaseSolid2Transp1( deoglLightShaderConfig &shaconf );
	void pSetBaseSolid2Transp2( deoglLightShaderConfig &shaconf );
	void pSetBaseLumSolid1( deoglLightShaderConfig &shaconf );
	void pSetBaseLumSolid2( deoglLightShaderConfig &shaconf );
	void pSetBaseGIRayNoShadow( deoglLightShaderConfig &shaconf );
	void pSetBaseGIRaySolid1( deoglLightShaderConfig &shaconf );
	void pSetBaseGIRaySolid2( deoglLightShaderConfig &shaconf );
	
	void pSetGI( deoglLightShaderConfig &shaconf );
	void pSetNonGI( deoglLightShaderConfig &shaconf );
};

#endif
