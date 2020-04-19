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
 * \brief Animator rule bone transformator.
 */
class aeRuleBoneTransformator : public aeRule{
private:
	decVector pMinTranslation;
	decVector pMaxTranslation;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinScaling;
	decVector pMaxScaling;
	
	deAnimatorRuleBoneTransformator::eCoordinateFrames pCoordinateFrame;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	decString pTargetBone;
	
	aeControllerTarget pTargetTranslation;
	aeControllerTarget pTargetRotation;
	aeControllerTarget pTargetScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new bone transformator rule. */
	aeRuleBoneTransformator();
	/** \brief Create a copy of a bone transformator rule. */
	aeRuleBoneTransformator( const aeRuleBoneTransformator &copy );
	/** \brief Clean up the bone transformator rule. */
	virtual ~aeRuleBoneTransformator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the minimum translation. */
	inline const decVector &GetMinimumTranslation() const{ return pMinTranslation; }
	/** \brief Set the minimum translation. */
	void SetMinimumTranslation( const decVector &translation );
	/** \brief Retrieve the maximum translation. */
	inline const decVector &GetMaximumTranslation() const{ return pMaxTranslation; }
	/** \brief Set the maximum translation. */
	void SetMaximumTranslation( const decVector &translation );
	/** \brief Retrieve the minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	/** \brief Set the minimum rotation. */
	void SetMinimumRotation( const decVector &rotation );
	/** \brief Retrieve the maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	/** \brief Set the maximum rotation. */
	void SetMaximumRotation( const decVector &rotation );
	/** \brief Retrieve the minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	/** \brief Set the minimum scaling. */
	void SetMinimumScaling( const decVector &scaling );
	/** \brief Retrieve the maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	/** \brief Set the maximum scaling. */
	void SetMaximumScaling( const decVector &scaling );
	
	/** \brief Retrieve the coordinate frame to rotate around. */
	inline deAnimatorRuleBoneTransformator::eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	/** \brief Set the coordinate frame to rotate around. */
	void SetCoordinateFrame( deAnimatorRuleBoneTransformator::eCoordinateFrames coordinateFrame );
	
	/** \brief Determine if position is altered. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** \brief Set if position is altered. */
	void SetEnablePosition( bool enabled );
	/** \brief Determine if orientation is altered. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** \brief Set if orientation is altered. */
	void SetEnableOrientation( bool enabled );
	/** \brief Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Retrieve the name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	/** \brief Set the name of the target bone. */
	void SetTargetBone( const char *boneName );
	
	/** \brief Retrieve the translation target. */
	inline aeControllerTarget &GetTargetTranslation(){ return pTargetTranslation; }
	/** \brief Retrieve the rotation target. */
	inline aeControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	/** \brief Retrieve the scaling target. */
	inline aeControllerTarget &GetTargetScaling(){ return pTargetScaling; }
	
	/** \brief Creates an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** \brief Update targets. */
	virtual void UpdateTargets();
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another animation rule to this rule. */
	virtual aeRuleBoneTransformator &operator=( const aeRuleBoneTransformator &copy );
	/*@}*/
};

#endif
