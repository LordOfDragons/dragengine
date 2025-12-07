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

#ifndef _DEARANIMATIONMOVE_H_
#define _DEARANIMATIONMOVE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deAnimationMove;
class dearAnimationKeyframeList;
class dearAnimationKeyframeVPSList;



/** Animation peer move. */
class dearAnimationMove : public deObject{
private:
	decString pName;
	float pPlaytime;
	
	dearAnimationKeyframeList **pKeyframeLists;
	int pKeyframeListCount;
	
	dearAnimationKeyframeVPSList **pKeyframeVPSLists;
	int pKeyframeVPSListCount;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dearAnimationMove> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new animation move. */
	dearAnimationMove(const deAnimationMove &move);
	
protected:
	/** Clean up the animation move. */
	virtual ~dearAnimationMove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{return pName;}
	
	/** Play time in seconds. */
	inline float GetPlaytime() const{return pPlaytime;}
	
	/** Count of keyframe lists. */
	inline int GetKeyframeListCount() const{return pKeyframeListCount;}
	
	/** Keyframe list at index. */
	dearAnimationKeyframeList *GetKeyframeListAt(int index) const;
	
	/** Count of keyframe lists. */
	inline int GetKeyframeVPSListCount() const{return pKeyframeVPSListCount;}
	
	/** Keyframe list at index. */
	dearAnimationKeyframeVPSList *GetKeyframeVPSListAt(int index) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateKeyframeLists(const deAnimationMove &move);
	void pCreateKeyframeVPSLists(const deAnimationMove &move);
};

#endif
