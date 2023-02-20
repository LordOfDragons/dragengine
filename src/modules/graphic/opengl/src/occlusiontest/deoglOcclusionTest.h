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
#include "../shaders/paramblock/deoglSPBlockSSBO.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglOcclusionMap;
class deoglOcclusionTestListener;
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
	
	deoglSPBlockSSBO::Ref pSSBOInput;
	deoglSPBlockSSBO::Ref pSSBOResult;

	deoglOcclusionTestListener **pInputListeners;
	sInputData *pInputData;
	int pInputDataCount;
	int pInputDataSize;
	
	
	
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
	/** Count of input data. */
	inline int GetInputDataCount() const{ return pInputDataCount; }
	
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
	
	/** Update input data SSBO. */
	void UpdateSSBO();
	
	/** Input data SSBO. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOInput() const{ return pSSBOInput; }
	
	/** Result data SSBO. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOResult() const{ return pSSBOResult; }
	
	/** Update results from SSBO. */
	void UpdateResults();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pResizeInputData( int size );
};

#endif
