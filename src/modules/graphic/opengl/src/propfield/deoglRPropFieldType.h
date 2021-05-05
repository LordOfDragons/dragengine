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

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/deObject.h>
#include "../skin/deoglSkinTexture.h"

class deoglPFClusterGenerator;
class deoglPropFieldCluster;
class deoglRModel;
class deoglRPropField;
class deoglRSkin;
class deoglSPBlockUBO;
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
	
	decVector pMinExtend;
	decVector pMaxExtend;
	float pBendFactor;
	
	deoglSPBlockUBO *pParamBlock;
	
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
	
	
	
	/** Model or \em NULL if not set. */
	inline deoglRModel *GetModel() const{ return pModel; }
	
	/** Set model or \em NULL if not set. */
	void SetModel( deoglRModel *model );
	
	/** Skin or \em NULL if not set. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin or \em NULL if not set. */
	void SetSkin( deoglRSkin *skin );
	
	/** Skin texture to use or \em NULL if not valid. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	
	
	/** Minimum extend. */
	inline const decVector &GetMinimumExtend() const{ return pMinExtend; }
	
	/** Maximum extend. */
	inline const decVector &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** Bending factor. */
	inline float GetBendFactor() const{ return pBendFactor; }
	
	
	
	/** Rebuild instances. */
	void RebuildInstances( const dePropFieldType &type );
	
	/** Add clusters with a point sieve. */
	void AddClustersWithSieve( const dePropFieldType &type );
	
	/** Add clusters with a cluster generator. */
	void AddClustersWithGenerator( const dePropFieldType &type );
	
	/** Add clusters from a cluster generator. */
	void AddClustersFromGenerator(  const dePropFieldType &type, const deoglPFClusterGenerator &generator );
	
	
	
	/** Prepare for rendering. */
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
	
	
	
	/** Prepare bend states. */
	void PrepareBendStateData( const dePropFieldType &type );
	
	
	
	/** Shader parameter block for a shader type. */
	deoglSPBlockUBO *GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType );
	
	/**
	 * Sarameter block or \em NULL if there is no valid skin texture.
	 * \details This texture units configuration works for the shader types estComponent*.
	 */
	deoglSPBlockUBO *GetParamBlock();
	
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
};

#endif
