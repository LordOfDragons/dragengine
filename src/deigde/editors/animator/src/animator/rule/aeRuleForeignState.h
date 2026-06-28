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
	
	igdeMetaPropertyStringStorage::Storage pMPForeignBone;
	igdeMetaPropertyStringStorage::Storage pMPForeignVertexPositionSet;
	igdeMetaPropertyFloatStorage::Storage pMPScalePosition;
	igdeMetaPropertyFloatStorage::Storage pMPScaleOrientation;
	igdeMetaPropertyFloatStorage::Storage pMPScaleSize;
	igdeMetaPropertyFloatStorage::Storage pMPScaleVertexPositionSet;
	igdeMetaPropertyBooleanStorage::Storage pMPEnablePosition;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableOrientation;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableSize;
	igdeMetaPropertyBooleanStorage::Storage pMPEnableVertexPositionSet;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage pMPSourceCoordinateFrame;
	igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage pMPDestCoordinateFrame;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetPosition;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetOrientation;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetSize;
	igdeMetaPropertyObjectSetStorage<aeLink>::Storage pMPTargetVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	aeRuleForeignState() = delete;
	aeRuleForeignState(const aeRuleForeignState&) = delete;
	
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
	inline igdeMetaPropertyStringStorage::Storage &GetMPForeignBone(){ return pMPForeignBone; }
	inline igdeMetaPropertyStringStorage::Storage &GetMPForeignVertexPositionSet(){ return pMPForeignVertexPositionSet; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPScalePosition(){ return pMPScalePosition; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPScaleOrientation(){ return pMPScaleOrientation; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPScaleSize(){ return pMPScaleSize; }
	inline igdeMetaPropertyFloatStorage::Storage &GetMPScaleVertexPositionSet(){ return pMPScaleVertexPositionSet; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnablePosition(){ return pMPEnablePosition; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableOrientation(){ return pMPEnableOrientation; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableSize(){ return pMPEnableSize; }
	inline igdeMetaPropertyBooleanStorage::Storage &GetMPEnableVertexPositionSet(){ return pMPEnableVertexPositionSet; }
	inline igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage &GetMPSourceCoordinateFrame(){ return pMPSourceCoordinateFrame; }
	inline igdeMetaPropertySelectionEnumStorage<deAnimatorRuleForeignState::eCoordinateFrames>::Storage &GetMPDestCoordinateFrame(){ return pMPDestCoordinateFrame; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetPosition(){ return pMPTargetPosition; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetOrientation(){ return pMPTargetOrientation; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetSize(){ return pMPTargetSize; }
	inline igdeMetaPropertyObjectSetStorage<aeLink>::Storage &GetMPTargetVertexPositionSet(){ return pMPTargetVertexPositionSet; }
	
	
	/** Retrieve the name of the foreign bone. */
	inline const decString &GetForeignBone() const{ return pMPForeignBone; }
	
	/** Set the name of the foreign bone. */
	void SetForeignBone(const char *boneName);
	
	/** Retrieve the name of the foreign vertex position set. */
	inline const decString &GetForeignVertexPositionSet() const{ return pMPForeignVertexPositionSet; }
	
	/** Set the name of the foreign vertex position set. */
	void SetForeignVertexPositionSet(const char *value);
	
	/** Retrieve the position scale factor. */
	inline float GetScalePosition() const{ return pMPScalePosition; }
	/** Set the position scale factor. */
	void SetScalePosition(float value);
	/** Retrieve the orientation scale factor. */
	inline float GetScaleOrientation() const{ return pMPScaleOrientation; }
	/** Set the orientation scale factor. */
	void SetScaleOrientation(float value);
	
	/** Retrieve the size scale factor. */
	inline float GetScaleSize() const{ return pMPScaleSize; }
	
	/** Set the size scale factor. */
	void SetScaleSize(float value);
	
	/** Vertex position set scale factor. */
	inline float GetScaleVertexPositionSet() const{ return pMPScaleVertexPositionSet; }
	
	/** Set vertex position set scale factor. */
	void SetScaleVertexPositionSet(float scale);
	
	/** Retrieve the source coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetSourceCoordinateFrame() const{ return pMPSourceCoordinateFrame; }
	/** Set the source coordinate frame. */
	void SetSourceCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame);
	/** Retrieve the destination coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetDestCoordinateFrame() const{ return pMPDestCoordinateFrame; }
	/** Set the destination coordinate frame. */
	void SetDestCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame);
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pMPEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool value);
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pMPEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool value);
	
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pMPEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool value);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pMPEnableVertexPositionSet; }
	
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
	aeRule::Ref CreateCopy(aeWindowMain &windowMain) const override;
	
	/** List all links of all rule targets. */
	void ListLinks(aeLink::List& list) override;
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	aeRuleForeignState &operator=(const aeRuleForeignState &copy) = delete;
	/*@}*/
};

#endif
