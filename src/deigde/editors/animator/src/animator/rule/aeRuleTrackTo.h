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
	
private:
	aeControllerTarget::Ref pTargetPosition;
	aeControllerTarget::Ref pTargetUp;
	
	igdeMetaPropertyStringStorage::Storage pMPTrackBone;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>::Storage pMPTrackAxis;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>::Storage pMPUpAxis;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eUpTarget>::Storage pMPUpTarget;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eLockedAxis>::Storage pMPLockedAxis;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetUp;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleTrackTo() = delete;
	aeRuleTrackTo(const aeRuleTrackTo&) = delete;
	
	/** Create a new track to rule. */
	aeRuleTrackTo(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a track to rule. */
	aeRuleTrackTo(aeWindowMain &windowMain, const aeRuleTrackTo &copy);
	/** Clean up the animator rule. */
protected:
	~aeRuleTrackTo() override;
private:
	aeRuleTrackTo(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline igdeMetaPropertyStringStorage::Storage &GetMPTrackBone(){ return pMPTrackBone; }
	inline igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>::Storage &GetMPTrackAxis(){ return pMPTrackAxis; }
	inline igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eTrackAxis>::Storage &GetMPUpAxis(){ return pMPUpAxis; }
	inline igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eUpTarget>::Storage &GetMPUpTarget(){ return pMPUpTarget; }
	inline igdeMetaPropertySelectionEnumStorage<deAnimatorRuleTrackTo::eLockedAxis>::Storage &GetMPLockedAxis(){ return pMPLockedAxis; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetPosition(){ return pMPTargetPosition; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetUp(){ return pMPTargetUp; }
	
	
	/** Retrieve the name of the track bone or empty string to use none. */
	inline const decString &GetTrackBone() const{ return pMPTrackBone; }
	/** Set the name of the track bone or empty string to use none. */
	void SetTrackBone(const char *boneName);
	/** Retrieve the track axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetTrackAxis() const{ return pMPTrackAxis; }
	/** Set the track axis. */
	void SetTrackAxis(deAnimatorRuleTrackTo::eTrackAxis axis);
	/** Retrieve the up axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetUpAxis() const{ return pMPUpAxis; }
	/** Set the up axis. */
	void SetUpAxis(deAnimatorRuleTrackTo::eTrackAxis axis);
	/** Retrieve the up target. */
	inline deAnimatorRuleTrackTo::eUpTarget GetUpTarget() const{ return pMPUpTarget; }
	/** Set the up target. */
	void SetUpTarget(deAnimatorRuleTrackTo::eUpTarget target);
	/** Retrieve the locked axis. */
	inline deAnimatorRuleTrackTo::eLockedAxis GetLockedAxis() const{ return pMPLockedAxis; }
	/** Set the locked axis. */
	void SetLockedAxis(deAnimatorRuleTrackTo::eLockedAxis axis);
	
	/** Retrieve the position target. */
	inline const aeControllerTarget::Ref &GetTargetPosition() const{ return pTargetPosition; }
	
	/** Retrieve the up target. */
	inline const aeControllerTarget::Ref &GetTargetUp() const{ return pTargetUp; }
	
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
	aeRule::Ref CreateCopy(aeWindowMain &windowMain) const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleTrackTo &operator=(const aeRuleTrackTo &copy) = delete;
	/*@}*/
};

#endif
