/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEMODELLOD_H_
#define _DEMODELLOD_H_

#include "../../common/math/decMath.h"

class deModelWeight;
class deModelVertex;
class deModelFace;
class deModelTextureCoordinatesSet;


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
class deModelLOD{
private:
	deModelWeight *pWeights;
	int pWeightCount;
	int *pWeightGroups;
	int pWeightGroupCount;
	
	deModelVertex *pVertices;
	int pVertexCount;
	
	int pNormalCount;
	int pTangentCount;
	
	deModelFace *pFaces;
	int pFaceCount;
	
	deModelTextureCoordinatesSet *pTextureCoordinatesSets;
	int pTextureCoordinatesSetsCount;
	int pTextureCoordinatesCount;
	
	bool pHasLodError;
	float pLodError;
	
	
	
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
	void SetHasLodError( bool hasError );
	
	/** \brief Custom lod error. */
	inline float GetLodError() const{ return pLodError; }
	
	/** \brief Set custom lod error. */
	void SetLodError( float error );
	/*@}*/
	
	
	
	/** \name Weight Sets Management */
	/*@{*/
	/** \brief Number of weights. */
	inline int GetWeightCount() const{ return pWeightCount; }
	
	/** \brief Set number of weights. */
	void SetWeightCount( int count );
	
	/** \brief Weight at the given position. */
	deModelWeight &GetWeightAt( int index ) const;
	
	/** \brief Pointer to the weights. */
	inline deModelWeight *GetWeights() const{ return pWeights; }
	
	/** \brief Number of weight groups. */
	inline int GetWeightGroupCount() const{ return pWeightGroupCount; }
	
	/** \brief Set number of weight groups. */
	void SetWeightGroupCount( int count );
	
	/** \brief Weight group at the given position. */
	int GetWeightGroupAt( int index ) const;
	
	/** \brief Set weight group at the given position. */
	void SetWeightGroupAt( int index, int weightSetCount ) const;
	
	/** \brief Pointer to the weight groups. */
	inline int *GetWeightGroups() const{ return pWeightGroups; }
	/*@}*/
	
	
	
	/** \name Vertex Management */
	/*@{*/
	/** \brief Count of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Set number of vertices. */
	void SetVertexCount( int count );
	
	/** \brief Vertex at the given index. */
	deModelVertex &GetVertexAt( int index ) const;
	
	/** \brief Pointer to the vertices. */
	inline deModelVertex *GetVertices() const{ return pVertices; }
	/*@}*/
	
	
	
	/** \name Face Management */
	/*@{*/
	/** \brief Count of normals. */
	inline int GetNormalCount() const{ return pNormalCount; }
	
	/** \brief Set count of normals. */
	void SetNormalCount( int count );
	
	/** \brief Count of tangents. */
	inline int GetTangentCount() const{ return pTangentCount; }
	
	/** \brief Set count of tangents. */
	void SetTangentCount( int count );
	
	/** \brief Count of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Set number of faces. */
	void SetFaceCount( int count );
	
	/** \brief Face at the given index. */
	deModelFace &GetFaceAt( int index ) const;
	
	/** \brief Pointer to the faces. */
	inline deModelFace *GetFaces() const{ return pFaces; }
	/*@}*/
	
	
	
	/** \name Texture Coordinates. */
	/*@{*/
	/** \brief Number of texture coordinates per set. */
	inline int GetTextureCoordinatesCount() const{ return pTextureCoordinatesCount; }
	
	/** \brief Set number of texture coordinates per set. */
	void SetTextureCoordinatesCount( int count );
	
	/** \brief Number of texture coordinates sets. */
	inline int GetTextureCoordinatesSetCount() const{ return pTextureCoordinatesSetsCount; }
	
	/** \brief Set number of texture coordinates sets which has to be at least 1. */
	void SetTextureCoordinatesSetCount( int count );
	
	/** \brief Texture coordinates set at the given index. */
	deModelTextureCoordinatesSet &GetTextureCoordinatesSetAt( int index ) const;
	
	/** \brief Pointer to the texture coordinates sets. */
	inline deModelTextureCoordinatesSet *GetTextureCoordinatesSets() const{ return pTextureCoordinatesSets; }
	/*@}*/
};

#endif
