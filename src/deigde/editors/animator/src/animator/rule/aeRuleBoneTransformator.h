/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AERULEBONETRANSFORMATOR_H_
#define _AERULEBONETRANSFORMATOR_H_

#include <dragengine/resources/animator/rule/deAnimatorRuleBoneTransformator.h>

#include "aeRule.h"



/**
 * Animator rule bone transformator.
 */
class aeRuleBoneTransformator : public aeRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<aeRuleBoneTransformator> Ref;
	
	
	
private:
	decVector pMinTranslation;
	decVector pMaxTranslation;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinScaling;
	decVector pMaxScaling;
	decVector pAxis;
	float pMinAngle;
	float pMaxAngle;
	
	deAnimatorRuleBoneTransformator::eCoordinateFrames pCoordinateFrame;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pUseAxis;
	
	decString pTargetBone;
	
	aeControllerTarget pTargetTranslation;
	aeControllerTarget pTargetRotation;
	aeControllerTarget pTargetScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new bone transformator rule. */
	aeRuleBoneTransformator();
	/** Create a copy of a bone transformator rule. */
	aeRuleBoneTransformator( const aeRuleBoneTransformator &copy );
	/** Clean up the bone transformator rule. */
	virtual ~aeRuleBoneTransformator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the minimum translation. */
	inline const decVector &GetMinimumTranslation() const{ return pMinTranslation; }
	/** Set the minimum translation. */
	void SetMinimumTranslation( const decVector &translation );
	/** Retrieve the maximum translation. */
	inline const decVector &GetMaximumTranslation() const{ return pMaxTranslation; }
	/** Set the maximum translation. */
	void SetMaximumTranslation( const decVector &translation );
	/** Retrieve the minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	/** Set the minimum rotation. */
	void SetMinimumRotation( const decVector &rotation );
	/** Retrieve the maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	/** Set the maximum rotation. */
	void SetMaximumRotation( const decVector &rotation );
	/** Retrieve the minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	/** Set the minimum scaling. */
	void SetMinimumScaling( const decVector &scaling );
	/** Retrieve the maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	/** Set the maximum scaling. */
	void SetMaximumScaling( const decVector &scaling );
	
	/** Rotation axis. */
	inline const decVector &GetAxis() const{ return pAxis; }
	
	/** Set rotation axis. */
	void SetAxis( const decVector &axis );
	
	/** Minimum axis rotation angle. */
	inline float GetMinimumAngle() const{ return pMinAngle; }
	
	/** Set minimum axis rotation angle. */
	void SetMinimumAngle( float angle );
	
	/** Maximum axis rotation angle. */
	inline float GetMaximumAngle() const{ return pMaxAngle; }
	
	/** Set maximum axis rotation angle. */
	void SetMaximumAngle( float angle );
	
	/** Retrieve the coordinate frame to rotate around. */
	inline deAnimatorRuleBoneTransformator::eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	/** Set the coordinate frame to rotate around. */
	void SetCoordinateFrame( deAnimatorRuleBoneTransformator::eCoordinateFrames coordinateFrame );
	
	/** Determine if position is altered. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** Set if position is altered. */
	void SetEnablePosition( bool enabled );
	/** Determine if orientation is altered. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** Set if orientation is altered. */
	void SetEnableOrientation( bool enabled );
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** Use rotation axis instead of rotation directly. */
	inline bool GetUseAxis() const{ return pUseAxis; }
	
	/** Set to use rotation axis instead of rotation directly. */
	void SetUseAxis( bool useAxis );
	
	/** Retrieve the name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	/** Set the name of the target bone. */
	void SetTargetBone( const char *boneName );
	
	/** Retrieve the translation target. */
	inline aeControllerTarget &GetTargetTranslation(){ return pTargetTranslation; }
	/** Retrieve the rotation target. */
	inline aeControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	/** Retrieve the scaling target. */
	inline aeControllerTarget &GetTargetScaling(){ return pTargetScaling; }
	
	/** Creates an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** Update targets. */
	virtual void UpdateTargets();
	/** Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another animation rule to this rule. */
	virtual aeRuleBoneTransformator &operator=( const aeRuleBoneTransformator &copy );
	/*@}*/
};

#endif
