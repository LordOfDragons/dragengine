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

#ifndef _AEWPANIMATORLISTENER_H_
#define _AEWPANIMATORLISTENER_H_

#include "../../animator/aeAnimatorNotifier.h"

class aeWPAnimator;



/**
 * \brief Animator panel listener
 */
class aeWPAnimatorListener : public aeAnimatorNotifier{
private:
	aeWPAnimator &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	aeWPAnimatorListener( aeWPAnimator &panel );
	
	/** \brief Clean up listener. */
	virtual ~aeWPAnimatorListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Animator changed. */
	virtual void AnimatorChanged( aeAnimator *animator );
	
	/** \brief Rig changed. */
	virtual void RigChanged( aeAnimator *animator );
	
	/** \brief Animation changed. */
	virtual void AnimationChanged( aeAnimator *animator );
	/*@}*/
};

#endif
