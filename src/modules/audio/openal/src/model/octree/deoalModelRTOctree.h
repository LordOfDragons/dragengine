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
