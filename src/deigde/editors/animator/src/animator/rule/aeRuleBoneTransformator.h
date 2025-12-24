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

#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>

#include "aeRule.h"


/**
 * Animator rule bone transformator.
 */
class aeRuleBoneTransformator : public aeRule{
public:
	typedef deTObjectReference<aeRuleBoneTransformator> Ref;
	
	
private:
	decVector pMinTranslation, pMaxTranslation, pMinRotation, pMaxRotation;
	decVector pMinScaling, pMaxScaling, pAxis;
	float pMinAngle, pMaxAngle;
	
	deAnimatorRuleBoneTransformator::eCoordinateFrames pCoordinateFrame;
	bool pEnablePosition, pEnableOrientation, pEnableSize, pUseAxis;
	
	decString pTargetBone, pInputBone;
	deAnimatorRuleBoneTransformator::eInputSources pInputSource;
	
	aeControllerTarget pTargetTranslation, pTargetRotation, pTargetScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new bone transformator rule. */
	aeRuleBoneTransformator();
	/** Create a copy of a bone transformator rule. */
	aeRuleBoneTransformator(const aeRuleBoneTransformator &copy);
	/** Clean up the bone transformator rule. */
	~aeRuleBoneTransformator() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the minimum translation. */
	inline const decVector &GetMinimumTranslation() const{ return pMinTranslation; }
	/** Set the minimum translation. */
	void SetMinimumTranslation(const decVector &translation);
	/** Retrieve the maximum translation. */
	inline const decVector &GetMaximumTranslation() const{ return pMaxTranslation; }
	/** Set the maximum translation. */
	void SetMaximumTranslation(const decVector &translation);
	/** Retrieve the minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	/** Set the minimum rotation. */
	void SetMinimumRotation(const decVector &rotation);
	/** Retrieve the maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	/** Set the maximum rotation. */
	void SetMaximumRotation(const decVector &rotation);
	/** Retrieve the minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	/** Set the minimum scaling. */
	void SetMinimumScaling(const decVector &scaling);
	/** Retrieve the maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	/** Set the maximum scaling. */
	void SetMaximumScaling(const decVector &scaling);
	
	/** Rotation axis. */
	inline const decVector &GetAxis() const{ return pAxis; }
	
	/** Set rotation axis. */
	void SetAxis(const decVector &axis);
	
	/** Minimum axis rotation angle. */
	inline float GetMinimumAngle() const{ return pMinAngle; }
	
	/** Set minimum axis rotation angle. */
	void SetMinimumAngle(float angle);
	
	/** Maximum axis rotation angle. */
	inline float GetMaximumAngle() const{ return pMaxAngle; }
	
	/** Set maximum axis rotation angle. */
	void SetMaximumAngle(float angle);
	
	/** Retrieve the coordinate frame to rotate around. */
	inline deAnimatorRuleBoneTransformator::eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	/** Set the coordinate frame to rotate around. */
	void SetCoordinateFrame(deAnimatorRuleBoneTransformator::eCoordinateFrames coordinateFrame);
	
	/** Determine if position is altered. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** Set if position is altered. */
	void SetEnablePosition(bool enabled);
	/** Determine if orientation is altered. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** Set if orientation is altered. */
	void SetEnableOrientation(bool enabled);
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** Use rotation axis instead of rotation directly. */
	inline bool GetUseAxis() const{ return pUseAxis; }
	
	/** Set to use rotation axis instead of rotation directly. */
	void SetUseAxis(bool useAxis);
	
	/** Retrieve the name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	/** Set the name of the target bone. */
	void SetTargetBone(const char *boneName);
	
	/** Name of the input bone. */
	inline const decString &GetInputBone() const{ return pInputBone; }
	
	/** Set name of input bone. */
	void SetInputBone(const char *boneName);
	
	/** Input source. */
	inline deAnimatorRuleBoneTransformator::eInputSources GetInputSource() const{ return pInputSource; }
	
	/** Set input source. */
	void SetInputSource(deAnimatorRuleBoneTransformator::eInputSources source);
	
	/** Retrieve the translation target. */
	inline aeControllerTarget &GetTargetTranslation(){ return pTargetTranslation; }
	inline const aeControllerTarget &GetTargetTranslation() const{ return pTargetTranslation; }
	
	/** Retrieve the rotation target. */
	inline aeControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	inline const aeControllerTarget &GetTargetRotation() const{ return pTargetRotation; }
	
	/** Retrieve the scaling target. */
	inline aeControllerTarget &GetTargetScaling(){ return pTargetScaling; }
	inline const aeControllerTarget &GetTargetScaling() const{ return pTargetScaling; }
	
	/** Creates an engine animator rule. */
	deAnimatorRule::Ref CreateEngineRule() override;
	/** Update targets. */
	void UpdateTargets() override;
	/** Retrieve the number of targets using a given link. */
	int CountLinkUsage(aeLink *link) const override;
	/** Removes a link from all targets using it. */
	void RemoveLinkFromTargets(aeLink *link) override;
	/** Removes all links from all targets. */
	void RemoveLinksFromAllTargets() override;
	
	/** Create a copy of this rule. */
	aeRule::Ref CreateCopy() const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another animation rule to this rule. */
	virtual aeRuleBoneTransformator &operator=(const aeRuleBoneTransformator &copy);
	/*@}*/
};

#endif
