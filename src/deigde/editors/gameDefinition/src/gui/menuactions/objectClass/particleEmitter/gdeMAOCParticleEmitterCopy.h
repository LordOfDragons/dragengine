/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEMAOCPARTICLEEMITTERCOPY_H_
#define _GDEMAOCPARTICLEEMITTERCOPY_H_

#include "../gdeBaseMAOCSubObject.h"


/**
 * \brief Menu action copy object class.
 */
class gdeMAOCParticleEmitterCopy : public gdeBaseMAOCSubObject{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create menu actions. */
	gdeMAOCParticleEmitterCopy( gdeWindowMain &windowMain );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run action if game definition is not NULL. */
	virtual igdeUndo *OnActionSubObject( gdeGameDefinition &gameDefinition, gdeObjectClass &objectClass );
	
	/** \brief Request update of action parameters if required. */
	virtual void Update();
	/*@}*/
};

#endif
