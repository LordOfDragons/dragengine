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

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>

/**
 * Animator rule animation difference.
 */
class aeRuleAnimationDifference : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleAnimationDifference>;
	
private:
	aeControllerTarget::Ref pTargetLeadMoveTime;
	aeControllerTarget::Ref pTargetRefMoveTime;
	
	igdeMetaPropertyStringStorage::Storage pMPLeadingMoveName;
	igdeMetaPropertyFloatStorage::Storage pMPLeadingMoveTime;
	igdeMetaPropertyStringStorage::Storage pMPReferenceMoveName;
	igdeMetaPropertyFloatStorage::Storage pMPReferenceMoveTime;
	igdeMetaPropertyBooleanStorage::Storage pMPUseComponentSpace;
	igdeMetaPropertyBooleanStorage::Storage pMPEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableSize;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableVertexPositionSet;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetLeadMoveTime;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetRefMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleAnimationDifference() = delete;
	aeRuleAnimationDifference(const aeRuleAnimationDifference&) = delete;
	
	/** Create a new animator difference rule. */
	explicit aeRuleAnimationDifference(aeWindowMain &windowMain, const char *name);
	/** Create a copy of an animator difference rule. */
	aeRuleAnimationDifference(aeWindowMain &windowMain, const aeRuleAnimationDifference &copy);
	/** Cleans up the animator difference rule. */
protected:
	~aeRuleAnimationDifference() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline igdeMetaPropertyStringStorage::Storage &GetMPLeadingMoveName(){ return pMPLeadingMoveName; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPLeadingMoveTime(){ return pMPLeadingMoveTime; }
	inline igdeMetaPropertyStringStorage::Storage &GetMPReferenceMoveName(){ return pMPReferenceMoveName; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPReferenceMoveTime(){ return pMPReferenceMoveTime; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPUseComponentSpace(){ return pMPUseComponentSpace; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnablePosition(){ return pMPEnablePosition; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableOrientation(){ return pMPEnableOrientation; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableSize(){ return pMPEnableSize; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableVertexPositionSet(){ return pMPEnableVertexPositionSet; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetLeadMoveTime(){ return pMPTargetLeadMoveTime; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetRefMoveTime(){ return pMPTargetRefMoveTime; }
	
	
	/** Retrieve the name of the leading animation move. */
	inline const decString &GetLeadingMoveName() const{ return pMPLeadingMoveName; }
	/** Set the leading animation move name. */
	void SetLeadingMoveName(const char *value);
	/** Retrieve the leading animation move time. */
	inline float GetLeadingMoveTime() const{ return pMPLeadingMoveTime; }
	/** Set the leading animation move time. */
	void SetLeadingMoveTime(float value);
	/** Retrieve the name of the reference animation move. */
	inline const decString &GetReferenceMoveName() const{ return pMPReferenceMoveName; }
	/** Set the reference animation move name. */
	void SetReferenceMoveName(const char *value);
	/** Retrieve the reference animation move time. */
	inline float GetReferenceMoveTime() const{ return pMPReferenceMoveTime; }
	/** Set the reference animation move time. */
	void SetReferenceMoveTime(float value);
	
	/** Use component space. */
	inline bool GetUseComponentSpace() const{ return pMPUseComponentSpace; }
	
	/** Set use component space. */
	void SetUseComponentSpace(bool value);
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pMPEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool value);
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pMPEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool value);
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pMPEnableSize; }
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool value);
	
	/** Determine if vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pMPEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool value);
	
	/** Retrieve the leading move time target. */
	inline const aeControllerTarget::Ref &GetTargetLeadingMoveTime() const{ return pTargetLeadMoveTime; }
	
	/** Retrieve the reference move time target. */
	inline const aeControllerTarget::Ref &GetTargetReferenceMoveTime() const{ return pTargetRefMoveTime; }
	
	/** Creates an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	/** Update targets. */
	void UpdateTargets() override;
	/** Retrieve the number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	/** Removes a link from all targets using it. */
	void RemoveLinkFromTargets(aeLink *link) override;
	/** Removes all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy(aeWindowMain &windowMain) const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleAnimationDifference &operator=(const aeRuleAnimationDifference &copy) = delete;
	/*@}*/
};

#endif
