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

#ifndef _DEOALMODELRTOCTREE_H_
#define _DEOALMODELRTOCTREE_H_

#include <dragengine/common/math/decMath.h>


class deoalModelFace;
class deoalModelOctree;


/**
 * \brief Ray-tracing optimized model octree.
 */
class deoalModelRTOctree{
public:
	struct sFace{
		decVector normal;
		decVector baseVertex;
		decVector edgeNormal[ 3 ];
		float edgeDistance[ 3 ];
		int indexFace;
		int indexTexture;
	};
	
	struct sNode{
		decVector center;
		decVector halfSize;
		int firstNode;
		int nodeCount;
		int firstFace;
		int faceCount;
	};
	
	
	
private:
	sFace *pFaces;
	int pFaceCount;
	sNode *pNodes;
	int pNodeCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create octree. */
	deoalModelRTOctree( deoalModelOctree &octree );
	
	/** \brief Clean up octree. */
	~deoalModelRTOctree();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Faces. */
	inline const sFace *GetFaces() const{ return pFaces; }
	
	/** \brief Face count. */
	inline int GetFaceCount() const{ return pFaceCount; }
	
	/** \brief Nodes. */
	inline const sNode *GetNodes() const{ return pNodes; }
	
	/** \brief Node count. */
	inline int GetNodeCount() const{ return pNodeCount; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
