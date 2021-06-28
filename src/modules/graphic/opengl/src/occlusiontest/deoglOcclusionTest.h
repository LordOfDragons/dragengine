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

#ifndef _DEOGLOCCLUSIONTEST_H_
#define _DEOGLOCCLUSIONTEST_H_

#include "../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglFramebuffer;
class deoglOcclusionMap;
class deoglOcclusionTestListener;
class deoglPixelBuffer;
class deoglTexture;
class deoglVBOLayout;


/**
 * Provides support for occlusion tests using a simplified hardware approach. Depending on
 * the screen size the starting lod level of the occlusion map can be altered.
 */
class deoglOcclusionTest{
private:
	struct sInputData{
		oglVector3 minExtend;
		oglVector3 maxExtend;
	};
	
	
	
	deoglRenderThread &pRenderThread;
	
	deoglTexture *pTextureResult;
	deoglFramebuffer *pFBOResult;
	
	GLuint pVBO;
	GLuint pVAO;
	deoglVBOLayout *pVBOLayout;
	
	GLuint pVBOResult;
	GLfloat *pVBOResultData;
	int pVBOResultDataSize;

	deoglOcclusionTestListener **pInputListeners;
	sInputData *pInputData;
	int pInputDataCount;
	int pInputDataSize;
	
	deoglPixelBuffer *pPBResults;
	GLubyte *pResults;
	decPoint pResultSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create occlusion test. */
	deoglOcclusionTest( deoglRenderThread &renderThread );
	
	/** Clean up occlusion test. */
	~deoglOcclusionTest();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Result texture. */
	inline deoglTexture *GetTextureResult() const{ return pTextureResult; }
	
	/** Result fbo. */
	inline deoglFramebuffer *GetFBOResult() const{ return pFBOResult; }
	
	
	
	/** Count of input data. */
	inline int GetInputDataCount() const{ return pInputDataCount; }
	
	/** Input data for debugging purpose only. */
	inline void *GetInputData() const{ return pInputData; }
	
	/**
	 * Add input data returning the index to fetch the result later. The test box is
	 * defined using the world minimum and maximum extends translated translated by
	 * the camera position. Hence only the translation part of the camera matrix has
	 * to be applied to the world box extends before using them as input data.
	 * The rotation and projection is handled on the shader side.
	 */
	int AddInputData( const decVector &minExtend, const decVector &maxExtend,
		deoglOcclusionTestListener *listener );
	
	/** Remove all input data. */
	void RemoveAllInputData();
	
	/**
	 * Update VBO using the collected input data. This also resizes the result textures
	 * if required. Always call UpdateVBO before calling GetVBOResult(), GetVAO(),
	 * GetTextureResult() or GetFBOResult().
	 */
	void UpdateVBO();
	
	/** VAO. */
	GLuint GetVAO();
	
	/** Result vbo. */
	inline GLuint GetVBOResult() const{ return pVBOResult; }
	
	/** Update results from result texture. */
	void UpdateResults();
	
	/** Result at index. */
	bool GetResultAt( int index ) const;
	
	/** Result size. */
	inline const decPoint &GetResultSize() const{ return pResultSize; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateVBOLayout();
	void pResizeResult( const decPoint &size );
	void pUpdateTexturesFBO();
};

#endif
