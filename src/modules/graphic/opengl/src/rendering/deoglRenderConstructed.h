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

#ifndef _DEOGLRENDERCONSTRUCTED_H_
#define _DEOGLRENDERCONSTRUCTED_H_

#include "deoglRenderBase.h"
#include "../skin/state/node/deoglSkinStateConstructedNode.h"

#include <dragengine/resources/skin/property/node/deSkinPropertyNode.h>


class deoglRenderCanvasContext;
class deoglSkinStateCNImage;
class deoglSkinStateCNShape;
class deoglSkinStateCNText;
class deoglRenderTarget;


/**
 * Constructed renderer.
 */
class deoglRenderConstructed : public deoglRenderBase{
private:
	GLuint pVBOShapes;
	GLuint pVAOShapes;
	GLuint pActiveVAO;
	
	const deoglPipeline *pPipelineCanvasColor[ 2 ];
	const deoglPipeline *pPipelineCanvasColorMask[ 2 ];
	const deoglPipeline *pPipelineCanvasImage[ 2 ];
	const deoglPipeline *pPipelineCanvasImageMask[ 2 ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create canvas renderer. */
	deoglRenderConstructed( deoglRenderThread &renderThread );
	
	/** Clean up canvas renderer. */
	virtual ~deoglRenderConstructed() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set opengl states required for constructed rendering. */
	void Prepare( const deoglRenderCanvasContext &context );
	
	/** Draw shape node. */
	void DrawNodeShape( const deoglRenderCanvasContext &context, const deoglSkinStateCNShape &node );
	
	/** Draw image node. */
	void DrawNodeImage( const deoglRenderCanvasContext &context, const deoglSkinStateCNImage &node );
	
	/** Draw text node. */
	void DrawNodeText( const deoglRenderCanvasContext &context, const deoglSkinStateCNText &node );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateShapesVAO();
	void pActivateVAOShapes();
	void pCreatePipelines( const deoglPipeline* (&pipelines)[ 2 ], deoglPipelineConfiguration &config );
};

#endif
