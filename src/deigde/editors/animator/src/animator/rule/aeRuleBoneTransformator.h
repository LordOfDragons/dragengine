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

#ifndef _AERULEBONETRANSFORMATOR_H_
#define _AERULEBONETRANSFORMATOR_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>

/**
 * Animator rule bone transformator.
 */
class aeRuleBoneTransformator : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleBoneTransformator>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleBoneTransformator, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleBoneTransformator *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyVectorStorage::Storage mpMinTranslation;
	igdeMetaPropertyVectorStorage::Storage mpMaxTranslation;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpMinRotation;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpMaxRotation;
	igdeMetaPropertyVectorStorage::Storage mpMinScaling;
	igdeMetaPropertyVectorStorage::Storage mpMaxScaling;
	igdeMetaPropertyVectorStorage::Storage mpAxis;
	igdeMetaPropertyFloatStorage::Storage mpMinAngle;
	igdeMetaPropertyFloatStorage::Storage mpMaxAngle;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpUseAxis;
	igdeMetaPropertyStringStorage::Storage mpTargetBone;
	igdeMetaPropertyStringStorage::Storage mpInputBone;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleBoneTransformator::eCoordinateFrames>::Storage mpCoordinateFrame;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleBoneTransformator::eInputSources>::Storage mpInputSource;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetTranslation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetRotation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleBoneTransformator() = delete;
	
	/** Create a new bone transformator rule. */
	explicit aeRuleBoneTransformator(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a bone transformator rule. */
	aeRuleBoneTransformator(const aeRuleBoneTransformator &copy);
	/** Clean up the bone transformator rule. */
protected:
	~aeRuleBoneTransformator() override;
private:
	aeRuleBoneTransformator(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
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
	aeRuleBoneTransformator &operator=(const aeRuleBoneTransformator &copy) = delete;
	/*@}*/
};

#endif
