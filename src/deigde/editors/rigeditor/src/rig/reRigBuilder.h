/* 
 * Drag[en]gine IGDE Rig Editor
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

// include only once
#ifndef _RERIGBUILDER_H_
#define _RERIGBUILDER_H_

// includes
#include "dragengine/resources/rig/deRigBuilder.h"

// predefinitions
class reRig;
class reRigBone;



/**
 * @brief Rig Builder.
 * Builds an engine rig from an editor rig.
 */
class reRigBuilder : public deRigBuilder{
private:
	reRig *pRig;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig builder. */
	reRigBuilder( reRig *rig );
	/** Cleans up the rig. */
	virtual ~reRigBuilder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Build rig into the provided rig object. */
	virtual void BuildRig( deRig *engRig );
	/** Build rig bone. */
	void BuildRigBone( deRig *engRig, reRigBone *rigBone );
	/*@}*/
};

// end of include only once
#endif
