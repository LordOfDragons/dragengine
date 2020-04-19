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

#ifndef _AERULEANIMATION_H_
#define _AERULEANIMATION_H_

#include "aeRule.h"



/**
 * \brief Animator rule animation.
 */
class aeRuleAnimation : public aeRule{
private:
	decString pMoveName;
	float pMoveTime;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	aeControllerTarget pTargetMoveTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new animator rule. */
	aeRuleAnimation();
	/** \brief Create a copy of a animator rule. */
	aeRuleAnimation( const aeRuleAnimation &copy );
	/** \brief Clean up the animator rule. */
	virtual ~aeRuleAnimation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the name of the animation move. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	/** \brief Set the animation move name. */
	void SetMoveName( const char *moveName );
	/** \brief Retrieve the animation move time. */
	inline float GetMoveTime() const{ return pMoveTime; }
	/** \brief Set the animation move time. */
	void SetMoveTime( float moveTime );
	
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
	
	/** \brief Retrieve the move time target. */
	inline aeControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	inline const aeControllerTarget &GetTargetMoveTime() const{ return pTargetMoveTime; }
	
	/** \brief Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	/** \brief Update targets. */
	virtual void UpdateTargets();
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	/** \brief Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another animation rule to this rule. */
	virtual aeRuleAnimation &operator=( const aeRuleAnimation &copy );
	/*@}*/
};

#endif
