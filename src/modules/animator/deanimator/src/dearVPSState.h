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

#ifndef _DEARVPSSTATE_H_
#define _DEARVPSSTATE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearComponentVPSState;
class dearAnimationVPSState;
class deComponent;


/** Vertex position set state. */
class dearVPSState{
private:
	const char *pName;
	int pIndex;
	int pModelIndex;
	float pWeight;
	bool pProtect;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create state. */
	dearVPSState();
	
	/** Clean up state. */
	~dearVPSState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const char *GetName() const{return pName;}
	
	/** Set name. */
	void SetName(const char *name);
	
	/** Index of state. */
	inline int GetIndex() const{return pIndex;}
	
	/** Set index of state. */
	void SetIndex(int index);
	
	/** Model index. */
	inline int GetModelIndex() const{return pModelIndex;}
	
	/** Set model index. */
	void SetModelIndex(int index);
	
	/** Weight. */
	inline float GetWeight() const{return pWeight;}
	
	/** Set weight. */
	void SetWeight(float weight);
	
	/** State is protected. */
	inline bool GetProtected() const{return pProtect;}
	
	/** Set if state is protected. */
	void SetProtected(bool prot);
	
	/** State is dirty. */
	inline bool GetDirty() const{return pDirty;}
	
	/** Set if state is dirty. */
	void SetDirty(bool dirty);
	
	/** Copy another state to this state. */
	void SetFrom(const dearVPSState &state);
	
	/** Copy component state to this state. */
	void SetFrom(const deComponent &component);
	
	/** Copy component state to this state. */
	void SetFrom(const dearComponentVPSState &state);
	
	/** Update state by blending it with an empty state. */
	void BlendWithDefault(deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight);
	
	/** Update state by blending it with an incoming state. */
	void BlendWith(const dearVPSState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight);
	
	/** Update state by blending it with an incoming state. */
	void BlendWith(const dearAnimationVPSState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight);
	
	/** Update state by blending it with an incoming state. */
	void BlendWith(const deComponent &component, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight);
	
	/** Update state by blending it with an incoming state. */
	void BlendWith(const dearComponentVPSState &state, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight);
	
	/** Update state by blending it with an incoming state. */
	void BlendWith(float weight, deAnimatorRule::eBlendModes blendMode,
		float blendFactor, bool enableWeight);
	
	/** Full copy. */
	dearVPSState &operator=(const dearVPSState &state);
	/*@}*/
};

#endif
