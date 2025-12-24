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

#ifndef _AERULEANIMATIONSELECT_H_
#define _AERULEANIMATIONSELECT_H_

#include "aeRule.h"

#include <dragengine/common/string/decStringList.h>



/**
 * Animator rule animation select.
 */
class aeRuleAnimationSelect : public aeRule{
public:
	typedef deTObjectReference<aeRuleAnimationSelect> Ref;
	
	
private:
	decStringList pMoves;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	aeControllerTarget pTargetMoveTime;
	aeControllerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create animator select rule. */
	aeRuleAnimationSelect();
	
	/** Create copy of animator select rule. */
	aeRuleAnimationSelect(const aeRuleAnimationSelect &copy);
	
	/** Clean up animator select rule. */
	~aeRuleAnimationSelect() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Moves. */
	inline const decStringList &GetMoves() const{ return pMoves; }
	
	/** Set moves. */
	void SetMoves(const decStringList &moves);
	
	
	
	/** Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	
	/** Orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	
	/** Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	
	
	/** Move time target. */
	inline aeControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	inline const aeControllerTarget &GetTargetMoveTime() const{ return pTargetMoveTime; }
	
	/** Select target. */
	inline aeControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const aeControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	
	
	/** Create engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	
	
	/** Update targets. */
	void UpdateTargets() override;
	
	/** Number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	
	/** Remove link from all targets using it. */
	void RemoveLinkFromTargets(aeLink *link) override;
	
	/** Remove all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	
	
	/** Create copy of rule. */
	aeRule::Ref CreateCopy() const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy another animator difference rule to this animator difference rule. */
	virtual aeRuleAnimationSelect &operator=(const aeRuleAnimationSelect &copy);
	/*@}*/
};

#endif
