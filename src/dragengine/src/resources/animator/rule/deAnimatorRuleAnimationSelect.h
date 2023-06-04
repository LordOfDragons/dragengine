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

#ifndef _DEANIMATORRULEANIMATIONSELECT_H_
#define _DEANIMATORRULEANIMATIONSELECT_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decStringList.h"


/**
 * \brief Animator animation select rule.
 * 
 * Apply animation state from an animation move selected from a list of animation moves.
 * 
 * The animation move to use is selected using a link. The animation moves are distributed
 * evenly across the 0 to 1 link value range in the order they are defined. The closest
 * animation move is chosen. To blend between multiple animation states use multiple
 * deAnimatorRuleAnimationSelect.
 * 
 * This rule is best used for automated animation system requiring picking animations from
 * a list of animations. This can be for example a motion matching system using a database
 * of animations.
 */
class DE_DLL_EXPORT deAnimatorRuleAnimationSelect : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleAnimationSelect> Ref;
	
	
	
private:
	decStringList pMoves;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	deAnimatorControllerTarget pTargetMoveTime;
	deAnimatorControllerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator rule. */
	deAnimatorRuleAnimationSelect();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleAnimationSelect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief List of move. */
	inline decStringList &GetMoves(){ return pMoves; }
	inline const decStringList &GetMoves() const{ return pMoves; }
	
	/** \brief Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** \brief Move time target. */
	inline deAnimatorControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	inline const deAnimatorControllerTarget &GetTargetMoveTime() const{ return pTargetMoveTime; }
	
	/** \brief Select target. */
	inline deAnimatorControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const deAnimatorControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
