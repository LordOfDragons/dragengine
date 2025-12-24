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

#ifndef _AERULESUBANIMATOR_H_
#define _AERULESUBANIMATOR_H_

#include "aeRule.h"
#include "../controller/aeController.h"

#include <dragengine/resources/animator/deAnimator.h>

class aeLoadSaveSystem;
class deAnimatorRuleSubAnimator;



/**
 * Animator rule sub animator.
 */
class aeRuleSubAnimator : public aeRule{
public:
	typedef deTObjectReference<aeRuleSubAnimator> Ref;
	
	
private:
	decString pPathSubAnimator;
	deAnimator::Ref pSubAnimator;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	aeController::List pConnections;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new sub animator rule. */
	aeRuleSubAnimator();
	/** Create a copy of a sub animator rule. */
	aeRuleSubAnimator(const aeRuleSubAnimator &copy);
	/** Clean up the sub animator rule. */
	~aeRuleSubAnimator() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the path to the sub animator. */
	inline const decString &GetPathSubAnimator() const{ return pPathSubAnimator; }
	/** Set the path to the sub animator. */
	void SetPathSubAnimator(const char *path);
	/** Retrieve the sub animator or nullptr if not existing. */
	inline const deAnimator::Ref &GetSubAnimator() const{ return pSubAnimator; }
	/** Load the sub animator using the stored path. */
	void LoadSubAnimator();
	
	/** Connections. */
	inline const aeController::List &GetConnections() const{ return pConnections; }
	
	/** Set controller for target controller or \em nullptr. */
	void SetControllerAt(int position, aeController *controller);
	
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
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	/** Create an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Update Component and Animation. */
	void UpdateCompAnim() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	
	/** Parent animator changed. */
	void OnParentAnimatorChanged() override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another sub animator rule to this sub animator rule. */
	virtual aeRuleSubAnimator &operator=(const aeRuleSubAnimator &copy);
	/*@}*/
	
private:
	void pUpdateConnections(deAnimatorRuleSubAnimator &rule) const;
};

#endif
