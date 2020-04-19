/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARANIMATOR_H_
#define _DEARANIMATOR_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorAnimator.h>

class deDEAnimator;
class deAnimator;



/**
 * \brief DEAnimator Animator Module Class.
 * Animator module.
 */
class dearAnimator : public deBaseAnimatorAnimator{
private:
	deDEAnimator &pModule;
	deAnimator &pAnimator;
	unsigned int pUpdateTracker;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator peer. */
	dearAnimator( deDEAnimator &module, deAnimator &animator );
	
	/** \brief Clean up animator. */
	virtual ~dearAnimator();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deDEAnimator &GetModule(){ return pModule; }
	inline const deDEAnimator &GetModule() const{ return pModule; }
	
	/** \brief Animator. */
	inline deAnimator &GetAnimator(){ return pAnimator; }
	inline const deAnimator &GetAnimator() const{ return pAnimator; }
	
	/** \brief Current update tracker state. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** \brief Rig changed. */
	virtual void RigChanged();
	
	/** \brief Animation changed. */
	virtual void AnimationChanged();
	
	/** \brief List of bones changed. */
	virtual void BonesChanged();
	
	/** \brief Controllers added or removed. */
	virtual void ControllerCountChanged();
	
	/** \brief Links added, removed or changed. */
	virtual void LinksChanged();
	
	/** \brief Rules added, removed or changed. */
	virtual void RulesChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
