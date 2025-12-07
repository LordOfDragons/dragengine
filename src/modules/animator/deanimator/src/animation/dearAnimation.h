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

#ifndef _DEARANIMATION_H_
#define _DEARANIMATION_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorAnimation.h>
#include <dragengine/common/collection/decObjectList.h>

class dearAnimationMove;
class deDEAnimator;
class deAnimation;



/**
 * DEAnimator animation peer.
 */
class dearAnimation : public deBaseAnimatorAnimation{
private:
	deDEAnimator *pModule;
	
	deAnimation *pAnimation;
	
	decObjectList pMoves;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	dearAnimation(deDEAnimator *module, deAnimation *animation);
	/** Cleans up the peer. */
	virtual ~dearAnimation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the module. */
	inline deDEAnimator *GetModule() const{ return pModule; }
	/** Retrieve the animation. */
	inline deAnimation *GetAnimation() const{ return pAnimation; }
	
	/** Retrieve the number of moves. */
	int GetMoveCount() const;
	/** Retrieve move by index. */
	dearAnimationMove *GetMoveAt(int index) const;
	/** Retrieve move by name or NULL if not found. */
	dearAnimationMove *GetMoveNamed(const char *name) const;
	/*@}*/
	
private:
	void pCreateMoves();
};

#endif
