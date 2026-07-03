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
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleAnimationDifference, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleAnimationDifference *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyStringStorage::Storage mpLeadingMoveName;
	igdeMetaPropertyFloatStorage::Storage mpLeadingMoveTime;
	igdeMetaPropertyStringStorage::Storage mpReferenceMoveName;
	igdeMetaPropertyFloatStorage::Storage mpReferenceMoveTime;
	igdeMetaPropertyBooleanStorage::Storage mpUseComponentSpace;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetLeadingMoveTime;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetReferenceMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleAnimationDifference() = delete;
	
	/** Create a new animator difference rule. */
	explicit aeRuleAnimationDifference(aeWindowMain &windowMain, const char *name);
	/** Create a copy of an animator difference rule. */
	aeRuleAnimationDifference(const aeRuleAnimationDifference &copy);
	/** Cleans up the animator difference rule. */
protected:
	~aeRuleAnimationDifference() override;
private:
	aeRuleAnimationDifference(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the leading animation move. */
	inline const decString &GetLeadingMoveName() const{ return mpLeadingMoveName; }
	/** Set the leading animation move name. */
	void SetLeadingMoveName(const char *value);
	/** Retrieve the leading animation move time. */
	inline float GetLeadingMoveTime() const{ return mpLeadingMoveTime; }
	/** Set the leading animation move time. */
	void SetLeadingMoveTime(float value);
	/** Retrieve the name of the reference animation move. */
	inline const decString &GetReferenceMoveName() const{ return mpReferenceMoveName; }
	/** Set the reference animation move name. */
	void SetReferenceMoveName(const char *value);
	/** Retrieve the reference animation move time. */
	inline float GetReferenceMoveTime() const{ return mpReferenceMoveTime; }
	/** Set the reference animation move time. */
	void SetReferenceMoveTime(float value);
	
	/** Use component space. */
	inline bool GetUseComponentSpace() const{ return mpUseComponentSpace; }
	
	/** Set use component space. */
	void SetUseComponentSpace(bool value);
	
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
	
	/** Determine if vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return mpEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool value);
	
	/** Creates an engine animator rule. */
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
	aeRuleAnimationDifference &operator=(const aeRuleAnimationDifference &copy) = delete;
	/*@}*/
};

#endif
