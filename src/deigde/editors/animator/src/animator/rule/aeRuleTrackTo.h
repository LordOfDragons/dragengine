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

#ifndef _AERULETRACKTO_H_
#define _AERULETRACKTO_H_

#include "aeRule.h"
#include <dragengine/resources/animator/rule/deAnimatorRuleTrackTo.h>



/**
 * \brief Animator rule track to.
 */
class aeRuleTrackTo : public aeRule{
private:
	decString pTrackBone;
	deAnimatorRuleTrackTo::eTrackAxis pTrackAxis;
	deAnimatorRuleTrackTo::eTrackAxis pUpAxis;
	deAnimatorRuleTrackTo::eUpTarget pUpTarget;
	deAnimatorRuleTrackTo::eLockedAxis pLockedAxis;
	
	aeControllerTarget pTargetPosition;
	aeControllerTarget pTargetUp;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new track to rule. */
	aeRuleTrackTo();
	/** \brief Create a copy of a track to rule. */
	aeRuleTrackTo( const aeRuleTrackTo &copy );
	/** \brief Clean up the animator rule. */
	virtual ~aeRuleTrackTo();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the name of the track bone or empty string to use none. */
	inline const decString &GetTrackBone() const{ return pTrackBone; }
	/** \brief Set the name of the track bone or empty string to use none. */
	void SetTrackBone( const char *boneName );
	/** \brief Retrieve the track axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetTrackAxis() const{ return pTrackAxis; }
	/** \brief Set the track axis. */
	void SetTrackAxis( deAnimatorRuleTrackTo::eTrackAxis axis );
	/** \brief Retrieve the up axis. */
	inline deAnimatorRuleTrackTo::eTrackAxis GetUpAxis() const{ return pUpAxis; }
	/** \brief Set the up axis. */
	void SetUpAxis( deAnimatorRuleTrackTo::eTrackAxis axis );
	/** \brief Retrieve the up target. */
	inline deAnimatorRuleTrackTo::eUpTarget GetUpTarget() const{ return pUpTarget; }
	/** \brief Set the up target. */
	void SetUpTarget( deAnimatorRuleTrackTo::eUpTarget target );
	/** \brief Retrieve the locked axis. */
	inline deAnimatorRuleTrackTo::eLockedAxis GetLockedAxis() const{ return pLockedAxis; }
	/** \brief Set the locked axis. */
	void SetLockedAxis( deAnimatorRuleTrackTo::eLockedAxis axis );
	
	/** \brief Retrieve the position target. */
	inline aeControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	/** \brief Retrieve the up target. */
	inline aeControllerTarget &GetTargetUp(){ return pTargetUp; }
	
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
	/** \brief Copy another track to rule to this track to rule. */
	virtual aeRuleTrackTo &operator=( const aeRuleTrackTo &copy );
	/*@}*/
};

#endif
