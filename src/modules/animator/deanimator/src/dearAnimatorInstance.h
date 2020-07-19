/* 
 * Drag[en]gine Animator Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEARANIMATORINSTANCE_H_
#define _DEARANIMATORINSTANCE_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorAnimatorInstance.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/collection/decThreadSafeObjectOrderedSet.h>

#include "dearBoneStateList.h"
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
 * \brief Animator instance peer.
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
	
	unsigned int pAnimatorUpdateTracker;
	
	dearAnimation *pAnimation;
	dearComponent *pComponent;
	
	dearBoneStateList pBoneStateList;
	decIntList pMappingRigToState;
	
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
	/** \brief Create peer. */
	dearAnimatorInstance( deDEAnimator &module, deAnimatorInstance &instance );
	
	/** \brief Clean up peer. */
	virtual ~dearAnimatorInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Animator module. */
	inline deDEAnimator &GetModule(){ return pModule; }
	
	
	
	/** \brief Animation or \em NULL if not set. */
	inline dearAnimation *GetAnimation() const{ return pAnimation; }
	
	/** \brief Component or \em NULL if not set. */
	inline dearComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Bone state list. */
	inline dearBoneStateList &GetBoneStateList(){ return pBoneStateList; }
	inline const dearBoneStateList &GetBoneStateList() const{ return pBoneStateList; }
	
	/** \brief Controller states. */
	inline const dearControllerStates &GetControllerStates() const{ return pControllerStates; }
	
	/** \brief Set capture current component state to true. */
	void SetCaptureComponentState();
	
	
	
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/** \brief Link at index. */
	dearLink *GetLinkAt( int index ) const;
	
	/** \brief Add link. */
	void AddLink( dearLink *link );
	
	
	
	/**
	 * \brief Apply rules to animator instance state.
	 * \details This call is thread-safe. It can be used parallel or synchronous.
	 *          The resulting bone state is not applied to anything. Call the
	 *          appropriate ApplyStateTo* calls for this.
	 */
	void ApplyRules();
	
	/**
	 * \brief Apply bone states to the bound animator module component if existing.
	 * \details This call is asynchronous. It is used by task Run() call.
	 */
	void ApplyStateToArComponent() const;
	
	/**
	 * \brief Stop task running apply rules in parallel.
	 * \details Called by dearTaskApplyRules.Finished() only.
	 */
	void StopTaskApplyRules();
	
	
	
	
	/**
	 * \brief Apply state of animator to the component if existing.
	 * \details Animator modules can decide to calculate this in parallel. If
	 *          \em direct is \em true the application is always done
	 *          synchronously and is done after the call returns. If \em direct
	 *          is \em false the calculation can be parallel. The affected
	 *          resources take care of waiting for the result to become ready
	 *          if required.
	 */
	virtual void Apply( bool direct );
	
	/**
	 * \brief Capture current state of component into rules matching identifier.
	 */
	virtual void CaptureStateInto( int identifier );
	
	/**
	 * \brief Store animation frame from animation into rules matching identifier.
	 * \details If \em moveName does not exist in the animation a default state is captured.
	 * \throws deeInvalidParam \em moveName is \em NULL.
	 */
	virtual void StoreFrameInto( int identifier, const char *moveName, float moveTime );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Animator changed. */
	virtual void AnimatorChanged();
	
	/** \brief Component changed. */
	virtual void ComponentChanged();
	
	/** \brief Animation changed. */
	virtual void AnimationChanged();
	
	/** \brief Blend factor changed. */
	virtual void BlendFactorChanged();
	
	/** \brief Enable retargeting changed. */
	virtual void EnableRetargetingChanged();
	
	/** \brief Protect dynamic bones changed. */
	virtual void ProtectDynamicBonesChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int index );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCheckRequireRebuild();
	void pUpdateBoneMappings();
	void pCheckAnimatorChanged();
	void pUpdateAnimator();
	void pUpdateFakeRootBones();
	
	void pRemoveAllLinks();
	void pAddAnimatorLinks();
	
	void pDropRules();
	void pUpdateRules();
	void pUpdateRuleParams();
	
	dearTaskApplyRules *pNewTaskApplyRules();
	
	
	
	/** \brief Update controller states. */
	void pUpdateControllerStates();
	
	/**
	 * \brief Apply bone states to the bound engine component if existing.
	 * \details This call is synchronous. It is used by synchronous calls or
	 *          by the task Finished() call.
	 */
	void pApplyStateToComponent() const;
	
	/**
	 * \brief Run task to apply rules.
	 * 
	 * If running in parallel is disabled this calls ApplyRules() instead.
	 */
	void pStartTaskApplyRules();
	
	/**
	 * \brief Cancel task to apply rules if existing.
	 * \details Forces parallel tasks to finish to ensure the tasks is finished.
	 */
	void pCancelTaskApplyRules();
	
	void pWaitTaskApplyRules();
	
	void pWaitAnimTaskFinished();
};

#endif
