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

#ifndef _AERULEGROUP_H_
#define _AERULEGROUP_H_

#include "aeRule.h"
#include "aeRuleList.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>



/**
 * Group animator rule.
 */
class aeRuleGroup : public aeRule{
public:
	typedef deTObjectReference<aeRuleGroup> Ref;
	
private:
	aeRuleList pRules;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	bool pUseCurrentState;
	deAnimatorRuleGroup::eApplicationTypes pApplicationType;
	
	aeControllerTarget pTargetSelect;
	
	bool pTreeListExpanded;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new group rule. */
	aeRuleGroup();
	/** Create a copy of a group rule. */
	aeRuleGroup( const aeRuleGroup &copy );
	/** Clean up the group rule. */
	virtual ~aeRuleGroup();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Rules. */
	inline const aeRuleList &GetRules() const{ return pRules; }
	/** Add a new rule. */
	void AddRule( aeRule *rule );
	/** Insert a new rule. */
	void InsertRuleAt( aeRule *rule, int index );
	/** Move a rule to a new position. */
	void MoveRuleTo( aeRule *rule, int index );
	/** Remove the given rule. */
	void RemoveRule( aeRule *rule );
	/** Remove all rules. */
	void RemoveAllRules();
	
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
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** Use current animation state instead of empty state. */
	inline bool GetUseCurrentState() const{ return pUseCurrentState; }
	
	/** Set if current animation state is used instead of empty state. */
	void SetUseCurrentState( bool useCurrentState );
	
	/** Retrieve the rule application type. */
	inline deAnimatorRuleGroup::eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	/** Set the rule application type. */
	void SetApplicationType( deAnimatorRuleGroup::eApplicationTypes type );
	
	/** Retrieve the select target. */
	inline aeControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	inline const aeControllerTarget &GetTargetSelect() const{ return pTargetSelect; }
	
	/** Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Tree list expanded state. */
	inline bool GetTreeListExpanded() const{ return pTreeListExpanded; }
	
	/** Set tree list expanded state. */
	void SetTreeListExpanded( bool expanded );
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	
	/** Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copies another group rule to this group rule. */
	virtual aeRuleGroup &operator=( const aeRuleGroup &copy );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
