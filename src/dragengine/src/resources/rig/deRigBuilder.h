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

#ifndef _DERIGBUILDER_H_
#define _DERIGBUILDER_H_

#include "../../dragengine_export.h"

class deRig;


/**
 * \brief Rig Builder.
 *
 * The rig builder is provided for applications requiring to produce
 * rig resources not from a file but from data somewhere in memory.
 * Usually used by editors to create rig resources from an in-editor
 * representation of a rig.
 */
class DE_DLL_EXPORT deRigBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new rig builder. */
	deRigBuilder();
	
	/** \brief Clean up rig builder. */
	virtual ~deRigBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Build rig into the provided rig object. */
	virtual void BuildRig( deRig *rig ) = 0;
	/*@}*/
};

#endif
