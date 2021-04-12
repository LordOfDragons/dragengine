/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLGIINSTANCE_H_
#define _DEOGLGIINSTANCE_H_

#include "../component/deoglComponentListener.h"

#include <dragengine/deObject.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglRComponent;
class deoglGIBVHLocal;
class deoglGIBVHDynamic;
class deoglTexUnitsConfig;


/**
 * Global illumination instance.
 */
class deoglGIInstance : public deObject{
private:
	class cComponentListener : public deoglComponentListener {
	private:
		deoglGIInstance &pInstance;
		
	public:
		cComponentListener( deoglGIInstance &instance );
		virtual void ComponentDestroyed( deoglRComponent &component );
		virtual void ParentWorldChanged( deoglRComponent &component );
		virtual void LayerMaskChanged( deoglRComponent &component );
		virtual void BoundariesChanged( deoglRComponent &component );
		virtual void OcclusionMeshChanged( deoglRComponent &component );
		virtual void TexturesChanged( deoglRComponent &component );
	};
	
	
	
	deoglRComponent *pComponent;
	deoglRComponent *pOcclusionMesh;
	deObjectReference pComponentListener;
	
	deoglGIBVHLocal *pGIBVHLocal;
	deoglGIBVHDynamic *pGIBVHDynamic;
	
	decVector pMinExtend;
	decVector pMaxExtend;
	int pIndexNodes;
	int pIndexFaces;
	int pIndexVertices;
	bool pHasBVHNodes;
	
	decPointerList pTUCs;
	bool pDirtyTUCs;
	
	bool pDynamic;
	bool pChanged;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination instance. */
	deoglGIInstance();
	
	/** Clean up global illumination instance. */
	virtual ~deoglGIInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Component or NULL. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Set component or NULL. */
	void SetComponent( deoglRComponent *component, bool dynamic );
	
	/** Occlusion mesh or NULL. */
	inline deoglRComponent *GetOcclusionMesh() const{ return pOcclusionMesh; }
	
	/** Set occlusion mesh or NULL. */
	void SetOcclusionMesh( deoglRComponent *occlusionMesh, bool dynamic );
	
	
	
	/** Local GI BVH or NULL. */
	inline deoglGIBVHLocal *GetGIBVHLocal() const{ return pGIBVHLocal; }
	
	/** Dynamic GI BVH or NULL. */
	inline deoglGIBVHDynamic *GetGIBVHDynamic() const{ return pGIBVHDynamic; }
	
	/** Local minimum extend for BVH. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Local maximum extend for BVH. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** First BVH node index. */
	inline int GetIndexNodes() const{ return pIndexNodes; }
	
	/** First face index. */
	inline int GetIndexFaces() const{ return pIndexFaces; }
	
	/** First vertex index. */
	inline int GetIndexVertices() const{ return pIndexVertices; }
	
	/** BVH has nodes. */
	inline bool GetHasBVHNodes() const{ return pHasBVHNodes; }
	
	/** Update extends. */
	void UpdateExtends();
	
	
	
	/** Slot is dynamic. */
	inline bool GetDynamic() const{ return pDynamic; }
	
	/** Instance changed in a way ray cast results are invalidated. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** Set instance changed in a way ray cast results are invalidated. */
	void SetChanged( bool changed );
	
	/** Slot is empty. */
	inline bool Empty() const{ return pComponent == NULL; }
	
	/** Slot is not empty. */
	inline bool NotEmpty() const{ return ! Empty(); }
	
	/** Clear instance. */
	void Clear();
	
	
	
	/** Count of TUCs. Has to match component texture count. */
	int GetTUCCount() const;
	
	/** Get TUC at index. TUC can be NULL. */
	deoglTexUnitsConfig *GetTUCAt( int index ) const;
	
	/** Remove all TUCs. */
	void RemoveAllTUCs();
	
	/** Add TUC. TUC can be NULL. */
	void AddTUC( deoglTexUnitsConfig *tuc );
	
	/** TUCs are dirty. */
	inline bool GetDirtyTUCs() const{ return pDirtyTUCs; }
	
	/** Set TUCs dirty. */
	void SetDirtyTUCs( bool dirty );
	/*@}*/
	
	
	
private:
	void pInitParameters();
};

#endif
