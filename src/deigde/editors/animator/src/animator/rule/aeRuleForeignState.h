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

#ifndef _AERULEFOREIGNSTATE_H_
#define _AERULEFOREIGNSTATE_H_

#include "aeRule.h"

#include <dragengine/resources/animator/rule/deAnimatorRuleForeignState.h>



/**
 * \brief Foreign state rule.
 */
class aeRuleForeignState : public aeRule{
public:
	typedef deTObjectReference<aeRuleForeignState> Ref;
	
private:
	decString pForeignBone;
	deAnimatorRuleForeignState::eCoordinateFrames pSourceCoordinateFrame;
	deAnimatorRuleForeignState::eCoordinateFrames pDestCoordinateFrame;
	float pScalePosition;
	float pScaleOrientation;
	float pScaleSize;
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	aeControllerTarget pTargetPosition;
	aeControllerTarget pTargetOrientation;
	aeControllerTarget pTargetSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new foreign state rule. */
	aeRuleForeignState();
	/** \brief Create a copy of a foreign state rule. */
	aeRuleForeignState( const aeRuleForeignState &copy );
	/** \brief Clean up the foreign state rule. */
	virtual ~aeRuleForeignState();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the name of the foreign bone. */
	inline const decString &GetForeignBone() const{ return pForeignBone; }
	/** \brief Set the name of the foreign bone. */
	void SetForeignBone( const char *boneName );
	
	/** \brief Retrieve the position scale factor. */
	inline float GetScalePosition() const{ return pScalePosition; }
	/** \brief Set the position scale factor. */
	void SetScalePosition( float scalePosition );
	/** \brief Retrieve the orientation scale factor. */
	inline float GetScaleOrientation() const{ return pScaleOrientation; }
	/** \brief Set the orientation scale factor. */
	void SetScaleOrientation( float scaleOrientation );
	/** \brief Retrieve the size scale factor. */
	inline float GetScaleSize() const{ return pScaleSize; }
	/** \brief Set the size scale factor. */
	void SetScaleSize( float scaleSize );
	/** \brief Retrieve the source coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetSourceCoordinateFrame() const{ return pSourceCoordinateFrame; }
	/** \brief Set the source coordinate frame. */
	void SetSourceCoordinateFrame( deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame );
	/** \brief Retrieve the destination coordinate frame. */
	inline deAnimatorRuleForeignState::eCoordinateFrames GetDestCoordinateFrame() const{ return pDestCoordinateFrame; }
	/** \brief Set the destination coordinate frame. */
	void SetDestCoordinateFrame( deAnimatorRuleForeignState::eCoordinateFrames coordinateFrame );
	
	/** \brief Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** \brief Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	/** \brief Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	/** \brief Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	/** \brief Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Retrieve the scale position target. */
	inline aeControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	inline const aeControllerTarget &GetTargetPosition() const{ return pTargetPosition; }
	
	/** \brief Retrieve the scale orientation target. */
	inline aeControllerTarget &GetTargetOrientation(){ return pTargetOrientation; }
	inline const aeControllerTarget &GetTargetOrientation() const{ return pTargetOrientation; }
	
	/** \brief Retrieve the scale size target. */
	inline aeControllerTarget &GetTargetSize(){ return pTargetSize; }
	inline const aeControllerTarget &GetTargetSize() const{ return pTargetSize; }
	
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
	/** \brief Copy another foreign state rule to this foreign state rule. */
	virtual aeRuleForeignState &operator=( const aeRuleForeignState &copy );
	/*@}*/
};

#endif
