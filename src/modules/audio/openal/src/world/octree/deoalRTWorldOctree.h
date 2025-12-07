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

#ifndef _DEOALRTWORLDOCTREE_H_
#define _DEOALRTWORLDOCTREE_H_

#include <dragengine/common/math/decMath.h>

class deoalAComponent;



/**
 * \brief OpenAL ray tracing world octree.
 * 
 * Optimized version of deoalWorldOctree for ray tracing. Octree is relative to a world
 * position which is typically the microphone or spaker. Components affected by ray tracing
 * are inserted into the octree and stored in a faster to visit way than deoalWorldOctree
 * can achieve. This octree also builds faster than deoalWorldOctree. For this reason flat
 * arrays are used with child nodes indexing into the array.
 * 
 * Building the octree is done in two phases. First an octree is build from scratch which
 * uses 8 optional child nodes as deoalOctree does. In the second phase a second octree is
 * build with only the existing child nodes packed as tightly as possible. This second
 * octree is then used for visiting.
 * 
 * Components in this octree are only visited by checking the bounding box. During building
 * the visitor has to prepare the component and check if the component has faces and is
 * matching layer mask. Once added no checks will be done anymore to get the highest
 * performance possible.
 */
class deoalRTWorldOctree{
public:
	/** \brief Octree node used for building octree. */
	struct sBuildNode{
		decVector center;
		decVector halfSize;
		int child[8];
		int childCount;
		int firstComponent;
		int lastComponent;
		int componentCount;
	};
	
	/** \brief Component used for building octree. */
	struct sBuildComponent{
		decVector center;
		decVector halfSize;
// 		decVector sphereCenter;
// 		float sphereRadiusSquared;
		deoalAComponent *component;
		int next;
	};
	
	/** \brief Octree node used for visiting octree. */
	struct sVisitNode{
		decVector center;
		decVector halfSize;
		int firstNode;
		int nodeCount;
		int firstComponent;
		int componentCount;
	};
	
	/** \brief Component used for visiting octree. */
	struct sVisitComponent{
		decVector center;
		decVector halfSize;
// 		decVector sphereCenter;
// 		float sphereRadiusSquared;
		deoalAComponent *component;
		decMatrix inverseMatrix;
	};
	
	
	
private:
	decDVector pPosition;
	decVector pHalfExtends;
	int pMaxDepth;
	
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
	
	int pGatherNodeCount;
	int pGatherComponentCount;
	int pIndexChild;
	int pIndexComponent;
	decDMatrix pWorldMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create octree. */
	deoalRTWorldOctree();
	
	/** \brief Clean up octree. */
	~deoalRTWorldOctree();
	/*@}*/
	
	
	
	/** \name Building */
	/*@{*/
	/** \brief Octree position in world coordinates. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Octree half extends in world coordinates. */
	inline const decVector &GetHalfExtends() const{ return pHalfExtends; }
	
	/**
	 * \brief Begin building octree.
	 * 
	 * Clears visit and build nodes. Sets position and half extends.
	 */
	void Build(const decDVector &position, const decVector &halfExtends);
	
	/**
	 * \brief Begin building octree.
	 * 
	 * Clears visit and build nodes. Sets position and half extends.
	 */
	void Build(const decDVector &position, double radius);
	
	/** \brief Add component during build phase. */
	void AddComponent(deoalAComponent *component);
	
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
	int pGetBuildNodeFor(const decVector &center, const decVector &halfSize);
	int pGetBuildNodeFor(int nodeIndex, const decVector &center, const decVector &halfSize);
	int pGetBuildOctantFor(sBuildNode &node, const decVector &center, const decVector &halfSize) const;
	int pAddBuildNode();
	int pAddBuildComponent();
	void pGatherCounts(const sBuildNode &node);
	void pBuildVisitNode(const sBuildNode &buildNode, sVisitNode &node);
};

#endif
