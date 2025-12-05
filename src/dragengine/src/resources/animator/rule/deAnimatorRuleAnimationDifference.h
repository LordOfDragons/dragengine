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

#ifndef _DEANIMATORRULEANIMATIONDIFFERENCE_H_
#define _DEANIMATORRULEANIMATIONDIFFERENCE_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Animation Difference Rule Class.
 *
 * Retrieves two animation states from two different animation
 * frames and calculates the difference between the two states.
 * This difference is then blended over the existing state.
 * State 1 is considered the leading state whereas state 2 is
 * considered the reference state. The difference is taken from
 * the leading state minus the reference state. If both states
 * use the same move an additional flag can be set so you only
 * have to set the move of the leading state to set both moves.
 */
class DE_DLL_EXPORT deAnimatorRuleAnimationDifference : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleAnimationDifference> Ref;
	
	
private:
	decString pMove1Name;
	float pMove1Time;
	decString pMove2Name;
	float pMove2Time;
	bool pUseSameMove;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	deAnimatorControllerTarget pTargetLeadMoveTime;
	deAnimatorControllerTarget pTargetRefMoveTime;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleAnimationDifference();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleAnimationDifference();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the leading animation move. */
	inline const decString &GetLeadingMoveName() const{ return pMove1Name; }
	
	/** \brief Set leading animation move name. */
	void SetLeadingMoveName( const char *moveName );
	
	/** \brief Leading animation move time. */
	inline float GetLeadingMoveTime() const{ return pMove1Time; }
	
	/** \brief Set leading animation move time. */
	void SetLeadingMoveTime( float moveTime );
	
	/** \brief Name of the reference animation move. */
	inline const decString &GetReferenceMoveName() const{ return pMove2Name; }
	
	/** \brief Set reference animation move name. */
	void SetReferenceMoveName( const char *moveName );
	
	/** \brief Reference animation move time. */
	inline float GetReferenceMoveTime() const{ return pMove2Time; }
	
	/** \brief Set reference animation move time. */
	void SetReferenceMoveTime( float moveTime );
	
	/** \brief Reference move is the same as the leading move. */
	inline bool GetUseSameMove() const{ return pUseSameMove; }
	
	/** \brief Set if reference move is the same as the leading move. */
	void SetUseSameMove( bool useSameMove );
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** \brief Leading move time target. */
	inline deAnimatorControllerTarget &GetTargetLeadingMoveTime(){ return pTargetLeadMoveTime; }
	inline const deAnimatorControllerTarget &GetTargetLeadingMoveTime() const{ return pTargetLeadMoveTime; }
	
	/** \brief Reference move time target. */
	inline deAnimatorControllerTarget &GetTargetReferenceMoveTime(){ return pTargetRefMoveTime; }
	inline const deAnimatorControllerTarget &GetTargetReferenceMoveTime() const{ return pTargetRefMoveTime; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
