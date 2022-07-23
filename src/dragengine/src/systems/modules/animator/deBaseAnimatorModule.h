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

#ifndef _DEBASEANIMATORMODULE_H_
#define _DEBASEANIMATORMODULE_H_

#include "../deBaseModule.h"

class deAnimation;
class deAnimator;
class deAnimatorInstance;
class deBaseAnimatorAnimation;
class deBaseAnimatorAnimator;
class deBaseAnimatorAnimatorInstance;
class deBaseAnimatorComponent;
class deComponent;



/**
 * \brief Base animator module providing support for producing animations.
 */
class DE_DLL_EXPORT deBaseAnimatorModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseAnimatorModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseAnimatorModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down module and clean up. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for animator. */
	virtual deBaseAnimatorAnimator *CreateAnimator( deAnimator *animator ) = 0;
	
	/** \brief Create peer for animator instance. */
	virtual deBaseAnimatorAnimatorInstance *CreateAnimatorInstance( deAnimatorInstance *instance ) = 0;
	
	/** \brief Create peer for animation. */
	virtual deBaseAnimatorAnimation *CreateAnimation( deAnimation *animation ) = 0;
	
	/** \brief Create peer for component. */
	virtual deBaseAnimatorComponent *CreateComponent( deComponent *component ) = 0;
	/*@}*/
};

#endif
