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

#ifndef _DEOGLRENDERTASKINSTANCE_H_
#define _DEOGLRENDERTASKINSTANCE_H_

#include "../../deoglBasics.h"

#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/collection/decTList.h>

class deoglShaderParameterBlock;
class deoglRenderTaskSharedInstance;



/**
 * Render task instance.
 */
class deoglRenderTaskInstance{
public:
	using Ref = deTUniqueReference<deoglRenderTaskInstance>;
	
	/** Sub instance. */
	struct sSubInstance{
		int instance;
		int flags;
	};
	
	
	
private:
	const deoglRenderTaskSharedInstance *pInstance;
	
	decTList<sSubInstance> pSubInstances;
	deoglShaderParameterBlock *pSIIndexInstanceSPB;
	int pSIIndexInstanceFirst;
	int pDrawIndirectIndex;
	int pDrawIndirectCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task instance. */
	deoglRenderTaskInstance();
	
	/** Clean up render task instance. */
	~deoglRenderTaskInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Shared render task instance. */
	inline const deoglRenderTaskSharedInstance *GetInstance() const{ return pInstance; }
	
	/** Set shared render task instance. */
	void SetInstance(const deoglRenderTaskSharedInstance *instance);
	
	
	
	/** Sub instances to render. */
	inline decTList<sSubInstance> &GetSubInstances(){ return pSubInstances; }
	inline const decTList<sSubInstance> &GetSubInstances() const{ return pSubInstances; }
	
	/** Sub instance index SPB. */
	inline deoglShaderParameterBlock *GetSIIndexInstanceSPB() const{ return pSIIndexInstanceSPB; }
	
	/** Index of first sub instance index for instance shared SPB. */
	inline int GetSIIndexInstanceFirst() const{ return pSIIndexInstanceFirst; }
	
	/** Set sub instance SPB information for later update. */
	void SetSIIndexInstanceParam(deoglShaderParameterBlock *paramBlock, int firstIndex);
	
	/** Write sub instance indices for instance shared SPB. */
	void WriteSIIndexInstanceInt(bool useFlags);
	void WriteSIIndexInstanceCompute();
	
	/** Draw indirect index. */
	inline int GetDrawIndirectIndex() const{ return pDrawIndirectIndex; }
	
	/** Set draw indirect index. */
	void SetDrawIndirectIndex(int index);
	
	/** Draw indirect count. */
	inline int GetDrawIndirectCount() const{ return pDrawIndirectCount; }
	
	/** Set draw indirect count. */
	void SetDrawIndirectCount(int count);
	
	
	
	/** Reset. */
	void Reset();
	/*@}*/
};

#endif
