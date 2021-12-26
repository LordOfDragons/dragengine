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

#ifndef _DEANIMATORRULEVISITOR_H_
#define _DEANIMATORRULEVISITOR_H_

class deAnimatorRule;
class deAnimatorRuleAnimation;
class deAnimatorRuleAnimationDifference;
class deAnimatorRuleAnimationSelect;
class deAnimatorRuleBoneTransformator;
class deAnimatorRuleInverseKinematic;
class deAnimatorRuleStateManipulator;
class deAnimatorRuleStateSnapshot;
class deAnimatorRuleForeignState;
class deAnimatorRuleGroup;
class deAnimatorRuleSubAnimator;
class deAnimatorRuleRetarget;
class deAnimatorRuleTrackTo;
class deAnimatorRuleLimit;


/**
 * \brief Animator rule visitor.
 */
class DE_DLL_EXPORT deAnimatorRuleVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator rule visitor . */
	deAnimatorRuleVisitor();
	
	/** \brief Clean up animator rule visitor. */
	virtual ~deAnimatorRuleVisitor();
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
