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

#ifndef _DEMODELLOD_H_
#define _DEMODELLOD_H_

#include "deModelWeight.h"
#include "deModelVertex.h"
#include "deModelFace.h"
#include "deModelTextureCoordinatesSet.h"
#include "deModelLodVertexPositionSet.h"
#include "../../common/collection/decTList.h"
#include "../../common/collection/decTUniqueList.h"
#include "../../common/math/decMath.h"


/**
 * \brief Model LOD.
 * 
 * Stores a single level of detail mesh for the model. Stores weights, vertices, faces and
 * texture coordinate sets. Bones and textures are shared across all LOD meshes. To remove
 * wasted memory the weights, vertices and faces are stored in flattened arrays. The weights
 * are stored in two different flattened arrays which can not be indexed directly as weight
 * sets have usually different number of weights. All weight sets are stored one after the
 * other in the weights array sorted by the number of weights they contain. Thus first come
 * all weight sets with one bone, then all with two bones and so forth. The second array
 * stores the number of weight sets for each run of weight sets with the same number of
 * bones. This array too starts first with the count for all weight sets with one bone then
 * the count for all weight sets with two bones and so forth. The weight sets are indexed
 * in the sequence they are stored.
 */
class DE_DLL_EXPORT deModelLOD{
public:
	/** \brief Reference type. */
	using Ref = deTUniqueReference<deModelLOD>;
	
	/** \brief List type. */
	using List = decTUniqueList<deModelLOD>;
	
private:
	decTList<deModelWeight> pWeights;
	decTList<int> pWeightGroups;
	
	decTList<deModelVertex> pVertices;
	
	int pNormalCount;
	int pTangentCount;
	
	decTList<deModelFace> pFaces;
	
	decTList<deModelTextureCoordinatesSet> pTextureCoordinatesSets;
	int pTextureCoordinatesCount;
	
	bool pHasLodError;
	float pLodError;
	
	decTList<deModelLodVertexPositionSet> pVertexPositionSets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model lod object. */
	deModelLOD();
	
	/** \brief Clean up model lod object. */
	~deModelLOD();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Has custom lod error. */
	inline bool GetHasLodError() const{ return pHasLodError; }
	
	/** \brief Set has custom lod error. */
	void SetHasLodError(bool hasError);
	
	/** \brief Custom lod error. */
	inline float GetLodError() const{ return pLodError; }
	
	/** \brief Set custom lod error. */
	void SetLodError(float error);
	/*@}*/
	
	
	
	/** \name Weight Sets Management */
	/*@{*/
	/** \brief Weights. */
	inline decTList<deModelWeight> &GetWeights(){ return pWeights; }
	inline const decTList<deModelWeight> &GetWeights() const{ return pWeights; }
	
	/** \brief Weight groups. */
	inline decTList<int> &GetWeightGroups(){ return pWeightGroups; }
	inline const decTList<int> &GetWeightGroups() const{ return pWeightGroups; }
	/*@}*/
	
	
	
	/** \name Vertex Management */
	/*@{*/
	/** \brief Vertices. */
	inline decTList<deModelVertex> &GetVertices(){ return pVertices; }
	inline const decTList<deModelVertex> &GetVertices() const{ return pVertices; }
	/*@}*/
	
	
	
	/** \name Face Management */
	/*@{*/
	/** \brief Count of normals. */
	inline int GetNormalCount() const{ return pNormalCount; }
	
	/** \brief Set count of normals. */
	void SetNormalCount(int count);
	
	/** \brief Count of tangents. */
	inline int GetTangentCount() const{ return pTangentCount; }
	
	/** \brief Set count of tangents. */
	void SetTangentCount(int count);
	
	/** \brief Faces. */
	inline decTList<deModelFace> &GetFaces(){ return pFaces; }
	inline const decTList<deModelFace> &GetFaces() const{ return pFaces; }
	/*@}*/
	
	
	
	/** \name Texture Coordinates. */
	/*@{*/
	/** \brief Number of texture coordinates per set. */
	inline int GetTextureCoordinatesCount() const{ return pTextureCoordinatesCount; }
	
	/** \brief Set number of texture coordinates per set. */
	void SetTextureCoordinatesCount(int count);
	
	/** \brief Texture coordinates sets. */
	inline decTList<deModelTextureCoordinatesSet> &GetTextureCoordinatesSets(){ return pTextureCoordinatesSets; }
	inline const decTList<deModelTextureCoordinatesSet> &GetTextureCoordinatesSets() const{ return pTextureCoordinatesSets; }
	/*@}*/
	
	
	
	/** \name Vertex position sets. */
	/*@{*/
	/** \brief Vertex position sets. */
	inline decTList<deModelLodVertexPositionSet> &GetVertexPositionSets(){ return pVertexPositionSets; }
	inline const decTList<deModelLodVertexPositionSet> &GetVertexPositionSets() const{ return pVertexPositionSets; }
	/*@}*/
};

#endif
