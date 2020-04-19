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

#ifndef _DEOALRTWORLDBVH_H_
#define _DEOALRTWORLDBVH_H_

#include <dragengine/common/math/decMath.h>

class deoalAComponent;



/**
 * \brief OpenAL ray tracing world bounding volume hierarchie.
 */
class deoalRTWorldBVH{
public:
	/** \brief BVH node used for building tree. */
	struct sBuildNode{
		decVector center;
		decVector halfSize;
		int child1;
		int child2;
		int firstComponent;
		int lastComponent;
		int componentCount;
	};
	
	/** \brief Component used for building tree. */
	struct sBuildComponent{
		decVector center;
		decVector halfSize;
		decVector minExtend;
		decVector maxExtend;
		deoalAComponent *component;
		int next;
	};
	
	/** \brief BVH node used for visiting tree. */
	struct sVisitNode{
		decVector center;
		decVector halfSize;
		int node1;
		int node2;
		int firstComponent;
		int componentCount;
	};
	
	/** \brief Component used for visiting tree. */
	struct sVisitComponent{
		decVector center;
		decVector halfSize;
		deoalAComponent *component;
		decMatrix inverseMatrix;
	};
	
	
	
private:
	decDVector pPosition;
	
	sBuildNode *pBuildNodes;
	int pBuildNodeCount;
	int pBuildNodeSize;
	
	sBuildComponent *pBuildComponents;
	int pBuildComponentCount;
	int pBuildComponentSize;
	
	sVisitNode *pVisitNodes;
	int pVisitNodeCount;
	int pVisitNodeSize;
	
	sVisitComponent *pVisitComponents;
	int pVisitComponentCount;
	int pVisitComponentSize;
	
	int pIndexNode;
	int pIndexComponent;
	decDMatrix pWorldMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create BVH tree. */
	deoalRTWorldBVH();
	
	/** \brief Clean up BVH tree. */
	~deoalRTWorldBVH();
	/*@}*/
	
	
	
	/** \name Building */
	/*@{*/
	/** \brief BVH position in world coordinates. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/**
	 * \brief Begin building tree.
	 * 
	 * Clears visit and build nodes. Sets position.
	 */
	void Build( const decDVector &position );
	
	/** \brief Add component during build phase. */
	void AddComponent( deoalAComponent *component );
	
	/**
	 * \brief Finish building octree.
	 * 
	 * Creates visit nodes from build nodes. Clears build nodes.
	 */
	void Finish();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit nodes array. */
	inline const sVisitNode *GetVisitNodes() const{ return pVisitNodes; }
	
	/** \brief Visit node count. */
	inline int GetVisitNodeCount() const{ return pVisitNodeCount; }
	
	/** \brief Visit components array. */
	inline const sVisitComponent *GetVisitComponents() const{ return pVisitComponents; }
	
	/** \brief Visit component count. */
	inline int GetVisitComponentCount() const{ return pVisitComponentCount; }
	/*@}*/
	
	
	
private:
	int pAddBuildNode();
	int pAddBuildComponent();
	void pUpdateNodeExtends( sBuildNode &node ) const;
	void pSplitNode( int nodeIndex );
	void pNodeAddComponent( sBuildNode &node, int componentIndex );
	void pBuildVisitNode( const sBuildNode &buildNode );
};

#endif
