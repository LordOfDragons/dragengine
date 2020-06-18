/* 
 * FBX Animation Module
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

#ifndef _FBXANIMMODULE_H_
#define _FBXANIMMODULE_H_

#include <dragengine/systems/modules/animation/deBaseAnimationModule.h>


class fbxScene;
class fbxRig;


/**
 * \brief FBX Animation Module.
 */
class fbxAnimModule : public deBaseAnimationModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	fbxAnimModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~fbxAnimModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load animation. */
	virtual void LoadAnimation( decBaseFileReader &reader, deAnimation &animation );
	
	/** \brief Save animation. */
	virtual void SaveAnimation( decBaseFileWriter &writer, const deAnimation &animation );
	/*@}*/
	
	
	
private:
	void pLoadAnimation( deAnimation &animation, fbxScene &scene );
};

#endif
