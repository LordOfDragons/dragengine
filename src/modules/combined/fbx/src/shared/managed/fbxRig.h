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

#ifndef _FBXRIG_H_
#define _FBXRIG_H_


#include "../fbxNode.h"

#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxScene;
class fbxRigBone;

class deBaseModule;


/**
 * \brief FBX managed rig.
 */
class fbxRig : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxRig> Ref;
	
	
private:
	fbxScene &pScene;
	const fbxNode::Ref pNodePose;
	decObjectOrderedSet pBones;
	decMatrix pMatrix;
	decMatrix pMatrixInverse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	fbxRig( fbxScene &scene, fbxNode *nodePose );
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxRig();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Scene. */
	inline fbxScene &GetScene() const{ return pScene; }
	
	/** \brief Pose node. */
	inline fbxNode &GetNodePose() const{ return pNodePose; }
	
	
	
	/** \brief Count of bones. */
	int GetBoneCount() const;
	
	/** \brief Bone at index. */
	fbxRigBone *GetBoneAt( int index ) const;
	
	/** \brief Named bone or NULL if absent. */
	fbxRigBone *GetBoneNamed( const char *name ) const;
	
	/** \brief Bone with model ID or NULL if absent. */
	fbxRigBone *GetBoneWithModelID( int64_t id ) const;
	
	
	
	/** \brief Matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decMatrix &GetMatrixInverse() const{ return pMatrix; }
	
	
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	void pAddRootBone( fbxScene &scene, fbxNode &nodeRoot );
};

#endif
