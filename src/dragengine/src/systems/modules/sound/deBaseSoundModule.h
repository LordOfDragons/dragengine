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

#ifndef _DEBASESOUNDMODULE_H_
#define _DEBASESOUNDMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deBaseSoundDecoder;
class deBaseSoundInfo;
class deSound;


/**
 * \brief Base sound module providing load/save support for sounds.
 */
class DE_DLL_EXPORT deBaseSoundModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseSoundModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseSoundModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read sound information from file. */
	virtual void InitLoadSound( decBaseFileReader &reader, deBaseSoundInfo &info ) = 0;
	
	/**
	 * \brief Save sound.
	 * 
	 * You can be sure that the file provided has been rewinded prior to this
	 * function call.
	 */
	virtual void SaveSound( decBaseFileWriter &writer, const deSound &sound ) = 0;
	
	/** \brief Create sound decoder peer. */
	virtual deBaseSoundDecoder *CreateDecoder( decBaseFileReader *reader ) = 0;
	/*@}*/
};

#endif
