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

#ifndef _AERULESUBANIMATOR_H_
#define _AERULESUBANIMATOR_H_

#include "aeRule.h"

#include <dragengine/common/collection/decObjectList.h>

class aeController;
class aeLoadSaveSystem;

class deAnimator;
class deAnimatorRuleSubAnimator;



/**
 * \brief Animator rule sub animator.
 */
class aeRuleSubAnimator : public aeRule{
private:
	decString pPathSubAnimator;
	deAnimator *pSubAnimator;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	decObjectList pConnections;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new sub animator rule. */
	aeRuleSubAnimator();
	/** \brief Create a copy of a sub animator rule. */
	aeRuleSubAnimator( const aeRuleSubAnimator &copy );
	/** \brief Clean up the sub animator rule. */
	virtual ~aeRuleSubAnimator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the path to the sub animator. */
	inline const decString &GetPathSubAnimator() const{ return pPathSubAnimator; }
	/** \brief Set the path to the sub animator. */
	void SetPathSubAnimator( const char *path );
	/** \brief Retrieve the sub animator or NULL if not existing. */
	inline deAnimator *GetSubAnimator() const{ return pSubAnimator; }
	/** \brief Load the sub animator using the stored path. */
	void LoadSubAnimator();
	
	/** \brief Number of connections. */
	int GetConnectionCount() const;
	
	/** \brief Controller for target controller or \em NULL. */
	aeController *GetControllerAt( int position ) const;
	
	/** \brief Set controller for target controller or \em NULL. */
	void SetControllerAt( int position, aeController *controller );
	
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
	
	/** \brief Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	/** \brief Update Component and Animation. */
	virtual void UpdateCompAnim();
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	
	/** \brief Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another sub animator rule to this sub animator rule. */
	virtual aeRuleSubAnimator &operator=( const aeRuleSubAnimator &copy );
	/*@}*/
	
private:
	void pUpdateConnections( deAnimatorRuleSubAnimator &rule ) const;
};

#endif
