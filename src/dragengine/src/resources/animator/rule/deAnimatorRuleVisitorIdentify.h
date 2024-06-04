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

#ifndef _DEANIMATORRULEVISITORIDENTIFY_H_
#define _DEANIMATORRULEVISITORIDENTIFY_H_

#include "deAnimatorRuleVisitor.h"


/**
 * \brief Animator Rule Visitor Identify.
 * 
 * Animation rule visitor providing rule identification. After visiting a
 * rule the visitor can be asked what kind of rule has been visited. Useful
 * to identify rules and to carry out actions only for certain rule types.
 * Furthermore save casting is provided. If the cast is not valid an
 * exception is raised.
 */
class DE_DLL_EXPORT deAnimatorRuleVisitorIdentify : public deAnimatorRuleVisitor{
public:
	enum eRuleTypes{
		ertUnknown,
		ertAnimation,
		ertAnimationDifference,
		ertAnimationSelect,
		ertBoneTransformator,
		ertInverseKinematic,
		ertStateManipulator,
		ertStateSnapshot,
		ertForeignState,
		ertGroup,
		ertSubAnimator,
		ertTrackTo,
		ertLimit,
		
		/** \version 1.9 */
		ertMirror
	};
	
	
	
private:
	deAnimatorRule *pRule;
	eRuleTypes pType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identify animator rule visitor. */
	deAnimatorRuleVisitorIdentify();
	
	/** \brief Clean up identify animator rule visitor. */
	virtual ~deAnimatorRuleVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Rule type. */
	inline eRuleTypes GetType() const{ return pType; }
	
	/** \brief Rule is unknown. */
	inline bool IsUnknown() const{ return pType == ertUnknown; }
	
	/** \brief Rule is an animation rule. */
	inline bool IsAnimation() const{ return pType == ertAnimation; }
	
	/** \brief Rule is an animation Difference rule. */
	inline bool IsAnimationDifference() const{ return pType == ertAnimationDifference; }
	
	/** \brief Rule is an animation select rule. */
	inline bool IsAnimationSelect() const{ return pType == ertAnimationSelect; }
	
	/** \brief Rule is a bone transformator rule. */
	inline bool IsBoneTransformator() const{ return pType == ertBoneTransformator; }
	
	/** \brief Rule is an inverse Kinematic rule. */
	inline bool IsInverseKinematic() const{ return pType == ertInverseKinematic; }
	
	/** \brief Rule is a state manipulator rule. */
	inline bool IsStateManipulator() const{ return pType == ertStateManipulator; }
	
	/** \brief Rule is a state snapshot rule. */
	inline bool IsStateSnapshot() const{ return pType == ertStateSnapshot; }
	
	/** \brief Rule is a foreign state rule. */
	inline bool IsForeignState() const{ return pType == ertForeignState; }
	
	/** \brief Rule is a group rule. */
	inline bool IsGroup() const{ return pType == ertGroup; }
	
	/** \brief Rule is a sub animator rule. */
	inline bool IsSubAnimator() const{ return pType == ertSubAnimator; }
	
	/** \brief Rule is a track to rule. */
	inline bool IsTrackTo() const{ return pType == ertTrackTo; }
	
	/** \brief Rule is a limit rule. */
	inline bool IsLimit() const{ return pType == ertLimit; }
	
	/**
	 * \brief Rule is a mirror rule.
	 * \version 1.9
	 */
	inline bool IsMirror() const{ return pType == ertMirror; }
	
	/**
	 * \brief Cast to animation rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertAnimation.
	 */
	deAnimatorRuleAnimation &CastToAnimation() const;
	
	/**
	 * \brief Cast to animation difference rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertAnimationDifference.
	 */
	deAnimatorRuleAnimationDifference &CastToAnimationDifference() const;
	
	/**
	 * \brief Cast to animation select rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertAnimationSelect.
	 */
	deAnimatorRuleAnimationSelect &CastToAnimationSelect() const;
	
	/**
	 * \brief Cast to bone transformator rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertBoneTransformator.
	 */
	deAnimatorRuleBoneTransformator &CastToBoneTransformator() const;
	
	/**
	 * \brief Cast to inverse kinematic rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertInverseKinematic.
	 */
	deAnimatorRuleInverseKinematic &CastToInverseKinematic() const;
	
	/**
	 * \brief Cast to state manipulator rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertStateManipulator.
	 */
	deAnimatorRuleStateManipulator &CastToStateManipulator() const;
	
	/**
	 * \brief Cast to state snapshot rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertStateSnapshot.
	 */
	deAnimatorRuleStateSnapshot &CastToStateSnapshot() const;
	
	/**
	 * \brief Cast to foreign state rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertForeignState.
	 */
	deAnimatorRuleForeignState &CastToForeignState() const;
	
	/**
	 * \brief Cast to group rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertGroup.
	 */
	deAnimatorRuleGroup &CastToGroup() const;
	
	/**
	 * \brief Cast to sub animator rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertSubAnimator.
	 */
	deAnimatorRuleSubAnimator &CastToSubAnimator() const;
	
	/**
	 * \brief Cast to track to rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertTrackTo.
	 */
	deAnimatorRuleTrackTo &CastToTrackTo() const;
	
	/**
	 * \brief Cast to limit rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertLimit.
	 */
	deAnimatorRuleLimit &CastToLimit() const;
	
	/**
	 * \brief Cast to mirror rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertMirror.
	 */
	deAnimatorRuleMirror &CastToMirror() const;
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit animator rule. */
	virtual void VisitRule( deAnimatorRule &rule );
	
	/** \brief Visit animation rule. */
	virtual void VisitAnimation( deAnimatorRuleAnimation &rule );
	
	/** \brief Visit animation difference rule. */
	virtual void VisitAnimationDifference( deAnimatorRuleAnimationDifference &rule );
	
	/** \brief Visit animation select rule. */
	virtual void VisitAnimationSelect( deAnimatorRuleAnimationSelect &rule );
	
	/** \brief Visit bone rotator animator rule. */
	virtual void VisitBoneTransformator( deAnimatorRuleBoneTransformator &rule );
	
	/** \brief Visit inverse kinematic animator rule. */
	virtual void VisitInverseKinematic( deAnimatorRuleInverseKinematic &rule );
	
	/** \brief Visit state manipulator rule. */
	virtual void VisitStateManipulator( deAnimatorRuleStateManipulator &rule );
	
	/** \brief Visit state snapshot rule. */
	virtual void VisitStateSnapshot( deAnimatorRuleStateSnapshot &rule );
	
	/** \brief Visit foreign state rule. */
	virtual void VisitForeignState( deAnimatorRuleForeignState &rule );
	
	/** \brief Visit group rule. */
	virtual void VisitGroup( deAnimatorRuleGroup &rule );
	
	/** \brief Visit sub animator rule. */
	virtual void VisitSubAnimator( deAnimatorRuleSubAnimator &rule );
	
	/** \brief Visit track to rule. */
	virtual void VisitTrackTo( deAnimatorRuleTrackTo &rule );
	
	/** \brief Visit limit rule. */
	virtual void VisitLimit( deAnimatorRuleLimit &rule );
	
	/**
	 * \brief Visit mirror rule.
	 * \version 1.9
	 */
	virtual void VisitMirror( deAnimatorRuleMirror &rule );
	/*@}*/
};

#endif
