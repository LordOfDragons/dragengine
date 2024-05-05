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
