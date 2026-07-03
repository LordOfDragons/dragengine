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
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleAnimation, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleAnimation *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyStringStorage::Storage mpMoveName;
	igdeMetaPropertyFloatStorage::Storage mpMoveTime;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleAnimation() = delete;
	
	/** Create a new animator rule. */
	explicit aeRuleAnimation(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a animator rule. */
	aeRuleAnimation(const aeRuleAnimation &copy);
	/** Clean up the animator rule. */
protected:
	~aeRuleAnimation() override;
private:
	explicit aeRuleAnimation(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the animation move. */
	inline const decString &GetMoveName() const{ return mpMoveName; }
	/** Set the animation move name. */
	void SetMoveName(const char *value);
	/** Retrieve the animation move time. */
	inline float GetMoveTime() const{ return mpMoveTime; }
	/** Set the animation move time. */
	void SetMoveTime(float value);
	
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
	aeRuleAnimation &operator=(const aeRuleAnimation &copy) = delete;
	/*@}*/
};

#endif
