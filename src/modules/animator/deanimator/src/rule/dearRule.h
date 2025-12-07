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

#ifndef _DEARRULE_H_
#define _DEARRULE_H_

#include "../dearControllerTarget.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>

class dearAnimator;
class dearAnimatorInstance;
class dearAnimation;
class dearBoneStateList;
class dearVPSStateList;
class deDEAnimator;



/**
 * Base animator rule class.
 */
class dearRule{
private:
	dearAnimatorInstance &pInstance;
	const dearAnimator &pAnimator;
	const deAnimatorRule &pRule;
	
	int *pBoneMappings;
	int pBoneMappingCount;
	bool pUseAllBones;
	
	int *pVPSMappings;
	int pVPSMappingCount;
	bool pUseAllVPS;
	
	dearControllerTarget pTargetBlendFactor;
	
	const deAnimatorRule::eBlendModes pBlendMode;
	const float pBlendFactor;
	const bool pInvertBlendFactor;
	const bool pEnabled;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create rule. */
	dearRule(dearAnimatorInstance &instance, const dearAnimator &animator,
		int firstLink, const deAnimatorRule &rule);
	
	/** Clean up animator. */
	virtual ~dearRule();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Animator module. */
	deDEAnimator &GetModule() const;
	
	/** Animator. */
	inline const dearAnimator &GetAnimator() const{ return pAnimator; }
	
	/** Animator instance. */
	inline dearAnimatorInstance &GetInstance(){ return pInstance; } 
	inline const dearAnimatorInstance &GetInstance() const{ return pInstance; } 
	
	/** Animator rule. */
	inline const deAnimatorRule &GetRule() const{ return pRule; }
	
	
	
	/** All bones are used or a list of bones. */
	inline bool GetUseAllBones() const{ return pUseAllBones; }
	
	/** Bone mapping count. */
	int GetBoneMappingCount() const;
	
	/** Bone mapping for bone index. */
	int GetBoneMappingFor(int boneIndex) const;
	
	
	
	/** All vertex position sets are used or a list of vertex position sets. */
	inline bool GetUseAllVPS() const{ return pUseAllVPS; }
	
	/** Vertex position set mapping count. */
	int GetVPSMappingCount() const;
	
	/** Vertex position set mapping for vertex position set index. */
	int GetVPSMappingFor(int vpsIndex) const;
	
	
	
	/** Blend factor after applying a controller to it if one is set. */
	float GetBlendFactor() const;
	
	/** Animation to use. */
	dearAnimation *GetUseAnimation() const;
	
	
	
	/** Thread-safe rule blend mode. */
	inline deAnimatorRule::eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/** Thread-safe rule enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	
	
	/**
	 * Capture animator state.
	 * \details The default implementation throws an exception.
	 */
	virtual void CaptureStateInto(int identifier);
	
	/**
	 * Store animation frame.
	 * \details The default implementation throws an exception.
	 */
	virtual void StoreFrameInto(int identifier, const char *moveName, float moveTime);
	
	/**
	 * Check if a full rebuild of the animator instance is required.
	 */
	virtual bool RebuildInstance() const;
	
	/** Apply to animator. */
	virtual void Apply(dearBoneStateList &stalist, dearVPSStateList &vpsstalist) = 0;
	
	/** Controller changed. */
	virtual void ControllerChanged(int controller);
	
	/** Rule changed. */
	virtual void RuleChanged();
	/*@}*/
	
private:
	void pUpdateBoneMappings();
	void pUpdateVPSMappings();
};

#endif
