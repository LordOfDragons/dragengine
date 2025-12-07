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

#ifndef _FBXRIGBONE_H_
#define _FBXRIGBONE_H_

#include "../fbxScene.h"

#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxRig;
class fbxNode;
class fbxScene;

class deBaseModule;


/**
 * \brief FBX managed rig bone.
 */
class fbxRigBone : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxRigBone> Ref;
	
	
private:
	fbxRig &pRig;
	fbxNode &pNodePoseBone;
	fbxNode &pNodeModel;
	fbxNode *pNodeArmature;
	int64_t pNodeModelID;
	
	int pIndex;
	decString pName;
	fbxRigBone *pParent;
	decMatrix pFbxMatrix;
	decMatrix pBoneMatrix;
	decMatrix pMatrixInverse;
	decMatrix pMatrix;
	decMatrix pAnimMatrix;
	decVector pPosition;
	decQuaternion pOrientation;
	bool pDirty;
	fbxScene::eRotationOrder pRotationOrder;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	fbxRigBone(fbxRig &rig, fbxNode &nodePoseBone, fbxNode &nodeModel);
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxRigBone();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Rig. */
	inline fbxRig &GetRig() const{ return pRig; }
	
	/** \brief Pose bone node. */
	inline fbxNode &GetNodePoseBone() const{ return pNodePoseBone; }
	
	/** \brief Model node. */
	inline fbxNode &GetNodeModel() const{ return pNodeModel; }
	
	/** \brief Bone name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Set index. */
	void SetIndex(int index);
	
	/** \brief Set bone name. */
	void SetName(const char *name);
	
	/** \brief Node model ID. */
	inline int64_t GetNodeModelID() const{ return pNodeModelID; }
	
	/** \brief Parent or NULL. */
	inline fbxRigBone *GetParent() const{ return pParent; }
	
	/** \brief Fbx matrix. */
	inline const decMatrix &GetFbxMatrix() const{ return pFbxMatrix; }
	
	/** \brief World matrix. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decMatrix &GetMatrixInverse() const{ return pMatrixInverse; }
	
	/** \brief Bone matrix relative to parent. */
	inline const decMatrix &GetBoneMatrix() const{ return pBoneMatrix; }
	
	/** \brief Animation calculation matrix. */
	inline const decMatrix &GetAnimMatrix() const{ return pAnimMatrix; }
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Rotation order. */
	inline fbxScene::eRotationOrder GetRotationOrder() const{ return pRotationOrder; }
	
	
	
	/** \brief Prepare. */
	void Prepare();
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose = false) const;
	/*@}*/
};

#endif
