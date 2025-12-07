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

#ifndef _DEOALMODELFACE_H_
#define _DEOALMODELFACE_H_

#include <dragengine/common/math/decMath.h>

class deModelLOD;
class deModelFace;


/**
 * \brief Model face.
 */
class deoalModelFace{
private:
	int pIndex;
	decVector pVertices[3];
	int pWeightSets[3];
	int pTexture;
	decVector pMinExtend;
	decVector pMaxExtend;
	decVector pNormal;
	decVector pEdgeNormal[3];
	float pEdgeDistance[3];
	float pEdgeDistanceSafe[3];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model face. */
	deoalModelFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index. */
	inline int GetIndex() const{return pIndex;}
	
	/** \brief Texture. */
	inline int GetTexture() const{return pTexture;}
	
	
	
	/** \brief First vertex. */
	inline const decVector &GetVertex1() const{return pVertices[0];}
	
	/** \brief Set first vertex. */
	void SetVertex1(const decVector &vector);
	
	/** \brief Scond vertex. */
	inline const decVector &GetVertex2() const{return pVertices[1];}
	
	/** \brief Set second vertex. */
	void SetVertex2(const decVector &vector);
	
	/** \brief Third vertex. */
	inline const decVector &GetVertex3() const{return pVertices[2];}
	
	/** \brief Set third vertex. */
	void SetVertex3(const decVector &vector);
	
	/** \brief Vertex at index. */
	const decVector &GetVertexAt(int index) const;
	
	
	
	/** \brief First weight set. */
	inline int GetWeightSet1() const{return pWeightSets[0];}
	
	/** \brief Scond weight set. */
	inline int GetWeightSet2() const{return pWeightSets[1];}
	
	/** \brief Third weight set. */
	inline int GetWeightSet3() const{return pWeightSets[2];}
	
	/** \brief WeightSet at index. */
	int GetWeightSetAt(int index) const;
	
	
	
	/** \brief Min extend. */
	inline const decVector &GetMinExtend() const{return pMinExtend;}
	
	/** \brief Max extend. */
	inline const decVector &GetMaxExtend() const{return pMaxExtend;}
	
	
	
	/**
	 * \brief Normal.
	 * 
	 * Vertices are oriented clock-wise.
	 */
	inline const decVector &GetNormal() const{return pNormal;}
	
	
	
	/**
	 * \brief Edge 1 normal.
	 * 
	 * Edge normal points inside.
	 */
	inline const decVector &GetEdge1Normal() const{return pEdgeNormal[0];}
	
	/**
	 * \brief Edge 2 normal.
	 * 
	 * Edge normal points inside.
	 */
	inline const decVector &GetEdge2Normal() const{return pEdgeNormal[1];}
	
	/**
	 * \brief Edge 3 normal.
	 * 
	 * Edge normal points inside.
	 */
	inline const decVector &GetEdge3Normal() const{return pEdgeNormal[2];}
	
	/**
	 * \brief Edge normal at index matching vertex index.
	 * 
	 * Edge normal points inside.
	 */
	const decVector &GetEdgeNormalAt(int index) const;
	
	
	
	/**
	 * \brief Edge 1 distance.
	 * 
	 * Equals GetEdge1Normal() * GetVertex1().
	 */
	inline float GetEdge1Distance() const{return pEdgeDistance[0];}
	
	/**
	 * \brief Edge 2 distance.
	 * 
	 * Equals GetEdge2Normal() * GetVertex2().
	 */
	inline float GetEdge2Distance() const{return pEdgeDistance[1];}
	
	/**
	 * \brief Edge 3 distance.
	 * 
	 * Equals GetEdge3Normal() * GetVertex3().
	 */
	inline float GetEdge3Distance() const{return pEdgeDistance[2];}
	
	/**
	 * \brief Edge distance at index matching vertex index.
	 * 
	 * Equals GetEdgeNormalAt(index) * GetVertexAt(index).
	 */
	float GetEdgeDistanceAt(int index) const;
	
	/**
	 * \brief Edge 1 safe distance.
	 * 
	 * Equals GetEdge1Normal() * GetVertex1().
	 */
	inline float GetEdge1DistanceSafe() const{return pEdgeDistanceSafe[0];}
	
	/**
	 * \brief Edge 2 safe distance.
	 * 
	 * Equals GetEdge2Normal() * GetVertex2().
	 */
	inline float GetEdge2DistanceSafe() const{return pEdgeDistanceSafe[1];}
	
	/**
	 * \brief Edge 3 safe distance.
	 * 
	 * Equals GetEdge3Normal() * GetVertex3().
	 */
	inline float GetEdge3DistanceSafe() const{return pEdgeDistanceSafe[2];}
	
	/**
	 * \brief Edge safe distance at index matching vertex index.
	 * 
	 * Equals GetEdgeNormalAt(index) * GetVertexAt(index).
	 */
	float GetEdgeDistanceSafeAt(int index) const;
	
	
	
	/** \brief Init. */
	void Init(const deModelLOD &lod, int index, const deModelFace &face);
	
	/** \brief Init. */
	void InitFromStatic(const deoalModelFace &face);
	
	/** \brief Update normal and edge information from vertices. */
	void UpdateNormalAndEdges();
	/*@}*/
};

#endif
