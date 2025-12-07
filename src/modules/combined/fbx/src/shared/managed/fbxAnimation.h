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

#ifndef _FBXANIMATION_H_
#define _FBXANIMATION_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxNode;
class fbxScene;
class fbxRig;
class fbxAnimationMove;

class deBaseModule;


/**
 * \brief FBX managed animation.
 */
class fbxAnimation : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxAnimation> Ref;
	
	
private:
	fbxScene &pScene;
	decObjectOrderedSet pMoves;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animation. */
	fbxAnimation(fbxScene &scene);
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxAnimation();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Scene. */
	inline fbxScene &GetScene() const{return pScene;}
	
	
	
	/** \brief Count of moves. */
	int GetMoveCount() const;
	
	/** \brief Move at index. */
	fbxAnimationMove *GetMoveAt(int index) const;
	
	/** \brief Named move or NULL if absent. */
	fbxAnimationMove *GetMoveNamed(const char *name) const;
	
	/** \brief Match moves against rig. */
	void MatchRig(const fbxRig &rig);
	
	/** \brief Convert FBX time to seconds. */
	static float ConvTime(int64_t time);
	
	
	
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure(deBaseModule &module, const decString &prefix, bool verbose = false) const;
	/*@}*/
};

#endif
