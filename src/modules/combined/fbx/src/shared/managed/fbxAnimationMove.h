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

#ifndef _FBXANIMATIONMOVE_H_
#define _FBXANIMATIONMOVE_H_

#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class fbxRig;
class fbxAnimation;
class fbxAnimationMoveCurves;
class fbxNode;
class fbxScene;

class deBaseModule;


/**
 * \brief FBX managed model cluster.
 */
class fbxAnimationMove : public deObject{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<fbxAnimationMove>;
	
	/** \brief List of animation move nodes. */
	using List = decTCollectionQueryByName<decTObjectOrderedSet<fbxAnimationMove>,fbxAnimationMove>;
	
	
private:
	fbxAnimation &pAnimation;
	fbxNode &pNodeStack;
	int64_t pNodeStackID;
	fbxNode *pNodeLayer;
	int64_t pNodeLayerID;
	decTObjectOrderedSet<fbxAnimationMoveCurves> pCurveNodes;
	
	decString pName;
	int pFrameRate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create move. */
	fbxAnimationMove(fbxAnimation &animation, fbxNode &nodeStack);
	
protected:
	/** \brief Clean up move. */
	virtual ~fbxAnimationMove();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Animation. */
	inline fbxAnimation &GetAnimation() const{ return pAnimation; }
	
	/** \brief Stack node. */
	inline fbxNode &GetNodeStack() const{ return pNodeStack; }
	
	/** \brief Node stack ID. */
	inline int64_t GetNodeStackID() const{ return pNodeStackID; }
	
	/** \brief Node layer. */
	inline fbxNode &GetNodeLayer() const{ return *pNodeLayer; }
	
	/** \brief Node layer ID. */
	inline int64_t GetNodeLayerID() const{ return pNodeLayerID; }
	
	/** \brief Bone name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set bone name. */
	void SetName(const char *name);
	
	
	
	/** \brief Curves. */
	inline const decTObjectOrderedSet<fbxAnimationMoveCurves> &GetCurveNodes() const{ return pCurveNodes; }
	
	/** \brief Match curves against rig. */
	void MatchRig(const fbxRig &rig);
	
	/** \brief Frame rate. */
	inline int GetFrameRate() const{ return pFrameRate; }
	
	/** \brief Set frame rate. */
	void SetFrameRate(int frameRate);
	
	/** \brief Convert time to frame number. */
	int TimeToFrame(float time) const;
	
	/** \brief Convert frame number to time. */
	float FrameToTime(int frame) const;
	
	/** \brief Quantize time. */
	float QuantizeTime(float time) const;
	
	
	
	/** \brief Prepare. */
	void Prepare();
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose = false) const;
	/*@}*/
};

#endif
