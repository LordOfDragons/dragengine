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

#ifndef _AERULEFOREIGNSTATE_H_
#define _AERULEFOREIGNSTATE_H_

#include "aeRule.h"

#include <deigde/meta/property/igdeMetaPropertyBoolean.h>
#include <deigde/meta/property/igdeMetaPropertyFloat.h>
#include <deigde/meta/property/igdeMetaPropertySet.h>
#include <deigde/meta/property/igdeMetaPropertyString.h>
#include <deigde/meta/property/igdeMetaPropertySelection.h>

#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>

/**
 * Foreign state rule.
 */
class aeRuleForeignState : public aeRule{
public:
	using Ref = deTObjectReference<aeRuleForeignState>;
	
private:
	aeControllerTarget::Ref pTargetPosition;
	aeControllerTarget::Ref pTargetOrientation;
	aeControllerTarget::Ref pTargetSize;
	aeControllerTarget::Ref pTargetVertexPositionSet;
	
public:
	igdeMetaPropertyStringStorage::Storage foreignBone;
	igdeMetaPropertyStringStorage::Storage foreignVertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage scalePosition;
	igdeMetaPropertyFloatStorage::Storage scaleOrientation;
	igdeMetaPropertyFloatStorage::Storage scaleSize;
	igdeMetaPropertyFloatStorage::Storage scaleVertexPositionSet;
	igdeMetaPropertyBooleanStorage::Storage enablePosition;
	igdeMetaPropertyBooleanStorage::Storage enableOrientation;
	igdeMetaPropertyBooleanStorage::Storage enableSize;
	igdeMetaPropertyBooleanStorage::Storage enableVertexPositionSet;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage sourceCoordinateFrame;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage destCoordinateFrame;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetOrientation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetSize;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage targetVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new foreign state rule. */
	explicit aeRuleForeignState(aeWindowMain &windowMain, const char *name);
	/** Create a copy of a foreign state rule. */
	aeRuleForeignState(aeWindowMain &windowMain, const aeRuleForeignState &copy);
	/** Clean up the foreign state rule. */
protected:
	~aeRuleForeignState() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the foreign bone. */
	inline const decString &GetForeignBone() const{ return foreignBone; }
	
	/** Set the name of the foreign bone. */
	void SetForeignBone(const char *boneName);
	
	/** Retrieve the name of the foreign vertex position set. */
	inline const decString &GetForeignVertexPositionSet() const{ return foreignVertexPositionSet; }
	
	/** Set the name of the foreign vertex position set. */
	void SetForeignVertexPositionSet(const char *value);
	
	/** Retrieve the position scale factor. */
	inline float GetScalePosition() const{ return scalePosition; }
	/** Set the position scale factor. */
	void SetScalePosition(float value);
	/** Retrieve the orientation scale factor. */
	inline float GetScaleOrientation() const{ return scaleOrientation; }
	/** Set the orientation scale factor. */
	void SetScaleOrientation(float value);
	
	/** Retrieve the size scale factor. */
	inline float GetScaleSize() const{ return scaleSize; }
	
	/** Set the size scale factor. */
	void SetScaleSize(float value);
	
	/** Vertex position set scale factor. */
	inline float GetScaleVertexPositionSet() const{ return scaleVertexPositionSet; }
	
	/** Set vertex position set scale factor. */
	void SetScaleVertexPositionSet(float scale);
	
	/** Retrieve the source coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetSourceCoordinateFrame() const{ return sourceCoordinateFrame; }
	/** Set the source coordinate frame. */
	void SetSourceCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame);
	/** Retrieve the destination coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetDestCoordinateFrame() const{ return destCoordinateFrame; }
	/** Set the destination coordinate frame. */
	void SetDestCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame);
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return enablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool value);
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return enableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool value);
	
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return enableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool value);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return enableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool value);
	
	/** Retrieve the scale position target. */
	inline const aeControllerTarget::Ref &GetTargetPosition() const{ return pTargetPosition; }
	
	/** Retrieve the scale orientation target. */
	inline const aeControllerTarget::Ref &GetTargetOrientation() const{ return pTargetOrientation; }
	
	/** Retrieve the scale size target. */
	inline const aeControllerTarget::Ref &GetTargetSize() const{ return pTargetSize; }
	
	/** Vertex position set size target. */
	inline const aeControllerTarget::Ref &GetTargetVertexPositionSet() const{ return pTargetVertexPositionSet; }
	
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
	/** Copy another foreign state rule to this foreign state rule. */
	virtual aeRuleForeignState &operator=(const aeRuleForeignState &copy);
	/*@}*/
};

#endif
