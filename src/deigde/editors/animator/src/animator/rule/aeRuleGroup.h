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

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyList.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/utils/igdeUniqueNameGenerator.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleGroup.h>


/**
 * Group animator rule.
 */
class aeRuleGroup : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleGroup>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleGroup, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleGroup *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
private:
	bool pTreeListExpanded;
	
public:
	igdeMetaPropertyListStorage<aeRule, aeRule::List>::Storage mpRules;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertyBooleanStorage::Storage mpUseCurrentState;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleGroup::eApplicationTypes>::Storage mpApplicationType;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetSelect;
	
public:
	igdeUniqueNameGenerator uniqueNameRule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleGroup() = delete;
	
	/** Create a new group rule. */
	explicit aeRuleGroup(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a group rule. */
	aeRuleGroup(const aeRuleGroup &copy);
	/** Clean up the group rule. */
protected:
	~aeRuleGroup() override;
private:
	aeRuleGroup(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Rules. */
	inline const aeRule::List &GetRules() const{ return mpRules; }
	/** Add a new rule. */
	void AddRule(aeRule *rule);
	/** Insert a new rule. */
	void InsertRuleAt(aeRule *rule, int index);
	/** Move a rule to a new position. */
	void MoveRuleTo(aeRule *rule, int index);
	/** Remove the given rule. */
	void RemoveRule(aeRule *rule);
	/** Remove all rules. */
	void RemoveAllRules();
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return mpEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool value);
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return mpEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool value);
	
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return mpEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool value);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return mpEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool value);
	
	/** Use current animation state instead of empty state. */
	inline bool GetUseCurrentState() const{ return mpUseCurrentState; }
	
	/** Set if current animation state is used instead of empty state. */
	void SetUseCurrentState(bool value);
	
	/** Retrieve the rule application type. */
	inline deAnimatorRuleGroup::eApplicationTypes GetApplicationType() const{ return mpApplicationType; }
	/** Set the rule application type. */
	void SetApplicationType(deAnimatorRuleGroup::eApplicationTypes type);
	
	/** Create an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	/** Update targets. */
	void UpdateTargets() override;
	/** Retrieve the number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	
	/** Tree list expanded state. */
	inline bool GetTreeListExpanded() const{ return pTreeListExpanded; }
	
	/** Set tree list expanded state. */
	void SetTreeListExpanded(bool expanded);
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	
	/** Parent animator changed. */
	void OnParentAnimatorChanged() override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleGroup &operator=(const aeRuleGroup &copy) = delete;
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateRuleIndices();
};

#endif
