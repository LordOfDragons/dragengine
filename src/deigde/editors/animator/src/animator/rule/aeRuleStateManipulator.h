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

#ifndef _AERULESTATEMANIPULATOR_H_
#define _AERULESTATEMANIPULATOR_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>

/**
 * Animator rule state manipulator.
 */
class aeRuleStateManipulator : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleStateManipulator>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleStateManipulator, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleStateManipulator *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyVectorStorage::Storage mpMinPosition;
	igdeMetaPropertyVectorStorage::Storage mpMaxPosition;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpMinRotation;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpMaxRotation;
	igdeMetaPropertyVectorStorage::Storage mpMinSize;
	igdeMetaPropertyVectorStorage::Storage mpMaxSize;
	igdeMetaPropertyFloatStorage::Storage mpMinVertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage mpMaxVertexPositionSet;
	igdeMetaPropertyBooleanStorage::Storage mpEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage mpEnableRotation;
	igdeMetaPropertyBooleanStorage::Storage mpEnableSize;
	igdeMetaPropertyBooleanStorage::Storage mpEnableVertexPositionSet;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetRotation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetSize;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleStateManipulator() = delete;
	
	/** Create rule. */
	aeRuleStateManipulator(aeWindowMain &windowMain, const char *name);
	
	/** Create copy of rule. */
	aeRuleStateManipulator(const aeRuleStateManipulator &copy);
	
	/** Clean up rule. */
protected:
	~aeRuleStateManipulator() override;
private:
	aeRuleStateManipulator(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
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
	aeRuleStateManipulator &operator=(const aeRuleStateManipulator &copy) = delete;
	/*@}*/
};

#endif
