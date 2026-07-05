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

#ifndef _AERULEINVERSEKINEMATIC_H_
#define _AERULEINVERSEKINEMATIC_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertyVector.h>

/**
 * Animator rule inverse kinematic.
 */
class aeRuleInverseKinematic : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleInverseKinematic>;
	
	using MetaContext = igdeMetaContextTypeInherit<aeRuleInverseKinematic, aeRule>;
	static MetaContext::Ref CreateMetaContext(aeWindowMain &windowMain, aeRuleInverseKinematic *rule);
	
	template<typename T>
	using MetaProperty = igdeMetaPropertyMCT<T, MetaContext>;
	
public:
	igdeMetaPropertyVectorStorage::Storage mpGoalPosition;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpGoalOrientation;
	igdeMetaPropertyVectorStorage::Storage mpLocalPosition;
	igdeMetaPropertyVectorStorageQuaternion::Storage mpLocalOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpAdjustOrientation;
	igdeMetaPropertyBooleanStorage::Storage mpUseSolverBone;
	igdeMetaPropertyStringStorage::Storage mpSolverBone;
	igdeMetaPropertyFloatStorage::Storage mpReachRange;
	igdeMetaPropertyStringStorage::Storage mpReachBone;
	igdeMetaPropertyVectorStorage::Storage mpReachCenter;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetGoalPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetGoalOrientation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetLocalPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetLocalOrientation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetReachRange;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage mpTargetReachCenter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleInverseKinematic() = delete;
	
	/** Create inverse kinematic rule. */
	aeRuleInverseKinematic(aeWindowMain &windowMain, const char *name);
	
	/** Create copy of an inverse kinematic rule. */
	aeRuleInverseKinematic(const aeRuleInverseKinematic &copy);
	
	/** Clean up the animator rule. */
protected:
	~aeRuleInverseKinematic() override;
private:
	aeRuleInverseKinematic(aeWindowMain &windowMain, const char *name, const MetaContext::Ref &metaContext);
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Create engine animator rule. */
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
	aeRuleInverseKinematic &operator=(const aeRuleInverseKinematic &copy) = delete;
	/*@}*/
};

#endif
