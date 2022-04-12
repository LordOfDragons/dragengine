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

#ifndef _DEBASESKINMODULE_H_
#define _DEBASESKINMODULE_H_

#include "../deBaseModule.h"

class decBaseFileReader;
class decBaseFileWriter;
class deSkin;


/**
 * \brief Base skin module providing load/save support for skins.
 */
class DE_DLL_EXPORT deBaseSkinModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseSkinModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseSkinModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load skin. */
	virtual void LoadSkin( decBaseFileReader &reader, deSkin &skin ) = 0;
	
	/** \brief Save skin. */
	virtual void SaveSkin( decBaseFileWriter &writer, const deSkin &skin ) = 0;
	/*@}*/
};

#endif
