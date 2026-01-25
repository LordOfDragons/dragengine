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

#ifndef _DEARBONESTATELIST_H_
#define _DEARBONESTATELIST_H_

#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearBoneState;
class dearComponent;
class deComponent;
class deAnimation;
class deRig;
class deAnimator;


/**
 * List of bone states.
 */
class dearBoneStateList{
private:
	decTList<dearBoneState> pStates;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new list. */
	dearBoneStateList();

	dearBoneStateList(const dearBoneStateList& other);
	dearBoneStateList& operator=(const dearBoneStateList& other);

	/** Cleans up the list. */
	~dearBoneStateList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** States. */
	inline decTList<dearBoneState> &GetStates(){ return pStates; }
	inline const decTList<dearBoneState> &GetStates() const{ return pStates; }
	
	/** Retrieves the number of states. */
	inline int GetStateCount() const{ return pStates.GetCount(); }
	/** Sets the number of states. */
	void SetStateCount(int count);
	
	/** Retrieves the given state. */
	dearBoneState &GetStateAt(int index);
	const dearBoneState &GetStateAt(int index) const;
	
	/** Retrieves the index of the state with the given rig bone name. */
	int IndexOfStateNamed(const char *name) const;
	
	/** Creates a copy of this list. */
	dearBoneStateList *CreateCopy() const;
	
	/** Copy another state list to this list. */
	void SetFrom(const dearBoneStateList &stateList);
	
	/** Updates the states. */
	void UpdateStates();
	/** Mark dirty. */
	void MarkDirty();
	
	/** Updates the mappings. */
	void UpdateMappings(const deAnimator &animator);
	
	/** Apply states to an engine component. */
	void ApplyToComponent(deComponent *component) const;
	
	/** Apply states to an engine component. */
	void ApplyToComponent(deComponent *component, deAnimatorRule::eBlendModes blendMode, float blendFactor) const;
	
	/** Apply states to an animator module component. */
	void ApplyToComponent(dearComponent &component) const;
	
	/** Apply states to an animator module component. */
	void ApplyToComponent(dearComponent &component, deAnimatorRule::eBlendModes blendMode, float blendFactor) const;
	/*@}*/
};

// end of include only once
#endif
