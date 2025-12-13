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

#ifndef _AERULEANIMATION_H_
#define _AERULEANIMATION_H_

#include "aeRule.h"



/**
 * Animator rule animation.
 */
class aeRuleAnimation : public aeRule{
public:
	typedef deTObjectReference<aeRuleAnimation> Ref;
	
	
private:
	decString pMoveName;
	float pMoveTime;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	aeControllerTarget pTargetMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new animator rule. */
	aeRuleAnimation();
	/** Create a copy of a animator rule. */
	aeRuleAnimation(const aeRuleAnimation &copy);
	/** Clean up the animator rule. */
	~aeRuleAnimation() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the animation move. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** Set the animation move name. */
	void SetMoveName(const char *moveName);
	/** Retrieve the animation move time. */
	inline float GetMoveTime() const{ return pMoveTime; }
	/** Set the animation move time. */
	void SetMoveTime(float moveTime);
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** Determine if vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	/** Retrieve the move time target. */
	inline aeControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	inline const aeControllerTarget &GetTargetMoveTime() const{ return pTargetMoveTime; }
	
	/** Create an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	/** Update targets. */
	void UpdateTargets() override;
	/** Retrieve the number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	/** Remove a link from all targets using it. */
	void RemoveLinkFromTargets(aeLink *link) override;
	/** Remove all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLinkList& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another animation rule to this rule. */
	virtual aeRuleAnimation &operator=(const aeRuleAnimation &copy);
	/*@}*/
};

#endif
