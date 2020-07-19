/* 
 * FBX Rig Module
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

#ifndef _FBXRIGMODULE_H_
#define _FBXRIGMODULE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/rig/deBaseRigModule.h>

class fbxScene;
class fbxNode;
class fbxProperty;
class fbxRigBone;


/**
 * \brief FBX Rig Module.
 */
class fbxRigModule : public deBaseRigModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	fbxRigModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~fbxRigModule();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Load rig. */
	virtual void LoadRig( decBaseFileReader &reader, deRig &rig );
	
	/** \brief Save rig. */
	virtual void SaveRig( decBaseFileWriter &writer, const deRig &rig );
	/*@}*/
	
	
	
private:
	void pLoadRig( deRig &rig, fbxScene &scene );
	
	void pLoadRigBone( deRig &rig, const fbxScene &scene, const fbxRigBone &bone );
};

#endif
