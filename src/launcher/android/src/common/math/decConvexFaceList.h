/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECCONVEXFACELIST_H_
#define _DECCONVEXFACELIST_H_

#include "decMath.h"
#include "../collection/decPointerList.h"

class decConvexFace;
class decConvexVolume;



/**
 * \brief List of convex faces that can be processed by splitters.
 * 
 * Sometimes it is useful to have a list of convex faces that
 * describe some object or multiple objects. The Convex face
 * list stores a list of convex faces and their vertices.
 * Split operations can be done on the stored convex faces.
 * The newly generated convex faces are appended to the list.
 * This way a single convex face list can be used to cut an
 * original convex face into smaller pieces.
 * 
 * The convex face list does only provide storing convex faces
 * and does not use any kind of spatial optimiziation like
 * octrees. If you need spatial optimization or if you are
 * using a large number of convex faces writing your own class
 * is a better solution.
 * 
 * You can use this class as a temporary storage to split your
 * convex faces and moving the result afterwards into a better
 * place. The extraction functions allow to remove convex faces
 * without deleting them.
 */
class decConvexFaceList{
private:
	decVector *pVertices;
	int pVertexCount;
	int pVertexSize;
	
	decPointerList pFaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty list. */
	decConvexFaceList();
	
	/** \brief Create copy of list. */
	decConvexFaceList( const decConvexFaceList &list );
	
	/** \brief Clean up list. */
	virtual ~decConvexFaceList();
	/*@}*/
	
	
	
	/** \name Vertices */
	/*@{*/
	/** \brief Number of vertices. */
	int GetVertexCount() const;
	
	/** \brief Vertex at position. */
	const decVector &GetVertexAt( int index ) const;
	
	/** \brief Vertex is present. */
	bool HasVertex( const decVector &vertex ) const;
	
	/** \brief Index of vertex or -1 if absent. */
	int IndexOfVertex( const decVector &vertex ) const;
	
	/** \brief Add vertex. */
	void AddVertex( const decVector &vertex );
	
	/** \brief Remove vertex. */
	void RemoveVertex( int index );
	
	/** \brief Remove all vertices. */
	void RemoveAllVertices();
	/*@}*/
	
	
	
	/** \name Faces */
	/*@{*/
	/** \brief Number of faces. */
	int GetFaceCount() const;
	
	/** \brief Face at index. */
	decConvexFace *GetFaceAt( int index ) const;
	
	/** \brief Face is present. */
	bool HasFace( decConvexFace *face ) const;
	
	/** \brief Index of face or -1 if absent. */
	int IndexOfFace( decConvexFace *face ) const;
	
	/** \brief Add face. */
	void AddFace( decConvexFace *face );
	
	/** \brief Remove face. */
	void RemoveFace( decConvexFace *face );
	
	/** \brief Remove face at index. */
	void RemoveFaceAt( int index );
	
	/** \brief Remvoe face without deleting it. */
	void ExtractFace( decConvexFace *face );
	
	/** \brief Remove face at index without deleting it. */
	void ExtractFaceAt( int index );
	
	/** \brief Remove all faces. */
	void RemoveAllFaces();
	/*@}*/
	
	
	
	/** \name Operations */
	/*@{*/
	/** \brief Move all vertices. */
	void Move( const decVector &direction );
	
	/**
	 * \brief Split using face.
	 * 
	 * All newly created faces are added to the list. The face has
	 * not to be part of the convex face list.
	 */
	void SplitByFace( const decConvexFaceList &splitterFaceList, int splitterFaceIndex );
	
	/** \brief Split using list. */
	void SplitByFaceList( const decConvexFaceList &splitterFaceList );
	
	/** \brief Split using convex volume removing split faces inside. */
	void SplitByVolume( const decConvexVolume &volume );
	/*@}*/
	
	
	
	/** \name Subclassing */
	/*@{*/
	/**
	 * \brief Create convex face.
	 * 
	 * Overwrite to create convex faces of specialized subclasses.
	 * If face is not NULL it indicates the face this new face is based
	 * upon. The default implementation creates a convex face of type
	 *         decConvexFace using normal and marker from face if not NULL.
	 */
	virtual decConvexFace *CreateFace( decConvexFace *face );
	/*@}*/
	
	
	
private:
	void pSplitFaceByEdge( int faceIndex, const decVector &cutEdgeFrom, const decVector &cutEdgeTo );
};

#endif
