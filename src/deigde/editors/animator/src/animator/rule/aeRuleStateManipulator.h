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
	
private:
	aeControllerTarget::Ref pTargetPosition;
	aeControllerTarget::Ref pTargetRotation;
	aeControllerTarget::Ref pTargetSize;
	aeControllerTarget::Ref pTargetVertexPositionSet;
	
public:
	igdeMetaPropertyVectorStorage::Storage minPosition;
	igdeMetaPropertyVectorStorage::Storage maxPosition;
	igdeMetaPropertyVectorStorage::Storage minRotation;
	igdeMetaPropertyVectorStorage::Storage maxRotation;
	igdeMetaPropertyVectorStorage::Storage minSize;
	igdeMetaPropertyVectorStorage::Storage maxSize;
	igdeMetaPropertyFloatStorage::Storage minVertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage maxVertexPositionSet;
	igdeMetaPropertyBooleanStorage::Storage enablePosition;
	igdeMetaPropertyBooleanStorage::Storage enableRotation;
	igdeMetaPropertyBooleanStorage::Storage enableSize;
	igdeMetaPropertyBooleanStorage::Storage enableVertexPositionSet;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetRotation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetSize;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleStateManipulator() = delete;
	aeRuleStateManipulator(const aeRuleStateManipulator&) = delete;
	
	/** Create rule. */
	aeRuleStateManipulator(aeWindowMain &windowMain, const char *name);
	
	/** Create copy of rule. */
	aeRuleStateManipulator(aeWindowMain &windowMain, const aeRuleStateManipulator &copy);
	
	/** Clean up rule. */
protected:
	~aeRuleStateManipulator() override;
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Minimum position. */
	inline const decVector &GetMinimumPosition() const{ return minPosition; }
	
	/** Set minimum position. */
	void SetMinimumPosition(const decVector &value);
	
	/** Maximum position. */
	inline const decVector &GetMaximumPosition() const{ return maxPosition; }
	
	/** Set maximum position. */
	void SetMaximumPosition(const decVector &value);
	
	/** Minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return minRotation; }
	
	/** Set minimum rotation. */
	void SetMinimumRotation(const decVector &value);
	
	/** Maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return maxRotation; }
	
	/** Set maximum rotation. */
	void SetMaximumRotation(const decVector &value);
	
	/** Minimum size. */
	inline const decVector &GetMinimumSize() const{ return minSize; }
	
	/** Set minimum size. */
	void SetMinimumSize(const decVector &value);
	
	/** Maximum size. */
	inline const decVector &GetMaximumSize() const{ return maxSize; }
	
	/** Set maximum size. */
	void SetMaximumSize(const decVector &value);
	
	/** Minimum vertex position set. */
	inline float GetMinimumVertexPositionSet() const{ return minVertexPositionSet; }
	
	/** Set minimum vertex position set. */
	void SetMinimumVertexPositionSet(float weight);
	
	/** Maximum vertex position set. */
	inline float GetMaximumVertexPositionSet() const{ return maxVertexPositionSet; }
	
	/** Set maximum vertex position set. */
	void SetMaximumVertexPositionSet(float weight);
	
	
	
	/** Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return enablePosition; }
	
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool value);
	
	/** Rotation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return enableRotation; }
	
	/** Set if rotation manipulation is enabled. */
	void SetEnableRotation(bool value);
	
	/** Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return enableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool value);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return enableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool value);
	
	
	
	/** Position target. */
	inline const aeControllerTarget::Ref &GetTargetPosition() const{ return pTargetPosition; }
	
	/** Rotation target. */
	inline const aeControllerTarget::Ref &GetTargetRotation() const{ return pTargetRotation; }
	
	/** Size target. */
	inline const aeControllerTarget::Ref &GetTargetSize() const{ return pTargetSize; }
	
	/** Vertex position set target. */
	inline const aeControllerTarget::Ref &GetTargetVertexPositionSet() const{ return pTargetVertexPositionSet; }
	
	
	
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
	aeRuleStateManipulator &operator=(const aeRuleStateManipulator &copy) = delete;
	/*@}*/
};

#endif
