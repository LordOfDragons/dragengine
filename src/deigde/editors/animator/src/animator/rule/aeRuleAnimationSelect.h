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

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyStringList.h>

#include <dragengine/common/string/decStringList.h>


/**
 * Animator rule animation select.
 */
class aeRuleAnimationSelect : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleAnimationSelect>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleAnimationSelect, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleAnimationSelect *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyStringListStorage::Storage mpMoves;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetMoveTime;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetSelect;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleAnimationSelect() = delete;
	
	/** Create animator select rule. */
	explicit aeRuleAnimationSelect(aeWindowMain &windowMain, const char *name);
	
	/** Create copy of animator select rule. */
	aeRuleAnimationSelect(const aeRuleAnimationSelect &copy);
	
	/** Clean up animator select rule. */
protected:
	~aeRuleAnimationSelect() override;
private:
	aeRuleAnimationSelect(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Create engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	
	
	/** Update targets. */
	void UpdateTargets() override;
	
	/** Number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	
	
	/** Create copy of rule. */
	aeRule::Ref CreateCopy() const override;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	aeRuleAnimationSelect &operator=(const aeRuleAnimationSelect &copy) = delete;
	/*@}*/
};

#endif
