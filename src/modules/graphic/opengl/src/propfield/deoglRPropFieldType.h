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
class deoglWorldCompute;

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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRPropFieldType> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create prop field type. */
	deoglRPropFieldType(deoglRPropField &propField);
	
	/** Clean up prop field type. */
	virtual ~deoglRPropFieldType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Prop field. */
	inline deoglRPropField &GetPropField() const{return pPropField;}
	
	
	
	/** Model or NULL if not set. */
	inline deoglRModel *GetModel() const{return pModel;}
	
	/**
	 * Set model or NULL if not set.
	 * \warning Called during synchronization from main thread.
	 */
	void SetModel(deoglRModel *model);
	
	/** Skin or NULL if not set. */
	inline deoglRSkin *GetSkin() const{return pSkin;}
	
	/**
	 * Set skin or NULL if not set.
	 * \warning Called during synchronization from main thread.
	 */
	void SetSkin(deoglRSkin *skin);
	
	/** Skin texture to use or NULL if not valid. */
	inline deoglSkinTexture *GetUseSkinTexture() const{return pUseSkinTexture;}
	
	
	
	/** Minimum extend. */
	inline const decVector &GetMinimumExtend() const{return pMinExtend;}
	
	/** Maximum extend. */
	inline const decVector &GetMaximumExtend() const{return pMaxExtend;}
	
	/** Bending factor. */
	inline float GetBendFactor() const{return pBendFactor;}
	
	
	
	/**
	 * Rebuild instances.
	 * \warning Called during synchronization from main thread.
	 */
	void RebuildInstances(const dePropFieldType &type);
	
	/** Add clusters with a point sieve. */
	void AddClustersWithSieve(const dePropFieldType &type);
	
	/** Add clusters with a cluster generator. */
	void AddClustersWithGenerator(const dePropFieldType &type);
	
	/** Add clusters from a cluster generator. */
	void AddClustersFromGenerator(const dePropFieldType &type, const deoglPFClusterGenerator &generator);
	
	
	
	/** Prepare for render. Called by deoglRWorld if registered previously. */
	void PrepareForRender();
	
	/** Update instances. */
	void UpdateInstances(const decDVector &cameraPosition, const decDMatrix &cameraMatrix);
	
	
	
	/** Number of clusters. */
	int GetClusterCount() const;
	
	/** Cluster at index. */
	deoglPropFieldCluster *GetClusterAt(int index) const;
	
	/** Add cluster. */
	void AddCluster(deoglPropFieldCluster *cluster);
	
	/** Remove all clusters. */
	void RemoveAllClusters();
	
	/** Cluster requires prepare for render. */
	void ClusterRequiresPrepareForRender();
	
	
	
	/**
	 * Prepare bend states.
	 * \warning Called during synchronization from main thread.
	 */
	void PrepareBendStateData(const dePropFieldType &type);
	
	
	
	/** Parameter block or NULL if there is no valid skin texture. */
	inline const deoglSPBlockUBO::Ref &GetParamBlock() const{return pParamBlock;}
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	/** Mark parameter blocks dirty. */
	void MarkParamBlocksDirty();
	
	/** Marks texture units configurations dirty. */
	void MarkTUCsDirty();
	
	/** Update instance parameter shader parameter block. */
	void UpdateInstanceParamBlock(deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader);
	
	
	
	/** World reference point changed. */
	void WorldReferencePointChanged();
	
	
	
	/** Add to world compute. */
	void AddToWorldCompute(deoglWorldCompute &worldCompute);
	
	/** Update world compute. */
	void UpdateWorldCompute();
	
	/** Update world compute textures. */
	void UpdateWorldComputeTextures();
	
	/** Remove from world compute. */
	void RemoveFromWorldCompute();
	/*@}*/
	
	
	
private:
	void pPrepareModel();
	void pPrepareParamBlock();
};

#endif
