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

#ifndef _DEDEANIMATOR_H_
#define _DEDEANIMATOR_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorModule.h>



/**
 * \brief DEAnimator animator module.
 */
class deDEAnimator : public deBaseAnimatorModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animator module. */
	deDEAnimator( deLoadableModule &loadableModule );
	
	/** \brief Clean up animator module. */
	virtual ~deDEAnimator();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize module. */
	virtual bool Init();
	
	/** \brief Shut down module and clean up. */
	virtual void CleanUp();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for animator. */
	virtual deBaseAnimatorAnimator *CreateAnimator( deAnimator *animator );
	
	/** \brief Create peer for animator instance. */
	virtual deBaseAnimatorAnimatorInstance *CreateAnimatorInstance( deAnimatorInstance *instance );
	
	/** \brief Create peer for animation. */
	virtual deBaseAnimatorAnimation *CreateAnimation( deAnimation *animation );
	
	/** \brief Create peer for component. */
	virtual deBaseAnimatorComponent *CreateComponent( deComponent *component );
	/*@}*/
};

#endif
