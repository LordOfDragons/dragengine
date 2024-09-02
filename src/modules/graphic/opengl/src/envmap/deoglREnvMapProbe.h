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

#ifndef _DEOGLRENVMAPPROBE_H_
#define _DEOGLRENVMAPPROBE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class decShape;
class decShapeList;
class deoglRenderThread;
class deoglRWorld;
class deoglWorldOctree;
class deoglEnvironmentMap;


/**
 * Render environment map probe.
 */
class deoglREnvMapProbe : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	bool pDirtyOctreeNode;
	
	float pInfluenceBorderSize;
	
	decDMatrix pMatrix;
	
	deoglEnvironmentMap *pEnvMap;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render environment map probe. */
	deoglREnvMapProbe( deoglRenderThread &renderThread );
	
	/** Clean up render environment map probe. */
	virtual ~deoglREnvMapProbe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL if not set. */
	void SetParentWorld( deoglRWorld *world );
	
	/** Octree node or \em NULL if there is none. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** Set octree node or \em NULL if there is none. */
	void SetOctreeNode( deoglWorldOctree *node );
	
	
	
	/** Get influence border size. */
	inline float GetInfluenceBorderSize() const{ return pInfluenceBorderSize; }
	
	/** Set influence border size. */
	void SetInfluenceBorderSize( float borderSize );
	
	
	
	/** Retrieve matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Set matrix. */
	void SetMatrix( const decDMatrix &matrix );
	
	
	
	/** Retrieves the environment map or NULL if not existing. */
	inline deoglEnvironmentMap *GetEnvironmentMap() const{ return pEnvMap; }
	
	
	
	/** Update influence shape. */
	void UpdateInfluenceShape( const decShapeList &shapeList );
	
	/** Update reflection shape. */
	void UpdateReflectionShape( const decShapeList &maskShapeList, decShape *shape );
	
	
	
	/** Prepare for quick disposal of environment map probe. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
