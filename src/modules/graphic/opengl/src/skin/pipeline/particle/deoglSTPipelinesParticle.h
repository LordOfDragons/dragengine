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

#ifndef _DEOGLSTPIPELINESPARTICLE_H_
#define _DEOGLSTPIPELINESPARTICLE_H_

#include "../deoglSkinTexturePipelines.h"


/**
 * Particle skin texture pipelines.
 */
class deoglSTPipelinesParticle : public deoglSkinTexturePipelines{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline. */
	deoglSTPipelinesParticle( const deoglSkinTexture &texture );
	
protected:
	/** Clean up skin texture pipeline. */
	virtual ~deoglSTPipelinesParticle();
	/*@}*/
	
	
	
protected:
	virtual void pPreparePipelines( const ChannelInfo &cinfo );
	
	virtual void pSetTypeGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetTexturesGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	
	virtual void pSetDynamicGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDynamicsGeometryLuminance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDynamicDepth( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDynamicDepthOutline( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
};

#endif
