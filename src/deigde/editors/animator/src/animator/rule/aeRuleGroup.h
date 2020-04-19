/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AERULEGROUP_H_
#define _AERULEGROUP_H_

#include "aeRule.h"
#include "aeRuleList.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>



/**
 * \brief Group animator rule.
 */
class aeRuleGroup : public aeRule{
private:
	aeRuleList pRules;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	bool pUseCurrentState;
	deAnimatorRuleGroup::eApplicationTypes pApplicationType;
	
	aeControllerTarget pTargetSelect;
	
	bool pTreeListExpanded;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new group rule. */
	aeRuleGroup();
	/** \brief Create a copy of a group rule. */
	aeRuleGroup( const aeRuleGroup &copy );
	/** \brief Clean up the group rule. */
	virtual ~aeRuleGroup();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Rules. */
	inline const aeRuleList &GetRules() const{ return pRules; }
	/** \brief Add a new rule. */
	void AddRule( aeRule *rule );
	/** \brief Insert a new rule. */
	void InsertRuleAt( aeRule *rule, int index );
	/** \brief Move a rule to a new position. */
	void MoveRuleTo( aeRule *rule, int index );
	/** \brief Remove the given rule. */
	void RemoveRule( aeRule *rule );
	/** \brief Remove all rules. */
	void RemoveAllRules();
	
	/** \brief Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** \brief Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	/** \brief Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** \brief Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	/** \brief Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Use current animation state instead of empty state. */
	inline bool GetUseCurrentState() const{ return pUseCurrentState; }
	
	/** \brief Set if current animation state is used instead of empty state. */
	void SetUseCurrentState( bool useCurrentState );
	
	/** \brief Retrieve the rule application type. */
	inline deAnimatorRuleGroup::eApplicationTypes GetApplicationType() const{ return pApplicationType; }
	/** \brief Set the rule application type. */
	void SetApplicationType( deAnimatorRuleGroup::eApplicationTypes type );
	
	/** \brief Retrieve the select target. */
	inline aeControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	
	/** \brief Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** \brief Update targets. */
	virtual void UpdateTargets();
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Tree list expanded state. */
	inline bool GetTreeListExpanded() const{ return pTreeListExpanded; }
	
	/** \brief Set tree list expanded state. */
	void SetTreeListExpanded( bool expanded );
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	
	/** \brief Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copies another group rule to this group rule. */
	virtual aeRuleGroup &operator=( const aeRuleGroup &copy );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
