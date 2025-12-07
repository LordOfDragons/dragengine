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

#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>



/**
 * Foreign state rule.
 */
class aeRuleForeignState : public aeRule{
public:
	typedef deTObjectReference<aeRuleForeignState> Ref;
	
	
private:
	decString pForeignBone;
	decString pForeignVertexPositionSet;
	deAnimatorRuleForeignState::eCoordinateFrames pSourceCoordinateFrame;
	deAnimatorRuleForeignState::eCoordinateFrames pDestCoordinateFrame;
	float pScalePosition;
	float pScaleOrientation;
	float pScaleSize;
	float pScaleVertexPositionSet;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	aeControllerTarget pTargetPosition;
	aeControllerTarget pTargetOrientation;
	aeControllerTarget pTargetSize;
	aeControllerTarget pTargetVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new foreign state rule. */
	aeRuleForeignState();
	/** Create a copy of a foreign state rule. */
	aeRuleForeignState(const aeRuleForeignState &copy);
	/** Clean up the foreign state rule. */
	virtual ~aeRuleForeignState();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the name of the foreign bone. */
	inline const decString &GetForeignBone() const{ return pForeignBone; }
	
	/** Set the name of the foreign bone. */
	void SetForeignBone(const char *boneName);
	
	/** Retrieve the name of the foreign vertex position set. */
	inline const decString &GetForeignVertexPositionSet() const{ return pForeignVertexPositionSet; }
	
	/** Set the name of the foreign vertex position set. */
	void SetForeignVertexPositionSet(const char *vertexPositionSet);
	
	/** Retrieve the position scale factor. */
	inline float GetScalePosition() const{ return pScalePosition; }
	/** Set the position scale factor. */
	void SetScalePosition(float scalePosition);
	/** Retrieve the orientation scale factor. */
	inline float GetScaleOrientation() const{ return pScaleOrientation; }
	/** Set the orientation scale factor. */
	void SetScaleOrientation(float scaleOrientation);
	
	/** Retrieve the size scale factor. */
	inline float GetScaleSize() const{ return pScaleSize; }
	
	/** Set the size scale factor. */
	void SetScaleSize(float scaleSize);
	
	/** Vertex position set scale factor. */
	inline float GetScaleVertexPositionSet() const{ return pScaleVertexPositionSet; }
	
	/** Set vertex position set scale factor. */
	void SetScaleVertexPositionSet(float scale);
	
	/** Retrieve the source coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetSourceCoordinateFrame() const{ return pSourceCoordinateFrame; }
	/** Set the source coordinate frame. */
	void SetSourceCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame);
	/** Retrieve the destination coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetDestCoordinateFrame() const{ return pDestCoordinateFrame; }
	/** Set the destination coordinate frame. */
	void SetDestCoordinateFrame(deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame);
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	/** Retrieve the scale position target. */
	inline aeControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	inline const aeControllerTarget &GetTargetPosition() const{ return pTargetPosition; }
	
	/** Retrieve the scale orientation target. */
	inline aeControllerTarget &GetTargetOrientation(){ return pTargetOrientation; }
	inline const aeControllerTarget &GetTargetOrientation() const{ return pTargetOrientation; }
	
	/** Retrieve the scale size target. */
	inline aeControllerTarget &GetTargetSize(){ return pTargetSize; }
	inline const aeControllerTarget &GetTargetSize() const{ return pTargetSize; }
	
	/** Vertex position set size target. */
	inline aeControllerTarget &GetTargetVertexPositionSet(){ return pTargetVertexPositionSet; }
	inline const aeControllerTarget &GetTargetVertexPositionSet() const{ return pTargetVertexPositionSet; }
	
	/** Creates an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage(aeLink *link) const;
	/** Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets(aeLink *link);
	/** Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks(aeLinkList& list);
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another foreign state rule to this foreign state rule. */
	virtual aeRuleForeignState &operator=(const aeRuleForeignState &copy);
	/*@}*/
};

#endif
