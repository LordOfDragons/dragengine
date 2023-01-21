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

#ifndef _DEOGLRPROPFIELDTYPE_H_
#define _DEOGLRPROPFIELDTYPE_H_

#include "../skin/deoglSkinTexture.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/deObject.h>

class deoglPFClusterGenerator;
class deoglPropFieldCluster;
class deoglRModel;
class deoglRPropField;
class deoglRSkin;
class deoglSkinShader;
class deoglSkinTexture;
class deoglTexUnitsConfig;

class dePropFieldType;


/**
 * Render prop field type.
 */
class deoglRPropFieldType : public deObject{
private:
	deoglRPropField &pPropField;
	
	deoglRModel *pModel;
	deoglRSkin *pSkin;
	
	deoglSkinTexture *pUseSkinTexture;
	
	decPointerList pClusters;
	bool pClustersRequirePrepareForRender;
	
	decVector pMinExtend;
	decVector pMaxExtend;
	float pBendFactor;
	
	deoglSPBlockUBO::Ref pParamBlock;
	
	bool pValidParamBlock;
	bool pDirtyParamBlock;
	
	bool pDirtyModel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create prop field type. */
	deoglRPropFieldType( deoglRPropField &propField );
	
	/** Clean up prop field type. */
	virtual ~deoglRPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Prop field. */
	inline deoglRPropField &GetPropField() const{ return pPropField; }
	
	
	
	/** Model or NULL if not set. */
	inline deoglRModel *GetModel() const{ return pModel; }
	
	/**
	 * Set model or NULL if not set.
	 * \warning Called during synchronization from main thread.
	 */
	void SetModel( deoglRModel *model );
	
	/** Skin or NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/**
	 * Set skin or NULL if not set.
	 * \warning Called during synchronization from main thread.
	 */
	void SetSkin( deoglRSkin *skin );
	
	/** Skin texture to use or NULL if not valid. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	
	
	/** Minimum extend. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Bending factor. */
	inline float GetBendFactor() const{ return pBendFactor; }
	
	
	
	/**
	 * Rebuild instances.
	 * \warning Called during synchronization from main thread.
	 */
	void RebuildInstances( const dePropFieldType &type );
	
	/** Add clusters with a point sieve. */
	void AddClustersWithSieve( const dePropFieldType &type );
	
	/** Add clusters with a cluster generator. */
	void AddClustersWithGenerator( const dePropFieldType &type );
	
	/** Add clusters from a cluster generator. */
	void AddClustersFromGenerator(  const dePropFieldType &type, const deoglPFClusterGenerator &generator );
	
	
	
	/** Prepare for render. Called by deoglRWorld if registered previously. */
	void PrepareForRender();
	
	/** Update instances. */
	void UpdateInstances( const decDVector &cameraPosition, const decDMatrix &cameraMatrix );
	
	
	
	/** Number of clusters. */
	int GetClusterCount() const;
	
	/** Cluster at index. */
	deoglPropFieldCluster *GetClusterAt( int index ) const;
	
	/** Add cluster. */
	void AddCluster( deoglPropFieldCluster *cluster );
	
	/** Remove all clusters. */
	void RemoveAllClusters();
	
	/** Cluster requires prepare for render. */
	void ClusterRequiresPrepareForRender();
	
	
	
	/**
	 * Prepare bend states.
	 * \warning Called during synchronization from main thread.
	 */
	void PrepareBendStateData( const dePropFieldType &type );
	
	
	
	/** Parameter block or NULL if there is no valid skin texture. */
	inline const deoglSPBlockUBO::Ref &GetParamBlock() const{ return pParamBlock; }
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock( deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader );
	
	
	
	/** World reference point changed. */
	void WorldReferencePointChanged();
	/*@}*/
	
	
	
private:
	void pPrepareModel();
	void pPrepareParamBlock();
};

#endif
