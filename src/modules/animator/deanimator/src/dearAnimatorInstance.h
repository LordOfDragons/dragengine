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

#ifndef _DEARANIMATORINSTANCE_H_
#define _DEARANIMATORINSTANCE_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorAnimatorInstance.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decThreadSafeObjectOrderedSet.h>

#include "dearBoneStateList.h"
#include "dearVPSStateList.h"
#include "dearControllerStates.h"

class dearComponent;
class dearTaskApplyRules;
class deRig;
class deAnimatorControllerTarget;
class deAnimatorInstance;
class deAnimatorLink;
class dearAnimation;
class dearAnimator;
class dearRule;
class deDEAnimator;
class dearLink;



/**
 * Animator instance peer.
 */
class dearAnimatorInstance : public deBaseAnimatorAnimatorInstance{
private:
	deDEAnimator &pModule;
	
	deAnimatorInstance &pAnimatorInstance;
	
	dearAnimator *pAnimator;
	
	bool pDirtyMappings;
	bool pDirtyAnimator;
	bool pDirtyRuleParams;
	
	bool pUseBlending;
	bool pSkipApply;
	bool pUseAllBones;
	bool pUseAllVPS;
	
	unsigned int pAnimatorUpdateTracker;
	
	dearAnimation *pAnimation;
	dearComponent *pComponent;
	
	dearBoneStateList pBoneStateList;
	decIntList pMappingRigToState;
	
	dearVPSStateList pVPSStateList;
	decIntList pMappingModelToVPSState;
	
	dearControllerStates pControllerStates;
	
	decPointerList pLinks;
	
	dearRule **pRules;
	int pRuleCount;
	bool pDirtyRules;
	
	bool pCaptureComponentState;
	
	bool pUseParallelTask;
	dearTaskApplyRules *pActiveTaskApplyRule;
	decThreadSafeObjectOrderedSet pTaskApplyRules;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	dearAnimatorInstance(deDEAnimator &module, deAnimatorInstance &instance);
	
	/** Clean up peer. */
	virtual ~dearAnimatorInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Animator module. */
	inline deDEAnimator &GetModule(){return pModule;}
	
	
	
	/** Animation or nullptr if not set. */
	inline dearAnimation *GetAnimation() const{return pAnimation;}
	
	/** Component or nullptr if not set. */
	inline dearComponent *GetComponent() const{return pComponent;}
	
	/** Bone state list. */
	inline dearBoneStateList &GetBoneStateList(){return pBoneStateList;}
	inline const dearBoneStateList &GetBoneStateList() const{return pBoneStateList;}
	
	/** VPS state list. */
	inline dearVPSStateList &GetVPSStateList(){return pVPSStateList;}
	inline const dearVPSStateList &GetVPSStateList() const{return pVPSStateList;}
	
	/** Controller states. */
	inline const dearControllerStates &GetControllerStates() const{return pControllerStates;}
	
	/** Set capture current component state to true. */
	void SetCaptureComponentState();
	
	
	
	/** Number of links. */
	int GetLinkCount() const;
	
	/** Link at index. */
	dearLink *GetLinkAt(int index) const;
	
	/** Add link. */
	void AddLink(dearLink *link);
	
	
	
	/**
	 * Apply rules to animator instance state. This call is thread-safe. It can be used parallel
	 * or synchronous. The resulting bone state is not applied to anything. Call the appropriate
	 * ApplyStateTo* calls for this.
	 */
	void ApplyRules();
	
	/**
	 * Apply bone states to the bound animator module component if existing. This call is
	 * asynchronous. It is used by task Run() call.
	 */
	void ApplyStateToArComponent() const;
	
	/**
	 * Stop task running apply rules in parallel. Called by dearTaskApplyRules.Finished() only.
	 */
	void StopTaskApplyRules();
	
	
	
	
	/**
	 * Apply state of animator to the component if existing.
	 * \details Animator modules can decide to calculate this in parallel. If
	 *          \em direct is \em true the application is always done
	 *          synchronously and is done after the call returns. If \em direct
	 *          is \em false the calculation can be parallel. The affected
	 *          resources take care of waiting for the result to become ready
	 *          if required.
	 */
	virtual void Apply(bool direct);
	
	/**
	 * Capture current state of component into rules matching identifier.
	 */
	virtual void CaptureStateInto(int identifier);
	
	/**
	 * Store animation frame from animation into rules matching identifier.
	 * \details If \em moveName does not exist in the animation a default state is captured.
	 * \throws deeInvalidParam \em moveName is nullptr.
	 */
	virtual void StoreFrameInto(int identifier, const char *moveName, float moveTime);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Animator changed. */
	virtual void AnimatorChanged();
	
	/** Component changed. */
	virtual void ComponentChanged();
	
	/** Animation changed. */
	virtual void AnimationChanged();
	
	/** Blend factor changed. */
	virtual void BlendFactorChanged();
	
	/** Enable retargeting changed. */
	virtual void EnableRetargetingChanged();
	
	/** Protect dynamic bones changed. */
	virtual void ProtectDynamicBonesChanged();
	
	/** Controller changed. */
	virtual void ControllerChanged(int index);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCheckRequireRebuild();
	void pUpdateMappings();
	void pCheckAnimatorChanged();
	void pUpdateAnimator();
	void pUpdateFakeRootBones();
	
	void pRemoveAllLinks();
	void pAddAnimatorLinks();
	
	void pDropRules();
	void pUpdateRules();
	void pUpdateRuleParams();
	
	dearTaskApplyRules *pNewTaskApplyRules();
	
	
	
	/** Update controller states. */
	void pUpdateControllerStates();
	
	/**
	 * Apply bone states to the bound engine component if existing. This call is synchronous.
	 * It is used by synchronous calls or by the task Finished() call.
	 */
	void pApplyStateToComponent() const;
	
	/**
	 * Run task to apply rules.
	 * 
	 * If running in parallel is disabled this calls ApplyRules() instead.
	 */
	void pStartTaskApplyRules();
	
	/**
	 * Cancel task to apply rules if existing.
	 * \details Forces parallel tasks to finish to ensure the tasks is finished.
	 */
	void pCancelTaskApplyRules();
	
	void pWaitTaskApplyRules();
	
	void pWaitAnimTaskFinished();
};

#endif
