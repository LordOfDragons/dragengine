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
	
public:
	igdeMetaPropertyVectorStorage::Storage minPosition;
	igdeMetaPropertyVectorStorage::Storage maxPosition;
	igdeMetaPropertyVectorStorage::Storage minRotation;
	igdeMetaPropertyVectorStorage::Storage maxRotation;
	igdeMetaPropertyVectorStorage::Storage minScaling;
	igdeMetaPropertyVectorStorage::Storage maxScaling;
	igdeMetaPropertyFloatStorage::Storage minVertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage maxVertexPositionSet;
	igdeMetaPropertyStringStorage::Storage targetBone;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleLimit::eCoordinateFrames>::Storage coordinateFrame;
	igdeMetaPropertyBooleanStorage::Storage enablePositionXMin;
	igdeMetaPropertyBooleanStorage::Storage enablePositionXMax;
	igdeMetaPropertyBooleanStorage::Storage enablePositionYMin;
	igdeMetaPropertyBooleanStorage::Storage enablePositionYMax;
	igdeMetaPropertyBooleanStorage::Storage enablePositionZMin;
	igdeMetaPropertyBooleanStorage::Storage enablePositionZMax;
	igdeMetaPropertyBooleanStorage::Storage enableRotationXMin;
	igdeMetaPropertyBooleanStorage::Storage enableRotationXMax;
	igdeMetaPropertyBooleanStorage::Storage enableRotationYMin;
	igdeMetaPropertyBooleanStorage::Storage enableRotationYMax;
	igdeMetaPropertyBooleanStorage::Storage enableRotationZMin;
	igdeMetaPropertyBooleanStorage::Storage enableRotationZMax;
	igdeMetaPropertyBooleanStorage::Storage enableScalingXMin;
	igdeMetaPropertyBooleanStorage::Storage enableScalingXMax;
	igdeMetaPropertyBooleanStorage::Storage enableScalingYMin;
	igdeMetaPropertyBooleanStorage::Storage enableScalingYMax;
	igdeMetaPropertyBooleanStorage::Storage enableScalingZMin;
	igdeMetaPropertyBooleanStorage::Storage enableScalingZMax;
	igdeMetaPropertyBooleanStorage::Storage enableVertexPositionSetMin;
	igdeMetaPropertyBooleanStorage::Storage enableVertexPositionSetMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleLimit() = delete;
	aeRuleLimit(const aeRuleLimit&) = delete;
	
	/** Create a new limit rule. */
	explicit aeRuleLimit(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a limit rule. */
	aeRuleLimit(aeWindowMain &windowMain, const aeRuleLimit &copy);
	/** Clean up the limit rule. */
protected:
	~aeRuleLimit() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the minimum position. */
	inline const decVector &GetMinimumPosition() const{ return minPosition; }
	/** Set the minimum position. */
	void SetMinimumPosition(const decVector &value);
	/** Retrieve the maximum position. */
	inline const decVector &GetMaximumPosition() const{ return maxPosition; }
	/** Set the maximum position. */
	void SetMaximumPosition(const decVector &value);
	/** Retrieve the minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return minRotation; }
	/** Set the minimum rotation. */
	void SetMinimumRotation(const decVector &value);
	/** Retrieve the maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return maxRotation; }
	/** Set the maximum rotation. */
	void SetMaximumRotation(const decVector &value);
	
	/** Retrieve the minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return minScaling; }
	
	/** Set the minimum scaling. */
	void SetMinimumScaling(const decVector &value);
	
	/** Retrieve the maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return maxScaling; }
	
	/** Set the maximum scaling. */
	void SetMaximumScaling(const decVector &value);
	
	/** Retrieve the minimum vertex position set. */
	inline float GetMinimumVertexPositionSet() const{ return minVertexPositionSet; }
	
	/** Set the minimum vertex position set. */
	void SetMinimumVertexPositionSet(float weight);
	
	/** Retrieve the maximum vertex position set. */
	inline float GetMaximumVertexPositionSet() const{ return maxVertexPositionSet; }
	
	/** Set the maximum vertex poosition set. */
	void SetMaximumVertexPositionSet(float weight);
	
	/** Retrieve the coordinate frame to rotate around. */
	inline deAnimatorRuleLimit::eCoordinateFrames GetCoordinateFrame() const{ return coordinateFrame; }
	/** Set the coordinate frame to rotate around. */
	void SetCoordinateFrame(deAnimatorRuleLimit::eCoordinateFrames coordinateFrame);
	
	/** Determine if x position is limited to a minimum. */
	inline bool GetEnablePositionXMin() const{ return enablePositionXMin; }
	/** Set if x position is limited to a minimum. */
	void SetEnablePositionXMin(bool enabled);
	/** Determine if x position is limited to a maximum. */
	inline bool GetEnablePositionXMax() const{ return enablePositionXMax; }
	/** Set if x position is limited to a maximum. */
	void SetEnablePositionXMax(bool enabled);
	/** Determine if y position is limited to a minimum. */
	inline bool GetEnablePositionYMin() const{ return enablePositionYMin; }
	/** Set if y position is limited to a minimum. */
	void SetEnablePositionYMin(bool enabled);
	/** Determine if y position is limited to a maximum. */
	inline bool GetEnablePositionYMax() const{ return enablePositionYMax; }
	/** Set if y position is limited to a maximum. */
	void SetEnablePositionYMax(bool enabled);
	/** Determine if z position is limited to a minimum. */
	inline bool GetEnablePositionZMin() const{ return enablePositionZMin; }
	/** Set if z position is limited to a minimum. */
	void SetEnablePositionZMin(bool enabled);
	/** Determine if z position is limited to a maximum. */
	inline bool GetEnablePositionZMax() const{ return enablePositionZMax; }
	/** Set if z position is limited to a maximum. */
	void SetEnablePositionZMax(bool enabled);
	
	/** Determine if x rotation is limited to a minimum. */
	inline bool GetEnableRotationXMin() const{ return enableRotationXMin; }
	/** Set if x rotation is limited to a minimum. */
	void SetEnableRotationXMin(bool enabled);
	/** Determine if x rotation is limited to a maximum. */
	inline bool GetEnableRotationXMax() const{ return enableRotationXMax; }
	/** Set if x rotation is limited to a maximum. */
	void SetEnableRotationXMax(bool enabled);
	/** Determine if y rotation is limited to a minimum. */
	inline bool GetEnableRotationYMin() const{ return enableRotationYMin; }
	/** Set if y rotation is limited to a minimum. */
	void SetEnableRotationYMin(bool enabled);
	/** Determine if y rotation is limited to a maximum. */
	inline bool GetEnableRotationYMax() const{ return enableRotationYMax; }
	/** Set if y rotation is limited to a maximum. */
	void SetEnableRotationYMax(bool enabled);
	/** Determine if z rotation is limited to a minimum. */
	inline bool GetEnableRotationZMin() const{ return enableRotationZMin; }
	/** Set if z rotation is limited to a minimum. */
	void SetEnableRotationZMin(bool enabled);
	/** Determine if z rotation is limited to a maximum. */
	inline bool GetEnableRotationZMax() const{ return enableRotationZMax; }
	/** Set if z rotation is limited to a maximum. */
	void SetEnableRotationZMax(bool enabled);
	
	/** Determine if x scaling is limited to a minimum. */
	inline bool GetEnableScalingXMin() const{ return enableScalingXMin; }
	/** Set if x scaling is limited to a minimum. */
	void SetEnableScalingXMin(bool enabled);
	/** Determine if x scaling is limited to a maximum. */
	inline bool GetEnableScalingXMax() const{ return enableScalingXMax; }
	/** Set if x scaling is limited to a maximum. */
	void SetEnableScalingXMax(bool enabled);
	/** Determine if y scaling is limited to a minimum. */
	inline bool GetEnableScalingYMin() const{ return enableScalingYMin; }
	/** Set if y scaling is limited to a minimum. */
	void SetEnableScalingYMin(bool enabled);
	/** Determine if y scaling is limited to a maximum. */
	inline bool GetEnableScalingYMax() const{ return enableScalingYMax; }
	/** Set if y scaling is limited to a maximum. */
	void SetEnableScalingYMax(bool enabled);
	/** Determine if z scaling is limited to a minimum. */
	inline bool GetEnableScalingZMin() const{ return enableScalingZMin; }
	/** Set if z scaling is limited to a minimum. */
	void SetEnableScalingZMin(bool enabled);
	/** Determine if z scaling is limited to a maximum. */
	inline bool GetEnableScalingZMax() const{ return enableScalingZMax; }
	/** Set if z scaling is limited to a maximum. */
	void SetEnableScalingZMax(bool enabled);
	
	/** Vertex position set is limited to a minimum. */
	inline bool GetEnableVertexPositionSetMin() const{ return enableVertexPositionSetMin; }
	
	/** Set if vertex position set is limited to a minimum. */
	void SetEnableVertexPositionSetMin(bool enabled);
	
	/** Vertex position set is limited to a maximum. */
	inline bool GetEnableVertexPositionSetMax() const{ return enableVertexPositionSetMax; }
	
	/** Set if vertex position set is limited to a maximum. */
	void SetEnableVertexPositionSetMax(bool enabled);
	
	/** Retrieve the name of the target bone. */
	inline const decString &GetTargetBone() const{ return targetBone; }
	
	/** Set the name of the target bone. */
	void SetTargetBone(const char *boneName);
	
	/** Creates an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy(aeWindowMain &windowMain) const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleLimit &operator=(const aeRuleLimit &copy) = delete;
	/*@}*/
};

#endif
