/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEANIMATORANIMATOR_H_
#define _DEBASEANIMATORANIMATOR_H_

#include "../../../dragengine_export.h"

class deAnimatorRule;
class deAnimatorLink;
class deAnimatorController;



/**
 * \brief Animator module animator peer.
 */
class DE_DLL_EXPORT deBaseAnimatorAnimator{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseAnimatorAnimator();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAnimatorAnimator();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Rig changed. */
	virtual void RigChanged();
	
	/** \brief Animation changed. */
	virtual void AnimationChanged();
	
	/** \brief List of bones changed. */
	virtual void BonesChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int index, deAnimatorController *controller );
	
	/** \brief Controllers added or removed. */
	virtual void ControllerCountChanged();
	
	/** \brief Links added, removed or changed. */
	virtual void LinksChanged();
	
	/** \brief Rules added, removed or changed. */
	virtual void RulesChanged();
	/*@}*/
};

#endif
