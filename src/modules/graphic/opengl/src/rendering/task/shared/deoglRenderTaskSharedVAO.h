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

#ifndef _DEOGLRENDERTASKSHAREDVAO_H_
#define _DEOGLRENDERTASKSHAREDVAO_H_

#include <dragengine/deObject.h>

class deoglVAO;
class deoglRenderTaskSharedPool;


/**
 * Shared render task shader.
 */
class deoglRenderTaskSharedVAO : public deObject{
public:
	typedef deTObjectReference<deoglRenderTaskSharedVAO> Ref;
	
	
private:
	deoglRenderTaskSharedPool &pPool;
	const int pIndex;
	
	deoglVAO *pVAO;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared render task shader. */
	deoglRenderTaskSharedVAO(deoglRenderTaskSharedPool &pool, int index);
	
protected:
	/** Clean up shared render task shader. */
	virtual ~deoglRenderTaskSharedVAO();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pool. */
	inline deoglRenderTaskSharedPool &GetPool() const{ return pPool; }
	
	/** Index. */
	inline int GetIndex() const{ return pIndex; }
	
	
	
	/** VAO. */
	inline deoglVAO *GetVAO() const{ return pVAO; }
	
	/** Set vao. */
	void SetVAO(deoglVAO *shader);
	
	
	
	/** Clear. */
	void Clear();
	
	/** Return to pool. */
	void ReturnToPool();
	/*@}*/
};

#endif
