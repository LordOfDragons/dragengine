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
	const deoglPipeline *pPipelineCanvasImageArray[2];
	const deoglPipeline *pPipelineCanvasImageMaskArray[2];
	
	
	
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
	void pCreatePipelines(const deoglPipeline* (&pipelines)[2],
		deoglPipelineConfiguration &config, const deoglShaderSources *sources,
		const deoglShaderDefines &defines);
};

#endif
