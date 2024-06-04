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

#ifndef _DEOGLRENDERTASKVAO_H_
#define _DEOGLRENDERTASKVAO_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

#include "../../deoglBasics.h"

class deoglQuickSorter;
class deoglRenderTaskInstance;
class deoglSharedSPB;
class deoglRenderTaskSharedVAO;
class deoglRenderTaskSharedInstance;


/**
 * Render Task VAO.
 */
class deoglRenderTaskVAO{
private:
	const deoglRenderTaskSharedVAO *pVAO;
	
	decPointerList pInstances;
	int pInstanceCount;
	
	deoglRenderTaskInstance **pHasInstance;
	int pHasInstanceCount;
	int pHasInstanceSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render task vao. */
	deoglRenderTaskVAO();
	
	/** Clean up render task vao. */
	~deoglRenderTaskVAO();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset vao. */
	void Reset();
	
	
	
	/** Total amount of points. */
	int GetTotalPointCount() const;
	
	/** Total amount of sub-instances. */
	int GetTotalSubInstanceCount() const;
	
	
	
	/** VAO. */
	inline const deoglRenderTaskSharedVAO *GetVAO() const{ return pVAO; }
	
	/** Set VAO. */
	void SetVAO( const deoglRenderTaskSharedVAO *vao );
	
	
	
	/** Number of render task instances. */
	inline int GetInstanceCount() const{ return pInstanceCount; }
	
	/** Render task instance at index. */
	deoglRenderTaskInstance *GetInstanceAt( int index ) const;
	
	/** Add render task instance. */
	deoglRenderTaskInstance *AddInstance( const deoglRenderTaskSharedInstance *instance );
	
	/** Sort instances by distance. */
	void SortInstancesByDistance( deoglQuickSorter &sorter, const decDVector &position,
		const decDVector &direction, double posDotDir );
	/*@}*/
};

#endif
