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

#ifndef _DEHEIGHTTERRAINNAVSPACE_H_
#define _DEHEIGHTTERRAINNAVSPACE_H_

#include "../../navigation/space/deNavigationSpace.h"

class deHeightTerrainNavSpaceEdge;


/**
 * \brief Height terrain navigation space.
 *
 * Navigation space using height terrain points with cost defined per face.
 * Vertices are stored as navigation points derived from the terrain geometry:
 * <code>navpoint = image-dimension * coord.y + coord.x</code> .
 */
class DE_DLL_EXPORT deHeightTerrainNavSpace{
private:
	deNavigationSpace::eSpaceTypes pType;
	int pLayer;
	
	float pSnapDistance;
	float pSnapAngle;
	
	unsigned int *pCorners;
	int pCornerCount;
	
	deHeightTerrainNavSpaceEdge *pEdges;
	int pEdgeCount;
	
	deNavigationSpaceFace *pFaces;
	int pFaceCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create navigation space. */
	deHeightTerrainNavSpace();
	
	/** \brief Clean up navigation space. */
	~deHeightTerrainNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Navigation space type. */
	inline deNavigationSpace::eSpaceTypes GetType() const{return pType;}
	
	/** \brief Set navigation space type. */
	void SetType(deNavigationSpace::eSpaceTypes type);
	
	/** \brief Navigation layer number. */
	inline int GetLayer() const{return pLayer;}
	
	/** \brief Set navigation layer number. */
	void SetLayer(int layer);
	
	
	
	/**
	 * \brief Snap distance in meters up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap distance is 0.001 (1mm).
	 */
	inline float GetSnapDistance() const{return pSnapDistance;}
	
	/**
	 * \brief Set snap distance in meters up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap distance is 0.001 (1mm).
	 */
	void SetSnapDistance(float distance);
	
	/**
	 * \brief Snap angle in radians up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap angle is 180° in radians.
	 */
	inline float GetSnapAngle() const{return pSnapAngle;}
	
	/**
	 * \brief Set snap angle in radians up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap angle is 180° in radians.
	 */
	void SetSnapAngle(float angle);
	/*@}*/
	
	
	
	/** \name Corners */
	/*@{*/
	/** \brief Number of corners. */
	inline int GetCornerCount() const{return pCornerCount;}
	
	/** \brief Set number of corners. */
	void SetCornerCount(int count);
	
	/** \brief Corner at index . */
	unsigned int GetCornerAt(int index) const;
	
	/** \brief Set corner at index . */
	void SetCornerAt(int index, unsigned int corner);
	
	/** \brief Pointer to corners. */
	inline unsigned int *GetCorners() const{return pCorners;}
	/*@}*/
	
	
	
	/** \name Edges */
	/*@{*/
	/** \brief Number of edges. */
	inline int GetEdgeCount() const{return pEdgeCount;}
	
	/** \brief Set number edges. */
	void SetEdgeCount(int count);
	
	/** \brief Edge at the given position. */
	deHeightTerrainNavSpaceEdge &GetEdgeAt(int index) const;
	
	/** \brief Pointer to the edges. */
	inline deHeightTerrainNavSpaceEdge *GetEdges() const{return pEdges;}
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Number of faces. */
	inline int GetFaceCount() const{return pFaceCount;}
	
	/** \brief Set number faces. */
	void SetFaceCount(int count);
	
	/** \brief Face at the given position. */
	deNavigationSpaceFace &GetFaceAt(int index) const;
	
	/** \brief Pointer to the faces. */
	inline deNavigationSpaceFace *GetFaces() const{return pFaces;}
	/*@}*/
};

#endif
