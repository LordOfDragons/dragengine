/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALICOSPHERE_H_
#define _DEOALICOSPHERE_H_

#include <dragengine/common/math/decMath.h>

class deoalAudioThread;


/**
 * \brief Ico sphere builder.
 * 
 * Calculates different levels of ico sphere using an subdivision approach. The calculated
 * levels are guaranteed to compose of the same directions of previous levels with new data
 * applied to it. The vertices can be used directly as directions for ray casting.
 */
class deoalIcoSphere{
public:
	struct sFace{
		unsigned short vertices[ 3 ];
		unsigned short edges[ 3 ];
		void Set( int vertex1, int vertex2, int vertex3, int edge1, int edge2, int edge3 );
	};
	
	struct sEdge{
		unsigned short vertices[ 2 ];
		void Set( int vertex1, int vertex2 );
	};
	
	
	
private:
	decVector *pVertices;
	int pVertexCount;
	
	sEdge *pEdges;
	int pEdgeCount;
	
	sFace *pFaces;
	int pFaceCount;
	
	float pOpeningAngle;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty ico sphere. */
	deoalIcoSphere();
	
	/** \brief Create copy of ico sphere. */
	deoalIcoSphere( const deoalIcoSphere &icosphere );
	
	/** \brief Clean up ico sphere. */
	~deoalIcoSphere();
	
	/** \brief Base level ico sphere. */
	static deoalIcoSphere BaseLevel();
	/*@}*/
	
	
	
public:
	/** \name Manegement */
	/*@{*/
	/** \brief Vertices. */
	inline const decVector *GetVertices() const{ return pVertices; }
	
	/** \brief Number of vertices. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Edges. */
	inline const sEdge *GetEdges() const{ return pEdges; }
	
	/** \brief Edge count. */
	inline int GetEdgeCount() const{ return pEdgeCount; }
	
	/** \brief Opening angle. */
	inline float GetOpeningAngle() const{ return pOpeningAngle; }
	
	/** \brief Subdivide ico sphere. */
	deoalIcoSphere Subdivide() const;
	
	/** \brief Debug print (blender output). */
	void DebugPrintBlender( deoalAudioThread &audioThread ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy ico sphere. */
	deoalIcoSphere &operator=( const deoalIcoSphere &ico );
	/*@}*/
	
	
	
protected:
	void pCalculateOpeningAngle();
};

#endif
