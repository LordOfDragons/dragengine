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
	~deAnimatorRuleAnimationSelect() override;
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
	void SetEnablePosition(bool enabled);
	
	/** \brief Orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	
	/** \brief Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
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
	void Visit(deAnimatorRuleVisitor &visitor) override;
	/*@}*/
};

#endif
