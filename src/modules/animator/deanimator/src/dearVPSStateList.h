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

#ifndef _DEARVPSSTATELIST_H_
#define _DEARVPSSTATELIST_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearComponent;
class dearVPSState;
class deComponent;
class deAnimator;


/** VPS State List. */
class dearVPSStateList{
private:
	dearVPSState *pStates;
	int pStateCount;
	int pStateSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create state list. */
	dearVPSStateList();
	
	/** Clean up state list. */
	~dearVPSStateList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Count of states. */
	inline int GetStateCount() const{return pStateCount;}
	
	/** Set count of states. */
	void SetStateCount(int count);
	
	/** State at index. */
	dearVPSState &GetStateAt(int index) const;
	
	/** Index of named state or -1. */
	int IndexOfStateNamed(const char *name) const;
	
	/** Create copy of state list. */
	dearVPSStateList *CreateCopy() const;
	
	/** Copy another state list to this list. */
	void SetFrom(const dearVPSStateList &stateList);
	
	/** Mark dirty. */
	void MarkDirty();
	
	/** Update mappings. */
	void UpdateMappings(const deAnimator &animator, const deComponent *component);
	
	/** Apply states to engine component. */
	void ApplyToComponent(deComponent &component) const;
	
	/** Apply states to engine component. */
	void ApplyToComponent(deComponent &component,
		deAnimatorRule::eBlendModes blendMode, float blendFactor) const;
	
	/** Apply states to animator module component. */
	void ApplyToComponent(dearComponent &component) const;
	
	/** Apply states to animator module component. */
	void ApplyToComponent(dearComponent &component,
		deAnimatorRule::eBlendModes blendMode, float blendFactor) const;
	/*@}*/
};

#endif
