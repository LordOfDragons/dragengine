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

#ifndef _DEBASERIGMODULE_H_
#define _DEBASERIGMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deRig;


/**
 * \brief Base rig module providing load/save support for rigs
 */
class DE_DLL_EXPORT deBaseRigModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseRigModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseRigModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load rig. */
	virtual void LoadRig( decBaseFileReader &reader, deRig &rig ) = 0;
	
	/** \brief Save rig. */
	virtual void SaveRig( decBaseFileWriter &writer, const deRig &rig ) = 0;
	
	/**
	 * \brief File is compressed.
	 * \version 1.12
	 * \note Call has to be thread safe.
	 */
	virtual bool IsCompressed( decBaseFileReader &reader );
	/*@}*/
};

#endif
