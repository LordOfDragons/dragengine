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

#ifndef _DEOALMODELRTBVH_H_
#define _DEOALMODELRTBVH_H_

#include <dragengine/common/math/decMath.h>


class deoalAModel;
class deoalModelFace;
class deoalModelBVH;



/**
 * \brief Ray-tracing optimized model BVH.
 */
class deoalModelRTBVH{
public:
	struct sNode{
		decVector center;
		decVector halfSize;
		int node1;
		int node2;
		int firstFace;
		int faceCount;
	};
	
	struct sFace{
		decVector normal;
		decVector baseVertex;
		decVector edgeNormal[ 3 ];
		float edgeDistance[ 3 ];
		int indexFace;
		int indexTexture;
	};
	
	
	
private:
	struct sBuildNode{
		decVector center;
		decVector halfSize;
		int child1;
		int child2;
		int firstFace;
		int lastFace;
		int faceCount;
	};
	
	struct sBuildFace{
		decVector center;
		decVector minExtend;
		decVector maxExtend;
		const deoalModelFace *modelFace;
		int next;
	};
	
	sFace *pFaces;
	int pFaceCount;
	int pFaceSize;
	
	sNode *pNodes;
	int pNodeCount;
	int pNodeSize;
	
	sBuildNode *pBuildNodes;
	int pBuildNodeCount;
	int pBuildNodeSize;
	
	sBuildFace *pBuildFaces;
	int pBuildFaceCount;
	int pBuildFaceSize;
	
	int pIndexNode;
	int pIndexFace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create BVH. */
	deoalModelRTBVH();
	
	/** \brief Clean up BVH. */
	~deoalModelRTBVH();
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
	
	
	
	/** \brief Build from faces. */
	void Build( const deoalModelFace *faces, int faceCount );
	
	/** \brief Drop temporary build data. */
	void DropBuildData();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pAddBuildNode();
	void pUpdateNodeExtends( sBuildNode &node ) const;
	void pSplitNode( int nodeIndex );
	void pNodeAddFace( sBuildNode &node, int faceIndex );
	void pBuildVisitNode( const sBuildNode &buildNode );
};

#endif
