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

#ifndef _FBXANIMATIONMOVECURVES_H_
#define _FBXANIMATIONMOVECURVES_H_


#include <stdint.h>
#include "fbxAnimationCurve.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxAnimationMove;
class fbxNode;
class fbxScene;
class fbxRigBone;

class deBaseModule;


/**
 * \brief FBX managed model cluster.
 */
class fbxAnimationMoveCurves : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxAnimationMoveCurves> Ref;
	
	
public:
	/** \brief Target property. */
	enum eTargetProperty{
		etpPosition,
		etpRotation,
		etpScale,
		etpUnsupported
	};
	
	
	
private:
	fbxAnimationMove &pMove;
	fbxNode &pNodeCurves;
	int64_t pNodeCurvesID;
	fbxNode *pNodeModel;
	int64_t pNodeModelID;
	fbxAnimationCurve::Ref pCurveX;
	fbxAnimationCurve::Ref pCurveY;
	fbxAnimationCurve::Ref pCurveZ;
	decVector pDefaultValue;
	
	decString pBoneName;
	eTargetProperty pTargetProperty;
	fbxRigBone *pRigBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create move. */
	fbxAnimationMoveCurves(fbxAnimationMove &move, fbxNode &nodeCurves);
	
protected:
	/** \brief Clean up move. */
	virtual ~fbxAnimationMoveCurves();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Model. */
	inline fbxAnimationMove &GetMove() const{ return pMove; }
	
	/** \brief Curves node. */
	inline fbxNode &GetNodeCurves() const{ return pNodeCurves; }
	
	/** \brief Node curves ID. */
	inline int64_t GetNodeCurvesID() const{ return pNodeCurvesID; }
	
	/** \brief Model node. */
	inline fbxNode *GetNodeModel() const{ return pNodeModel; }
	
	/** \brief Model node ID. */
	inline int64_t GetNodeModelID() const{ return pNodeModelID; }
	
	/** \brief Target property. */
	inline eTargetProperty GetTargetProperty() const{ return pTargetProperty; }
	
	/** \brief Set target property. */
	void SetTargetProperty(eTargetProperty targetProperty);
	
	/** \brief Bone name. */
	inline const decString &GetBoneName() const{ return pBoneName; }
	
	/** \brief Set bone name. */
	void SetBoneName(const char *name);
	
	/** \brief Rig bone or NULL. */
	inline fbxRigBone *GetRigBone() const{ return pRigBone; }
	
	/** \brief Set rig bone or NULL. */
	void SetRigBone(fbxRigBone *bone);
	
	/** \brief Curves or NULL. */
	inline const fbxAnimationCurve::Ref &GetCurveX() const{ return pCurveX; }
	inline const fbxAnimationCurve::Ref &GetCurveY() const{ return pCurveY; }
	inline const fbxAnimationCurve::Ref &GetCurveZ() const{ return pCurveZ; }
	
	/** \brief Default value. */
	inline const decVector &GetDefaultValue() const{ return pDefaultValue; }
	
	/** \brief Set default value. */
	void SetDefaultValue(const decVector &defaultValue);
	
	
	
	/** \brief Prepare. */
	void Prepare();
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose = false) const;
	/*@}*/
};

#endif
