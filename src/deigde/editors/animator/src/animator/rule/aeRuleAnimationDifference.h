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

#ifndef _AERULEANIMATIONDIFFERENCE_H_
#define _AERULEANIMATIONDIFFERENCE_H_

#include "aeRule.h"



/**
 * Animator rule animation difference.
 */
class aeRuleAnimationDifference : public aeRule{
public:
	typedef deTObjectReference<aeRuleAnimationDifference> Ref;
	
	
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
	
	aeControllerTarget pTargetLeadMoveTime;
	aeControllerTarget pTargetRefMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new animator difference rule. */
	aeRuleAnimationDifference();
	/** Create a copy of an animator difference rule. */
	aeRuleAnimationDifference( const aeRuleAnimationDifference &copy );
	/** Cleans up the animator difference rule. */
	virtual ~aeRuleAnimationDifference();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the leading animation move. */
	inline const decString &GetLeadingMoveName() const{ return pMove1Name; }
	/** Set the leading animation move name. */
	void SetLeadingMoveName( const char *moveName );
	/** Retrieve the leading animation move time. */
	inline float GetLeadingMoveTime() const{ return pMove1Time; }
	/** Set the leading animation move time. */
	void SetLeadingMoveTime( float moveTime );
	/** Retrieve the name of the reference animation move. */
	inline const decString &GetReferenceMoveName() const{ return pMove2Name; }
	/** Set the reference animation move name. */
	void SetReferenceMoveName( const char *moveName );
	/** Retrieve the reference animation move time. */
	inline float GetReferenceMoveTime() const{ return pMove2Time; }
	/** Set the reference animation move time. */
	void SetReferenceMoveTime( float moveTime );
	/** Determine if the reference move is the same as the leading move. */
	inline bool GetUseSameMove() const{ return pUseSameMove; }
	/** Set if the reference move is the same as the leading move. */
	void SetUseSameMove( bool useSameMove );
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** Determine if vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** Retrieve the leading move time target. */
	inline aeControllerTarget &GetTargetLeadingMoveTime(){ return pTargetLeadMoveTime; }
	inline const aeControllerTarget &GetTargetLeadingMoveTime() const{ return pTargetLeadMoveTime; }
	
	/** Retrieve the reference move time target. */
	inline aeControllerTarget &GetTargetReferenceMoveTime(){ return pTargetRefMoveTime; }
	inline const aeControllerTarget &GetTargetReferenceMoveTime() const{ return pTargetRefMoveTime; }
	
	/** Creates an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another animator difference rule to this animator difference rule. */
	virtual aeRuleAnimationDifference &operator=( const aeRuleAnimationDifference &copy );
	/*@}*/
};

#endif
