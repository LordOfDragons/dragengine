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
	deoglOcclusionTest(deoglRenderThread &renderThread);
	
	/** Clean up occlusion test. */
	~deoglOcclusionTest();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of input data. */
	inline int GetInputDataCount() const{return pInputDataCount;}
	
	/**
	 * Add input data returning the index to fetch the result later. The test box is
	 * defined using the world minimum and maximum extends translated translated by
	 * the camera position. Hence only the translation part of the camera matrix has
	 * to be applied to the world box extends before using them as input data.
	 * The rotation and projection is handled on the shader side.
	 */
	int AddInputData(const decVector &minExtend, const decVector &maxExtend,
		deoglOcclusionTestListener *listener);
	
	/** Remove all input data. */
	void RemoveAllInputData();
	
	/** Update input data SSBO. */
	void UpdateSSBO();
	
	/** Input data SSBO. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOInput() const{return pSSBOInput;}
	
	/** Result data SSBO. */
	inline const deoglSPBlockSSBO::Ref &GetSSBOResult() const{return pSSBOResult;}
	
	/** Update results from SSBO. */
	void UpdateResults();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pResizeInputData(int size);
};

#endif
