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

#ifndef _AEWPPLAYGROUNDLISTENER_H_
#define _AEWPPLAYGROUNDLISTENER_H_

#include "../../animator/aeAnimatorNotifier.h"

class aeWPPlayground;



/**
 * \brief Playground Panel Listener
 */
class aeWPPlaygroundListener : public aeAnimatorNotifier{
private:
	aeWPPlayground &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	aeWPPlaygroundListener( aeWPPlayground &panel );
	
	/** \brief Clean up listener. */
	virtual ~aeWPPlaygroundListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Locomotion changed. */
	virtual void LocomotionChanged( aeAnimator *animator );
	
	/** Controller changed. */
	virtual void ControllerChanged( aeAnimator *animator, aeController *controller );
	
	/** \brief Controller name changed. */
	virtual void ControllerNameChanged( aeAnimator *animator, aeController *controller );
	
	/** \brief Controller value changed. */
	virtual void ControllerValueChanged( aeAnimator *animator, aeController *controller );
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged( aeAnimator *animator );
	/*@}*/
};

#endif
