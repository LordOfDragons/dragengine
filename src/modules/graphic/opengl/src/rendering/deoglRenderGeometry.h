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
class deoglDepthTriangleList;
class deoglFramebuffer;
class deoglRComponent;
class deoglRenderPlan;
class deoglRenderTask;
class deoglRLight;
class deoglShaderProgram;
class deoglSkinState;
class deoglSkinTexture;
class deoglTexture;
class deoglTriangleBatch;
class deoglVAO;



/**
 * @brief OpenGL Geometry Renderer.
 * Renderer for 3D geometry.
 */
class deoglRenderGeometry : public deoglRenderBase{
private:
	decColor pAmbient;
	
	deoglShaderProgram *pShaderParticle;
	
	deoglShaderProgram *pShaderTransformPositions;
	deoglShaderProgram *pShaderCalcNormalsTangents;
	deoglShaderProgram *pShaderWriteSkinnedVBO;
	
	deoglShaderProgram *pShaderApproxTransformVNT;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderer. */
	deoglRenderGeometry( deoglRenderThread &renderThread );
	/** Cleans up the renderer. */
	~deoglRenderGeometry();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the ambient. */
	inline const decColor &GetAmbient() const{ return pAmbient; }
	/** Sets the ambient light color. */
	void SetAmbient( const decColor &color );
	/*@}*/
	
	/** @name Rendering */
	/*@{*/
	/** Render a render task. */
	void RenderTask( const deoglRenderTask &renderTask );
	
	/** \brief Transform model posititions using component weight matrices using transform feedback. */
	void TransformPositions( const deoglVAO &vao, GLuint tboWeightMatrices,
		GLuint vboTransformed, int firstPoint, int pointCount );
	/** \brief Calculate normals and tangents for a component. */
	void CalcNormalsTangents( const deoglVAO &vao, GLuint tboPositions, deoglFramebuffer *fbo, int outputWidth,
		int outputHeight, int normalCount, int tangentCount, int firstPoint, int pointCount );
	/** \brief Write skinned vbo using transform feedback. */
	void WriteSkinnedVBO( const deoglVAO &vao, GLuint tboPositions, deoglTexture &texNorTan,
		GLuint vboSkinned, int normalCount, int firstPoint, int pointCount );
	/**
	 * \brief Approximately transform model posititions, normals and tangents
	 *        using component weight matrices using transform feedback.
	 */
	void ApproxTransformVNT( const deoglVAO &vao, GLuint tboWeightMatrices,
		GLuint vboTransformed, int firstPoint, int pointCount );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
