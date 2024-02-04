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
#include "../decal/deoglDecalListener.h"

#include <dragengine/deObject.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>

class deoglGIInstances;
class deoglRComponent;
class deoglRDecal;
class deoglGIBVHLocal;
class deoglGIBVHDynamic;
class deoglTexUnitsConfig;
class deoglDynamicTBOBlock;
class deoglDynamicTBOFloat16;
class deoglDynamicTBOUInt32;
class decLayerMask;


/**
 * Global illumination instance.
 */
class deoglGIInstance : public deObject{
private:
	class cListenerLink{
	public:
		deoglGIInstance &instance;
		
		cListenerLink( deoglGIInstance &instance );
		bool LayerMaskMatchesNot( const decLayerMask &layerMask ) const;
		bool GIImportanceMatchesNot( int importance ) const;
		void RemoveInstance() const;
		void ChangeInstance( bool hard ) const;
		void Moved() const;
		void TUCChanged() const;
		void DynamicChanged() const;
	};
	
	class cComponentListener : public deoglComponentListener{
	private:
		const cListenerLink pLink;
		
	public:
		cComponentListener( deoglGIInstance &instance );
		void ComponentDestroyed( deoglRComponent &component ) override;
		void ParentWorldChanged( deoglRComponent &component ) override;
		void LayerMaskChanged( deoglRComponent &component ) override;
		void BoundariesChanged( deoglRComponent &component ) override;
		void OcclusionMeshChanged( deoglRComponent &component ) override;
		void TexturesChanged( deoglRComponent &component ) override;
		void RenderStaticChanged( deoglRComponent &component ) override;
		void MovementHintChanged( deoglRComponent &component ) override;
		void VisibilityChanged( deoglRComponent &component) override;
		void GIImportanceChanged(deoglRComponent & component) override;
	};
	
	class cDecalListener : public deoglDecalListener{
	private:
		const cListenerLink pLink;
		
	public:
		cDecalListener( deoglGIInstance &instance );
		virtual void DecalDestroyed( deoglRDecal &decal );
		virtual void GeometryChanged( deoglRDecal &decal );
		virtual void TextureChanged( deoglRDecal &decal );
		virtual void TUCChanged( deoglRDecal &decal );
	};
	
	class cDecalComponentListener : public deoglComponentListener{
	private:
		const cListenerLink pLink;
		
	public:
		cDecalComponentListener( deoglGIInstance &instance );
		virtual void BoundariesChanged( deoglRComponent &component );
		virtual void RenderStaticChanged( deoglRComponent &component );
		virtual void MovementHintChanged( deoglRComponent &component );
	};
	
	
	
	deoglGIInstances &pInstances;
	
	deoglRComponent *pComponent;
	deObjectReference pComponentListener;
	
	deoglRDecal *pDecal;
	deObjectReference pDecalListener;
	deObjectReference pDecalComponentListener;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	
	deoglGIBVHLocal *pGIBVHLocal;
	deoglGIBVHDynamic *pGIBVHDynamic;
	decVector pBVHMinExtend;
	decVector pBVHMaxExtend;
	int pIndexNodes;
	int pIndexFaces;
	int pIndexVertices;
	bool pHasBVHNodes;
	
	decPointerList pTUCs;
	bool pDirtyTUCs;
	deObjectReference pBlockMaterial;
	deoglDynamicTBOUInt32 *pTBOMaterial;
	deoglDynamicTBOFloat16 *pTBOMaterial2;
	
	bool pDynamic;
	bool pChanged;
	bool pHardChanged;
	bool pMoved;
	bool pRecheckDynamic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create global illumination instance. */
	deoglGIInstance( deoglGIInstances &instances );
	
	/** Clean up global illumination instance. */
	virtual ~deoglGIInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Owner instances. */
	inline deoglGIInstances &GetInstances() const{ return pInstances; }
	
	/** Component or NULL. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	
	/** Set component or NULL. */
	void SetComponent( deoglRComponent *component, bool dynamic );
	
	/** Decal or NULL. */
	inline deoglRDecal *GetDecal() const{ return pDecal; }
	
	/** Set decal or NULL. */
	void SetDecal( deoglRDecal *decal, bool dynamic );
	
	
	
	/** Tracked instance minimum extend in world space. */
	inline const decDVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Tracked instance maximum extend in world space. */
	inline const decDVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Set tracked instance extends in world space. */
	void SetExtends( const decDVector &minExtend, const decDVector &maxExtend );
	
	/** Set extends from local BVH points. */
	void SetExtendsFromBVHLocal();
	
	
	
	/** Local GI BVH or NULL. */
	inline deoglGIBVHLocal *GetGIBVHLocal() const{ return pGIBVHLocal; }
	
	/** Dynamic GI BVH or NULL. */
	inline deoglGIBVHDynamic *GetGIBVHDynamic() const{ return pGIBVHDynamic; }
	
	/** Local minimum extend for BVH. */
	inline const decVector &GetBVHMinimumExtend() const{ return pBVHMinExtend; }
	
	/** Local maximum extend for BVH. */
	inline const decVector &GetBVHMaximumExtend() const{ return pBVHMaxExtend; }
	
	/** First BVH node index. */
	inline int GetIndexNodes() const{ return pIndexNodes; }
	
	/** First face index. */
	inline int GetIndexFaces() const{ return pIndexFaces; }
	
	/** First vertex index. */
	inline int GetIndexVertices() const{ return pIndexVertices; }
	
	/** BVH has nodes. */
	inline bool GetHasBVHNodes() const{ return pHasBVHNodes; }
	
	/** Update BVH local extends. */
	void UpdateBVHExtends();
	
	
	
	/** Slot is dynamic. */
	inline bool GetDynamic() const{ return pDynamic; }
	
	/** Set if slot is dynamic. */
	void SetDynamic( bool dynamic );
	
	/** Instance changed in a way ray cast results are invalidated. */
	inline bool GetChanged() const{ return pChanged; }
	
	/** Set instance changed in a way ray cast results are invalidated. */
	void SetChanged( bool changed );
	
	/** Instance changed in a way disabled probes have to be updated too. */
	inline bool GetHardChanged() const{ return pHardChanged; }
	
	/** Set instance changed in a way disabled probes have to be updated too. */
	void SetHardChanged( bool changed );
	
	/** Clear changed flags. */
	void ClearChanged();
	
	/** Instance moved. */
	inline bool GetMoved() const{ return pMoved; }
	
	/** Set instance moved. */
	void SetMoved( bool moved );
	
	/** Slot dynamic state potentially changed. */
	inline bool GetRecheckDynamic() const{ return pRecheckDynamic; }
	
	/** Set if slot dynamic state potentially changed. */
	void SetRecheckDynamic( bool recheckDynamic );
	
	/** Slot is empty. */
	bool Empty() const;
	
	/** Slot is not empty. */
	inline bool NotEmpty() const{ return ! Empty(); }
	
	/** Clear instance. */
	void Clear();
	
	
	
	/** Count of TUCs. Has to match instance texture count. */
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
	
	/** Material TBOs. */
	inline deoglDynamicTBOUInt32 *GetTBOMaterial(){ return pTBOMaterial; }
	inline deoglDynamicTBOFloat16 *GetTBOMaterial2(){ return pTBOMaterial2; }
	
	/** Get TBO block for materials. */
	deoglDynamicTBOBlock *GetBlockMaterial();
	
	/** Drop TBO blocks. */
	void DropBlockMaterial();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pInitParameters();
};

#endif
