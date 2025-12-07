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
		decVector edgeNormal[3];
		float edgeDistance[3];
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
	void Build(const deoalModelFace *faces, int faceCount);
	
	/** \brief Drop temporary build data. */
	void DropBuildData();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pAddBuildNode();
	void pUpdateNodeExtends(sBuildNode &node) const;
	void pSplitNode(int nodeIndex);
	void pNodeAddFace(sBuildNode &node, int faceIndex);
	void pBuildVisitNode(const sBuildNode &buildNode);
};

#endif
