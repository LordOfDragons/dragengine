/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEANIMATORRULEFOREIGNSTATE_H_
#define _DEANIMATORRULEFOREIGNSTATE_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Foreign State Rule Class.
 *
 * The foreign state rule applies the state of another bone to the affected
 * bones ( usually one ) after applying a scaling factor. This way bones can
 * be animated in dependence of other bones. This is useful for character
 * attachments where animation data does not exists for new bones added by
 * the attachment yet the right bone state can be calculated from existing
 * animation bones. An example would be a shoulder pad which is oriented
 * half way between the two arm/shoulder bones in proximity of the pad bone.
 * For each scaling factor a controller link can be provided.
 */
class deAnimatorRuleForeignState : public deAnimatorRule{
public:
	/** \brief Coordinate frame. */
	enum eCoordinateFrames{
		/** \brief Bone local coordinate frame. */
		ecfBoneLocal,
		
		/** \brief Component coordinate frame. */
		ecfComponent,
	};
	
	
	
private:
	decString pForeignBone;
	eCoordinateFrames pSourceCoordinateFrame;
	eCoordinateFrames pDestCoordinateFrame;
	float pScalePosition;
	float pScaleOrientation;
	float pScaleSize;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	deAnimatorControllerTarget pTargetPosition;
	deAnimatorControllerTarget pTargetOrientation;
	deAnimatorControllerTarget pTargetSize;
	
	
	
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
	
	/** \brief Source coordinate frame. */
	inline eCoordinateFrames GetSourceCoordinateFrame() const{ return pSourceCoordinateFrame; }
	
	/** \brief Set source coordinate frame. */
	void SetSourceCoordinateFrame( eCoordinateFrames coordinateFrame );
	
	/** \brief Destination coordinate frame. */
	inline eCoordinateFrames GetDestCoordinateFrame() const{ return pDestCoordinateFrame; }
	
	/** \brief Set destination coordinate frame. */
	void SetDestCoordinateFrame( eCoordinateFrames coordinateFrame );
	
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
	
	/** \brief Scale position target. */
	inline deAnimatorControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	inline const deAnimatorControllerTarget &GetTargetPosition() const{ return pTargetPosition; }
	
	/** \brief Scale orientation target. */
	inline deAnimatorControllerTarget &GetTargetOrientation(){ return pTargetOrientation; }
	inline const deAnimatorControllerTarget &GetTargetOrientation() const{ return pTargetOrientation; }
	
	/** \brief Scale size target. */
	inline deAnimatorControllerTarget &GetTargetSize(){ return pTargetSize; }
	inline const deAnimatorControllerTarget &GetTargetSize() const{ return pTargetSize; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
