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
	/** Retrieve the minimum translation. */
	inline const decVector &GetMinimumTranslation() const{ return mpMinTranslation; }
	/** Set the minimum translation. */
	void SetMinimumTranslation(const decVector &value);
	/** Retrieve the maximum translation. */
	inline const decVector &GetMaximumTranslation() const{ return mpMaxTranslation; }
	/** Set the maximum translation. */
	void SetMaximumTranslation(const decVector &value);
	/** Retrieve the minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return mpMinRotation; }
	/** Set the minimum rotation. */
	void SetMinimumRotation(const decVector &value);
	/** Retrieve the maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return mpMaxRotation; }
	/** Set the maximum rotation. */
	void SetMaximumRotation(const decVector &value);
	/** Retrieve the minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return mpMinScaling; }
	/** Set the minimum scaling. */
	void SetMinimumScaling(const decVector &value);
	/** Retrieve the maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return mpMaxScaling; }
	/** Set the maximum scaling. */
	void SetMaximumScaling(const decVector &value);
	
	/** Rotation axis. */
	inline const decVector &GetAxis() const{ return mpAxis; }
	
	/** Set rotation axis. */
	void SetAxis(const decVector &value);
	
	/** Minimum axis rotation angle. */
	inline float GetMinimumAngle() const{ return mpMinAngle; }
	
	/** Set minimum axis rotation angle. */
	void SetMinimumAngle(float value);
	
	/** Maximum axis rotation angle. */
	inline float GetMaximumAngle() const{ return mpMaxAngle; }
	
	/** Set maximum axis rotation angle. */
	void SetMaximumAngle(float value);
	
	/** Retrieve the coordinate frame to rotate around. */
	inline deAnimatorRuleBoneTransformator::eCoordinateFrames GetCoordinateFrame() const{ return mpCoordinateFrame; }
	/** Set the coordinate frame to rotate around. */
	void SetCoordinateFrame(deAnimatorRuleBoneTransformator::eCoordinateFrames coordinateFrame);
	
	/** Determine if position is altered. */
	inline bool GetEnablePosition() const{ return mpEnablePosition; }
	/** Set if position is altered. */
	void SetEnablePosition(bool value);
	/** Determine if orientation is altered. */
	inline bool GetEnableOrientation() const{ return mpEnableOrientation; }
	/** Set if orientation is altered. */
	void SetEnableOrientation(bool value);
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return mpEnableSize; }
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool value);
	
	/** Use rotation axis instead of rotation directly. */
	inline bool GetUseAxis() const{ return mpUseAxis; }
	
	/** Set to use rotation axis instead of rotation directly. */
	void SetUseAxis(bool value);
	
	/** Retrieve the name of the target bone. */
	inline const decString &GetTargetBone() const{ return mpTargetBone; }
	/** Set the name of the target bone. */
	void SetTargetBone(const char *boneName);
	
	/** Name of the input bone. */
	inline const decString &GetInputBone() const{ return mpInputBone; }
	
	/** Set name of input bone. */
	void SetInputBone(const char *boneName);
	
	/** Input source. */
	inline deAnimatorRuleBoneTransformator::eInputSources GetInputSource() const{ return mpInputSource; }
	
	/** Set input source. */
	void SetInputSource(deAnimatorRuleBoneTransformator::eInputSources source);
	
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
