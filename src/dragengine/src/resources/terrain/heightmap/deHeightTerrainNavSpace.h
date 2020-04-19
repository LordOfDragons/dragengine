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
class deHeightTerrainNavSpace{
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
	inline deNavigationSpace::eSpaceTypes GetType() const{ return pType; }
	
	/** \brief Set navigation space type. */
	void SetType( deNavigationSpace::eSpaceTypes type );
	
	/** \brief Navigation layer number. */
	inline int GetLayer() const{ return pLayer; }
	
	/** \brief Set navigation layer number. */
	void SetLayer( int layer );
	
	
	
	/**
	 * \brief Snap distance in meters up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap distance is 0.001 (1mm).
	 */
	inline float GetSnapDistance() const{ return pSnapDistance; }
	
	/**
	 * \brief Set snap distance in meters up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap distance is 0.001 (1mm).
	 */
	void SetSnapDistance( float distance );
	
	/**
	 * \brief Snap angle in radians up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap angle is 180° in radians.
	 */
	inline float GetSnapAngle() const{ return pSnapAngle; }
	
	/**
	 * \brief Set snap angle in radians up to which edges are linked to neighbor spaces.
	 * 
	 * The default snap angle is 180° in radians.
	 */
	void SetSnapAngle( float angle );
	/*@}*/
	
	
	
	/** \name Corners */
	/*@{*/
	/** \brief Number of corners. */
	inline int GetCornerCount() const{ return pCornerCount; }
	
	/** \brief Set number of corners. */
	void SetCornerCount( int count );
	
	/** \brief Corner at index . */
	unsigned int GetCornerAt( int index ) const;
	
	/** \brief Set corner at index . */
	void SetCornerAt( int index, unsigned int corner );
	
	/** \brief Pointer to corners. */
	inline unsigned int *GetCorners() const{ return pCorners; }
	/*@}*/
	
	
	
	/** \name Edges */
	/*@{*/
	/** \brief Number of edges. */
	inline int GetEdgeCount() const{ return pEdgeCount; }
	
	/** \brief Set number edges. */
	void SetEdgeCount( int count );
	
	/** \brief Edge at the given position. */
	deHeightTerrainNavSpaceEdge &GetEdgeAt( int index ) const;
	
	/** \brief Pointer to the edges. */
	inline deHeightTerrainNavSpaceEdge *GetEdges() const{ return pEdges; }
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Number of faces. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Set number faces. */
	void SetFaceCount( int count );
	
	/** \brief Face at the given position. */
	deNavigationSpaceFace &GetFaceAt( int index ) const;
	
	/** \brief Pointer to the faces. */
	inline deNavigationSpaceFace *GetFaces() const{ return pFaces; }
	/*@}*/
};

#endif
