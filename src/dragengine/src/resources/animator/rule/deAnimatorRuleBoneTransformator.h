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

#ifndef _DEANIMATORRULEBONETRANSFORMATOR_H_
#define _DEANIMATORRULEBONETRANSFORMATOR_H_

#include "deAnimatorRule.h"
#include "../../../common/math/decMath.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Bone Transformator Rule Class.
 * 
 * Applies a rotation to the current state of the stored bones.
 * The rotation is considered relative to the component. Often
 * it is useful to use the position of the respective bone as
 * the reference point for applying the rotation. The bone-local
 * parameter indicates this behaviour. As this is the often looked
 * for behavior it is set true by default. The rotation axes are
 * always parallel to the component axes no matter if bone-local
 * rotation is used. If a controller is specified it is used to
 * modulate the rotation from a minimum to a maximum rotation.
 */
class deAnimatorRuleBoneTransformator : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleBoneTransformator> Ref;
	
	
	
public:
	/** \brief Coordinate frame. */
	enum eCoordinateFrames{
		/** \brief Bone local coordinate frame. */
		ecfBoneLocal,
		
		/** \brief Component coordinate frame. */
		ecfComponent,
		
		/** \brief Target bone coordinate frame. */
		ecfTargetBone
	};
	
	
	
private:
	decVector pMinTranslation;
	decVector pMaxTranslation;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinScaling;
	decVector pMaxScaling;
	
	eCoordinateFrames pCoordinateFrame;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	decString pTargetBone;
	
	deAnimatorControllerTarget pTargetTranslation;
	deAnimatorControllerTarget pTargetRotation;
	deAnimatorControllerTarget pTargetScaling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleBoneTransformator();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleBoneTransformator();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Minimum translation. */
	inline const decVector &GetMinimumTranslation() const{ return pMinTranslation; }
	
	/** \brief Set minimum translation. */
	void SetMinimumTranslation( const decVector &translation );
	
	/** \brief Maximum translation. */
	inline const decVector &GetMaximumTranslation() const{ return pMaxTranslation; }
	
	/** \brief Set maximum translation. */
	void SetMaximumTranslation( const decVector &translation );
	
	/** \brief Minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	
	/** \brief Set minimum rotation. */
	void SetMinimumRotation( const decVector &rotation );
	
	/** \brief Maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	
	/** \brief Set maximum rotation. */
	void SetMaximumRotation( const decVector &rotation );
	
	/** \brief Minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	
	/** \brief Set minimum scaling. */
	void SetMinimumScaling( const decVector &scaling );
	
	/** \brief Maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	
	/** \brief Set maximum scaling. */
	void SetMaximumScaling( const decVector &scaling );
	
	/** \brief Transformation coordinate frame. */
	inline eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	
	/** \brief Set transformation coordinate frame. */
	void SetCoordinateFrame( eCoordinateFrames coordinateFrame );
	
	/** \brief Determines if position is altered. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position is altered. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if orientation is altered. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation is altered. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	
	/** \brief Set name of the target bone. */
	void SetTargetBone( const char *boneName );
	
	/** \brief Translation target. */
	inline deAnimatorControllerTarget &GetTargetTranslation(){ return pTargetTranslation; }
	inline const deAnimatorControllerTarget &GetTargetTranslation() const{ return pTargetTranslation; }
	
	/** \brief Rotation target. */
	inline deAnimatorControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	inline const deAnimatorControllerTarget &GetTargetRotation() const{ return pTargetRotation; }
	
	/** \brief Scaling target. */
	inline deAnimatorControllerTarget &GetTargetScaling(){ return pTargetScaling; }
	inline const deAnimatorControllerTarget &GetTargetScaling() const{ return pTargetScaling; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
