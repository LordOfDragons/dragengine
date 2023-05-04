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

#ifndef _AEWPRULELISTENER_H_
#define _AEWPRULELISTENER_H_

#include "../../animator/aeAnimatorNotifier.h"

class aeWPRule;



/**
 * Rule panel listener.
 */
class aeWPRuleListener : public aeAnimatorNotifier{
private:
	aeWPRule &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	aeWPRuleListener( aeWPRule &panel );
	
	/** Clean up listener. */
	virtual ~aeWPRuleListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Rig changed. */
	virtual void RigChanged( aeAnimator *animator );
	
	/** Animation changed. */
	virtual void AnimationChanged( aeAnimator *animator );
	
	/** Controller named changed. */
	virtual void ControllerNameChanged( aeAnimator *animator, aeController *controller );
	
	/** Controller count or order changed. */
	virtual void ControllerStructureChanged( aeAnimator *animator );
	
	/** Link name changed. */
	virtual void LinkNameChanged( aeAnimator *animator, aeLink *link );
	
	/** Link count or order changed. */
	virtual void LinkStructureChanged( aeAnimator *animator );
	
	/** Active rule changed. */
	virtual void ActiveRuleChanged( aeAnimator *animator, aeRule *rule );
	
	/** Rule changed. */
	virtual void RuleChanged( aeAnimator *animator, aeRule *rule );
	
	/** Rule name changed. */
	virtual void RuleNameChanged( aeAnimator *animator, aeRule *rule );
	
	/** Rule count or order changed. */
	virtual void RuleStructureChanged( aeAnimator *animator );
	/*@}*/
};

#endif
