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
 * @brief Occlusion Test.
 * Provides support for occlusion tests using a simplified hardware approach.
 * Depending on the screen size the starting lod level of the occlusion map
 * can be altered.
 */
class deoglOcclusionTest{
private:
	struct sInputData{
		oglVector3 minExtend;
		oglVector3 maxExtend;
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglOcclusionMap *pOcclusionMapMain;
	deoglOcclusionMap *pOcclusionMapMask;
	deoglOcclusionMap *pOcclusionMapSky;
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
	int pResultWidth;
	int pResultHeight;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new occlusion test object. */
	deoglOcclusionTest( deoglRenderThread &renderThread );
	/** Cleans up the occlusion test. */
	~deoglOcclusionTest();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the main occlusion map. */
	inline deoglOcclusionMap *GetOcclisionMapMain() const{ return pOcclusionMapMain; }
	/** Retrieves the mask occlusion map. */
	inline deoglOcclusionMap *GetOcclisionMapMask() const{ return pOcclusionMapMask; }
	/** Retrieves the sky occlusion map. */
	inline deoglOcclusionMap *GetOcclisionMapSky() const{ return pOcclusionMapSky; }
	/** Retrieves the result texture. */
	inline deoglTexture *GetTextureResult() const{ return pTextureResult; }
	/** Retrieves the result fbo. */
	inline deoglFramebuffer *GetFBOResult() const{ return pFBOResult; }
	
	/** Retrieves the number of input data. */
	inline int GetInputDataCount() const{ return pInputDataCount; }
	/** Retrieves the input data (for debugging purpose only). */
	inline void *GetInputData() const{ return pInputData; }
	/**
	 * Adds an input data returning the index to fetch the result later. The test box is defined using
	 * the world minimum and maximum extends translated translated by the camera position. Hence only
	 * the translation part of the camera matrix has to be applied to the world box extends before
	 * using them as input data. The rotation and projection is handled on the shader side.
	 */
	int AddInputData( const decVector &minExtend, const decVector &maxExtend, deoglOcclusionTestListener *listener );
	/** Remove all input data. */
	void RemoveAllInputData();
	
	/** Update VBO using the collected input data. */
	void UpdateVBO();
	/** Retrieves the VAO. */
	GLuint GetVAO();
	/** Retrieves the result vbo. */
	inline GLuint GetVBOResult() const{ return pVBOResult; }
	/** Updates the results from the result texture. */
	void UpdateResults();
	
	/** Retrieves the result at the given index. */
	bool GetResultAt( int index ) const;
	/** Retrieves the result width. */
	inline int GetResultWidth() const{ return pResultWidth; }
	/** Retrieves the result height. */
	inline int GetResultHeight() const{ return pResultHeight; }
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateFBO();
	void pCreateVBOLayout();
	void pResizeResult( int width, int height );
};

#endif
