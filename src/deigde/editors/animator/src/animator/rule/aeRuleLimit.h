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

#ifndef _AERULELIMIT_H_
#define _AERULELIMIT_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>

/**
 * Animator limit rule.
 */
class aeRuleLimit : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleLimit>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleLimit, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleLimit *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyVectorStorage::Storage mpMinPosition;
	igdeMetaPropertyVectorStorage::Storage mpMaxPosition;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpMinRotation;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpMaxRotation;
	igdeMetaPropertyVectorStorage::Storage mpMinScaling;
	igdeMetaPropertyVectorStorage::Storage mpMaxScaling;
	igdeMetaPropertyFloatStorage::Storage mpMinVertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage mpMaxVertexPositionSet;
	igdeMetaPropertyStringStorage::Storage mpTargetBone;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleLimit::eCoordinateFrames>::Storage mpCoordinateFrame;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePositionXMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePositionXMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePositionYMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePositionYMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePositionZMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePositionZMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnableRotationXMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnableRotationXMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnableRotationYMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnableRotationYMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnableRotationZMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnableRotationZMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnableScalingXMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnableScalingXMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnableScalingYMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnableScalingYMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnableScalingZMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnableScalingZMax;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSetMin;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSetMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleLimit() = delete;
	
	/** Create a new limit rule. */
	explicit aeRuleLimit(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a limit rule. */
	aeRuleLimit(const aeRuleLimit &copy);
	/** Clean up the limit rule. */
protected:
	~aeRuleLimit() override;
private:
	aeRuleLimit(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Creates an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleLimit &operator=(const aeRuleLimit &copy) = delete;
	/*@}*/
};

#endif
