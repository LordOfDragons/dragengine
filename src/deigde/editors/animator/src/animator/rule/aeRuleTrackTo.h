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

#ifndef _AERULETRACKTO_H_
#define _AERULETRACKTO_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>


/**
 * Animator rule track to.
 */
class aeRuleTrackTo : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleTrackTo>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleTrackTo, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleTrackTo *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyStringStorage::Storage mpTrackBone;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>::Storage mpTrackAxis;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>::Storage mpUpAxis;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eUpTarget>::Storage mpUpTarget;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eLockedAxis>::Storage mpLockedAxis;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetUp;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleTrackTo() = delete;
	
	/** Create a new track to rule. */
	aeRuleTrackTo(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a track to rule. */
	aeRuleTrackTo(const aeRuleTrackTo &copy);
	/** Clean up the animator rule. */
protected:
	~aeRuleTrackTo() override;
private:
	aeRuleTrackTo(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the track bone or empty string to use none. */
	inline const decString &GetTrackBone() const{ return mpTrackBone; }
	/** Set the name of the track bone or empty string to use none. */
	void SetTrackBone(const char *boneName);
	/** Retrieve the track axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetTrackAxis() const{ return mpTrackAxis; }
	/** Set the track axis. */
	void SetTrackAxis(deAnimatorRuleTrackTo::eTrackAxis axis);
	/** Retrieve the up axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetUpAxis() const{ return mpUpAxis; }
	/** Set the up axis. */
	void SetUpAxis(deAnimatorRuleTrackTo::eTrackAxis axis);
	/** Retrieve the up target. */
	inline deAnimatorRuleTrackTo::eUpTarget GetUpTarget() const{ return mpUpTarget; }
	/** Set the up target. */
	void SetUpTarget(deAnimatorRuleTrackTo::eUpTarget target);
	/** Retrieve the locked axis. */
	inline deAnimatorRuleTrackTo::eLockedAxis GetLockedAxis() const{ return mpLockedAxis; }
	/** Set the locked axis. */
	void SetLockedAxis(deAnimatorRuleTrackTo::eLockedAxis axis);
	
	/** Create an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	/** Update targets. */
	void UpdateTargets() override;
	/** Retrieve the number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleTrackTo &operator=(const aeRuleTrackTo &copy) = delete;
	/*@}*/
};

#endif
