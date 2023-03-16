/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRENDEROVERLAY_H_
#define _DEOGLRENDEROVERLAY_H_

#include "deoglRenderBase.h"

class deoglRenderCanvasContext;


/**
 * Overlay renderer.
 */
class deoglRenderOverlay : public deoglRenderBase{
private:
	const deoglPipeline *pPipelineImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create renderer. */
	deoglRenderOverlay( deoglRenderThread &renderThread );
	
	/** Clean up renderer. */
	~deoglRenderOverlay();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/** Render overlays. */
	void RenderOverlays( const deoglRenderCanvasContext &context );
	
	/** Render shader compiling information overlay. */
	void RenderShaderCompilingInfo( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif
