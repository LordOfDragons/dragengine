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

#ifndef _DEOGLRENDERGEOMETRY_H_
#define _DEOGLRENDERGEOMETRY_H_

#include "deoglRenderBase.h"

#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglComponentLOD;
class deoglFramebuffer;
class deoglPersistentRenderTask;
class deoglRComponent;
class deoglRenderPlan;
class deoglRenderTask;
class deoglComputeRenderTask;
class deoglRLight;
class deoglSkinState;
class deoglSkinTexture;
class deoglTexture;
class deoglTriangleBatch;
class deoglVAO;



/**
 * OpenGL Geometry Renderer.
 * Renderer for 3D geometry.
 */
class deoglRenderGeometry : public deoglRenderBase{
public:
	struct sVertexPositionSetParams{
		int firstPoint;
		int pointCount;
		float weight;
	};
	
	
	
private:
	decColor pAmbient;
	
	const deoglPipeline *pPipelineCopyVNT;
	const deoglPipeline *pPipelineVPSTransformVNT;
	const deoglPipeline *pPipelineApproxTransformVNT;
	const deoglPipeline *pPipelineApproxTransformVNTInplace;
	
	sVertexPositionSetParams *pVertexPositionSetParams;
	int pVertexPositionSetParamSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderGeometry( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	~deoglRenderGeometry();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the ambient. */
	inline const decColor &GetAmbient() const{ return pAmbient; }
	/** Sets the ambient light color. */
	void SetAmbient( const decColor &color );
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/** Render a render task. */
	void RenderTask( const deoglRenderTask &renderTask );
	void RenderTask( const deoglComputeRenderTask &renderTask );
	void RenderTask( const deoglPersistentRenderTask &renderTask );
	
	void CopyVNT( GLuint vao, GLuint vbo, const deoglSPBlockSSBO &transformed,
		int firstPoint, int pointCount );
	
	sVertexPositionSetParams *GetVertexPositionSetParams( int count );
	
	void VPSTransformVNT( GLuint vaoModelData, GLuint vboVertexPositionSetData,
		const sVertexPositionSetParams *params, int paramCount, const deoglSPBlockSSBO &transformed );
	
	void ApproxTransformVNT( GLuint vao, GLuint vbo, const deoglSPBlockSSBO *weightMatrices,
		const deoglSPBlockSSBO &transformed, int firstPoint, int pointCount, bool inplace );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
