/* 
 * FBX Modules
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

#ifndef _FBXMODEL_H_
#define _FBXMODEL_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxNode;
class fbxScene;
class fbxRig;
class fbxModelCluster;

class deBaseModule;
class deModelWeight;


/**
 * \brief FBX managed model.
 */
class fbxModel : public deObject{
private:
	struct sVertex {
		decIntList weights;
		decVector position;
		int weightSet;
	};
	
	
	fbxScene &pScene;
	fbxNode &pNodeGeomtry;
	fbxNode *pNodeModel;
	fbxNode *pNodeDeformer;
	int64_t pGeometryID;
	int64_t pModelID;
	int64_t pDeformerID;
	decObjectOrderedSet pClusters;
	decMatrix pMatrix;
	
	deModelWeight *pWeights;
	int pWeightCount;
	int pWeightSize;
	
	decIntList pWeightSetWeights;
	decIntList pWeightSetsFirstWeight;
	decIntList pWeightSetsWeightsCount;
	decIntList pWeightGroupsSetCount;
	
	float pWeightMatchThreshold;
	
	sVertex *pVertices;
	int pVertexCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	fbxModel( fbxScene &scene, fbxNode &nodeGeometry );
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxModel();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Scene. */
	inline fbxScene &GetScene() const{ return pScene; }
	
	/** \brief Geometry node. */
	inline fbxNode &GetNodeGeometry() const{ return pNodeGeomtry; }
	
	/** \brief Model node. */
	inline fbxNode &GetNodeModel() const{ return *pNodeModel; }
	
	/** \brief Deformer node. */
	inline fbxNode *GetNodeDeformer() const{ return pNodeDeformer; }
	
	/** \brief Geometry ID. */
	inline int64_t GetGeometryID() const{ return pGeometryID; }
	
	/** \brief Model ID. */
	inline int64_t GetModelID() const{ return pModelID; }
	
	/** \brief Deformer ID. */
	inline int64_t GetDeformerID() const{ return pDeformerID; }
	
	
	
	/** \brief Count of clusters. */
	int GetClusterCount() const;
	
	/** \brief Cluster at index. */
	fbxModelCluster *GetClusterAt( int index ) const;
	
	/** \brief Named cluster or NULL if absent. */
	fbxModelCluster *GetClusterNamed( const char *name ) const;
	
	/** \brief Match clusters against rig. */
	void MatchClusters( const fbxRig &rig );
	
	/** \brief Build weights. */
	void BuildWeights();
	
	/** \brief Count of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Vertex position. */
	inline const decVector &GetVertexPositionAt( int index ) const{ return pVertices[ index ].position; }
	
	/** \brief Vertex weight set. */
	inline int GetVertexWeightSetAt( int index ) const{ return pVertices[ index ].weightSet; }
	
	/** \brief Get weight. */
	const deModelWeight &GetWeightAt( int index ) const;
	
	/** \brief Weight sets. */
	inline const decIntList &GetWeightSetWeights() const{ return pWeightSetWeights; }
	inline const decIntList &GetWeightSetsFirstWeight() const{ return pWeightSetsFirstWeight; }
	inline const decIntList &GetWeightSetsWeightsCount() const{ return pWeightSetsWeightsCount; }
	inline const decIntList &GetWeightGroupsSetCount() const{ return pWeightGroupsSetCount; }
	
	
	
	/** \brief Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	int pAddWeight( int bone, float weight );
	int pAddWeightSet( const decIntList &weights );
};

#endif
