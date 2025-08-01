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

#ifndef _DEANIMATORRULEFOREIGNSTATE_H_
#define _DEANIMATORRULEFOREIGNSTATE_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Foreign State Rule Class.
 *
 * The foreign state rule applies the state of another bone to the affected
 * bones (usually one) after applying a scaling factor. This way bones can
 * be animated in dependence of other bones. This is useful for character
 * attachments where animation data does not exists for new bones added by
 * the attachment yet the right bone state can be calculated from existing
 * animation bones. An example would be a shoulder pad which is oriented
 * half way between the two arm/shoulder bones in proximity of the pad bone.
 * For each scaling factor a controller link can be provided.
 */
class DE_DLL_EXPORT deAnimatorRuleForeignState : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleForeignState> Ref;
	
	
	
public:
	/** \brief Coordinate frame. */
	enum eCoordinateFrames{
		/** \brief Bone local coordinate frame. */
		ecfBoneLocal,
		
		/** \brief Component coordinate frame. */
		ecfComponent,
	};
	
	
	
private:
	decString pForeignBone, pForeignVertexPositionSet;
	eCoordinateFrames pSourceCoordinateFrame, pDestCoordinateFrame;
	float pScalePosition, pScaleOrientation, pScaleSize, pScaleVertexPositionSet;
	bool pModifyX, pModifyY, pModifyZ;
	bool pEnablePosition, pEnableOrientation, pEnableSize, pEnableVertexPositionSet;
	
	deAnimatorControllerTarget pTargetPosition, pTargetOrientation, pTargetSize;
	deAnimatorControllerTarget pTargetVertexPositionSets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleForeignState();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleForeignState();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the foreign bone. */
	inline const decString &GetForeignBone() const{ return pForeignBone; }
	
	/** \brief Set name of the foreign bone. */
	void SetForeignBone( const char *boneName );
	
	/** \brief Name of the foreign vertex position set. */
	inline const decString &GetForeignVertexPositionSet() const{ return pForeignVertexPositionSet; }
	
	/** \brief Set name of the foreign vertex position set. */
	void SetForeignVertexPositionSet( const char *vertexPositionSet );
	
	/** \brief Position scale factor. */
	inline float GetScalePosition() const{ return pScalePosition; }
	
	/** \brief Set position scale factor. */
	void SetScalePosition( float scalePosition );
	
	/** \brief Orientation scale factor. */
	inline float GetScaleOrientation() const{ return pScaleOrientation; }
	
	/** \brief Set orientation scale factor. */
	void SetScaleOrientation( float scaleOrientation );
	
	/** \brief Size scale factor. */
	inline float GetScaleSize() const{ return pScaleSize; }
	
	/** \brief Set size scale factor. */
	void SetScaleSize( float scaleSize );
	
	/** \brief Vertex position set scale factor. */
	inline float GetScaleVertexPositionSet() const{ return pScaleVertexPositionSet; }
	
	/** \brief Set vertex position set scale factor. */
	void SetScaleVertexPositionSet( float scale );
	
	/** \brief Source coordinate frame. */
	inline eCoordinateFrames GetSourceCoordinateFrame() const{ return pSourceCoordinateFrame; }
	
	/** \brief Set source coordinate frame. */
	void SetSourceCoordinateFrame( eCoordinateFrames coordinateFrame );
	
	/** \brief Destination coordinate frame. */
	inline eCoordinateFrames GetDestCoordinateFrame() const{ return pDestCoordinateFrame; }
	
	/** \brief Set destination coordinate frame. */
	void SetDestCoordinateFrame( eCoordinateFrames coordinateFrame );
	
	/**
	 * \brief Determines if X component is modified.
	 * \version 1.6
	 */
	inline bool GetModifyX() const{ return pModifyX; }
	
	/**
	 * \brief Set if X component is modified.
	 * \version 1.6
	 */
	void SetModifyX( bool modify );
	
	/**
	 * \brief Determines if Y component is modified.
	 * \version 1.6
	 */
	inline bool GetModifyY() const{ return pModifyY; }
	
	/**
	 * \brief Set if Y component is modified.
	 * \version 1.6
	 */
	void SetModifyY( bool modify );
	
	/**
	 * \brief Determines if Z component is modified.
	 * \version 1.6
	 */
	inline bool GetModifyZ() const{ return pModifyZ; }
	
	/**
	 * \brief Set if Z component is modified.
	 * \version 1.6
	 */
	void SetModifyZ( bool modify );
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** \brief Scale position target. */
	inline deAnimatorControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	inline const deAnimatorControllerTarget &GetTargetPosition() const{ return pTargetPosition; }
	
	/** \brief Scale orientation target. */
	inline deAnimatorControllerTarget &GetTargetOrientation(){ return pTargetOrientation; }
	inline const deAnimatorControllerTarget &GetTargetOrientation() const{ return pTargetOrientation; }
	
	/** \brief Scale size target. */
	inline deAnimatorControllerTarget &GetTargetSize(){ return pTargetSize; }
	inline const deAnimatorControllerTarget &GetTargetSize() const{ return pTargetSize; }
	
	/** \brief Scale vertex position set target. */
	inline deAnimatorControllerTarget &GetTargetVertexPositionSet(){ return pTargetVertexPositionSets; }
	inline const deAnimatorControllerTarget &GetTargetVertexPositionSet() const{ return pTargetVertexPositionSets; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
