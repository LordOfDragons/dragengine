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

#ifndef _AERULEANIMATIONSELECT_H_
#define _AERULEANIMATIONSELECT_H_

#include "aeRule.h"

#include <dragengine/common/string/decStringList.h>



/**
 * \brief Animator rule animation select.
 */
class aeRuleAnimationSelect : public aeRule{
private:
	decStringList pMoves;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	aeControllerTarget pTargetMoveTime;
	aeControllerTarget pTargetSelect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator select rule. */
	aeRuleAnimationSelect();
	
	/** \brief Create copy of animator select rule. */
	aeRuleAnimationSelect( const aeRuleAnimationSelect &copy );
	
	/** \brief Clean up animator select rule. */
	virtual ~aeRuleAnimationSelect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Moves. */
	inline const decStringList &GetMoves() const{ return pMoves; }
	
	/** \brief Set moves. */
	void SetMoves( const decStringList &moves );
	
	
	
	/** \brief Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	
	
	/** \brief Move time target. */
	inline aeControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	
	/** \brief Select target. */
	inline aeControllerTarget &GetTargetSelect(){ return pTargetSelect; }
	
	
	
	/** \brief Create engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	
	/** \brief Remove link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	
	
	/** \brief Create copy of rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another animator difference rule to this animator difference rule. */
	virtual aeRuleAnimationSelect &operator=( const aeRuleAnimationSelect &copy );
	/*@}*/
};

#endif
