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
	deoglRenderGeometry(deoglRenderThread &renderThread);
	/** Cleans up the renderer. */
	~deoglRenderGeometry() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the ambient. */
	inline const decColor &GetAmbient() const{ return pAmbient; }
	/** Sets the ambient light color. */
	void SetAmbient(const decColor &color);
	/*@}*/
	
	/** \name Rendering */
	/*@{*/
	/** Render a render task. */
	void RenderTask(const deoglRenderTask &renderTask);
	void RenderTask(const deoglComputeRenderTask &renderTask);
	void RenderTask(const deoglPersistentRenderTask &renderTask);
	
	void CopyVNT(GLuint vao, GLuint vbo, const deoglSPBlockSSBO &transformed,
		int firstPoint, int pointCount);
	
	sVertexPositionSetParams *GetVertexPositionSetParams(int count);
	
	void VPSTransformVNT(GLuint vaoModelData, GLuint vboVertexPositionSetData,
		const sVertexPositionSetParams *params, int paramCount, const deoglSPBlockSSBO &transformed);
	
	void ApproxTransformVNT(GLuint vao, GLuint vbo, const deoglSPBlockSSBO *weightMatrices,
		const deoglSPBlockSSBO &transformed, int firstPoint, int pointCount, bool inplace);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
