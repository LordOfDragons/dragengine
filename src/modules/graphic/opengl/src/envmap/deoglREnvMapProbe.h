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
 * \brief Render environment map probe.
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
	/** \brief Create render environment map probe. */
	deoglREnvMapProbe( deoglRenderThread &renderThread );
	
	/** \brief Clean up render environment map probe. */
	virtual ~deoglREnvMapProbe();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL if not set. */
	void SetParentWorld( deoglRWorld *world );
	
	/** \brief Octree node or \em NULL if there is none. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set octree node or \em NULL if there is none. */
	void SetOctreeNode( deoglWorldOctree *node );
	
	
	
	/** \brief Get influence border size. */
	inline float GetInfluenceBorderSize() const{ return pInfluenceBorderSize; }
	
	/** \brief Set influence border size. */
	void SetInfluenceBorderSize( float borderSize );
	
	
	
	/** \brief Retrieve matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Set matrix. */
	void SetMatrix( const decDMatrix &matrix );
	
	
	
	/** Retrieves the environment map or NULL if not existing. */
	inline deoglEnvironmentMap *GetEnvironmentMap() const{ return pEnvMap; }
	
	
	
	/** \brief Update influence shape. */
	void UpdateInfluenceShape( const decShapeList &shapeList );
	
	/** \brief Update reflection shape. */
	void UpdateReflectionShape( const decShapeList &maskShapeList, decShape *shape );
	
	
	
	/** \brief Prepare for quick disposal of environment map probe. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
