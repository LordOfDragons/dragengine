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

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>

/**
 * Animator rule animation.
 */
class aeRuleAnimation : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleAnimation>;
	
private:
	aeControllerTarget::Ref pTargetMoveTime;
	
	igdeMetaPropertyStringStorage::Storage pMPMoveName;
	igdeMetaPropertyFloatStorage::Storage pMPMoveTime;
	igdeMetaPropertyBooleanStorage::Storage pMPEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableSize;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableVertexPositionSet;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleAnimation() = delete;
	aeRuleAnimation(const aeRuleAnimation&) = delete;
	
	/** Create a new animator rule. */
	explicit aeRuleAnimation(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a animator rule. */
	aeRuleAnimation(aeWindowMain &windowMain, const aeRuleAnimation &copy);
	/** Clean up the animator rule. */
protected:
	~aeRuleAnimation() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	inline igdeMetaPropertyStringStorage::Storage &GetMPMoveName(){ return pMPMoveName; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPMoveTime(){ return pMPMoveTime; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnablePosition(){ return pMPEnablePosition; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableOrientation(){ return pMPEnableOrientation; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableSize(){ return pMPEnableSize; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableVertexPositionSet(){ return pMPEnableVertexPositionSet; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetMoveTime(){ return pMPTargetMoveTime; }
	
	
	/** Retrieve the name of the animation move. */
	inline const decString &GetMoveName() const{ return pMPMoveName; }
	/** Set the animation move name. */
	void SetMoveName(const char *value);
	/** Retrieve the animation move time. */
	inline float GetMoveTime() const{ return pMPMoveTime; }
	/** Set the animation move time. */
	void SetMoveTime(float value);
	
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
	
	/** Retrieve the move time target. */
	inline const aeControllerTarget::Ref &GetTargetMoveTime() const{ return pTargetMoveTime; }
	
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
	aeRuleAnimation &operator=(const aeRuleAnimation &copy) = delete;
	/*@}*/
};

#endif
