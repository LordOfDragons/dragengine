/* 
 * Drag[en]gine Game Engine
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
class deAnimatorRuleVisitorIdentify : public deAnimatorRuleVisitor{
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
		ertRetarget,
		ertTrackTo,
		ertLimit
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
	
	/** \brief Rule is a retarget rule. */
	inline bool IsRetarget() const{ return pType == ertRetarget; }
	
	/** \brief Rule is a track to rule. */
	inline bool IsTrackTo() const{ return pType == ertTrackTo; }
	
	/** \brief Rule is a limit rule. */
	inline bool IsLimit() const{ return pType == ertLimit; }
	
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
	 * \brief Cast to retarget rule.
	 * \throws deeInvalidParam Type of rule is not deAnimatorRuleVisitorIdentify::ertRetarget.
	 */
	deAnimatorRuleRetarget &CastToRetarget() const;
	
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
	
	/** \brief Visit retarget rule. */
	virtual void VisitRetarget( deAnimatorRuleRetarget &rule );
	
	/** \brief Visit track to rule. */
	virtual void VisitTrackTo( deAnimatorRuleTrackTo &rule );
	
	/** \brief Visit limit rule. */
	virtual void VisitLimit( deAnimatorRuleLimit &rule );
	/*@}*/
};

#endif
