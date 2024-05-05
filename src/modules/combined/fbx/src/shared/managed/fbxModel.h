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
 * FBX managed model.
 */
class fbxModel : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<fbxModel> Ref;
	
	
	
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
	
	deModelWeight *pVertexWeights;
	int pVertexWeightCount;
	int pVertexWeightSize;
	
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
	
	bool pCulling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create connection. */
	fbxModel( fbxScene &scene, fbxNode &nodeGeometry );
	
protected:
	/** Clean up connection. */
	virtual ~fbxModel();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Scene. */
	inline fbxScene &GetScene() const{ return pScene; }
	
	/** Geometry node. */
	inline fbxNode &GetNodeGeometry() const{ return pNodeGeomtry; }
	
	/** Model node. */
	inline fbxNode &GetNodeModel() const{ return *pNodeModel; }
	
	/** Deformer node. */
	inline fbxNode *GetNodeDeformer() const{ return pNodeDeformer; }
	
	/** Geometry ID. */
	inline int64_t GetGeometryID() const{ return pGeometryID; }
	
	/** Model ID. */
	inline int64_t GetModelID() const{ return pModelID; }
	
	/** Deformer ID. */
	inline int64_t GetDeformerID() const{ return pDeformerID; }
	
	
	
	/** Count of clusters. */
	int GetClusterCount() const;
	
	/** Cluster at index. */
	fbxModelCluster *GetClusterAt( int index ) const;
	
	/** Named cluster or NULL if absent. */
	fbxModelCluster *GetClusterNamed( const char *name ) const;
	
	/** Match clusters against rig. */
	void MatchClusters( const fbxRig &rig );
	
	/** Build weights. */
	void BuildWeights();
	
	/** Count of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** Vertex position. */
	inline const decVector &GetVertexPositionAt( int index ) const{ return pVertices[ index ].position; }
	
	/** Vertex weight set. */
	inline int GetVertexWeightSetAt( int index ) const{ return pVertices[ index ].weightSet; }
	
	/** Get weight. */
	const deModelWeight &GetWeightAt( int index ) const;
	
	/** Weight sets. */
	inline const decIntList &GetWeightSetWeights() const{ return pWeightSetWeights; }
	inline const decIntList &GetWeightSetsFirstWeight() const{ return pWeightSetsFirstWeight; }
	inline const decIntList &GetWeightSetsWeightsCount() const{ return pWeightSetsWeightsCount; }
	inline const decIntList &GetWeightGroupsSetCount() const{ return pWeightGroupsSetCount; }
	
	
	
	/** Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Culling. */
	inline bool GetCulling() const{ return pCulling; }
	
	
	
	/** Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	int pAddVertexWeight( int bone, float weight );
	int pAddWeight( int bone, float weight );
	int pAddWeightSet( const decIntList &weights );
};

#endif
