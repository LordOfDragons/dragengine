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

#ifndef _AEWINDOWMAINLISTENER_H_
#define _AEWINDOWMAINLISTENER_H_

#include "../animator/aeAnimatorNotifier.h"

class aeWindowMain;



/**
 * \brief Main window listener.
 */
class aeWindowMainListener : public aeAnimatorNotifier{
private:
	aeWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	aeWindowMainListener( aeWindowMain &windowMain );
	
	/** \brief Clean up listener. */
	virtual ~aeWindowMainListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Changed or saved state changed. */
	virtual void StateChanged( aeAnimator *animator );
	
	/** \brief Undos changed. */
	virtual void UndoChanged( aeAnimator *animator );
	
	/** \brief Animator changed. */
	virtual void AnimatorChanged( aeAnimator *animator );
	
	/** \brief View changed. */
	virtual void ViewChanged( aeAnimator *animator );
	
	/** \brief Model changed. */
	virtual void ModelChanged( aeAnimator *animator );
	
	/** \brief Animation changed. */
	virtual void AnimationChanged( aeAnimator *animator );
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( aeAnimator *animator, aeController *controller );
	
	/** \brief Controller count or order changed. */
	virtual void ControllerStructureChanged( aeAnimator *animator );
	
	/** \brief Active rule changed. */
	virtual void ActiveRuleChanged( aeAnimator *animator, aeRule *rule );
	
	/** \brief Rule changed. */
	virtual void RuleChanged( aeAnimator *animator, aeRule *rule );
	
	/** \brief Rule name changed. */
	virtual void RuleNameChanged( aeAnimator *animator, aeRule *rule );
	
	/** \brief Rule count or order changed. */
	virtual void RuleStructureChanged( aeAnimator *animator );
	/*@}*/
};

#endif
