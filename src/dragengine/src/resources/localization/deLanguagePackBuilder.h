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

#ifndef _DELANGUAGEPACKBUILDER_H_
#define _DELANGUAGEPACKBUILDER_H_

#include "../../dragengine_export.h"

class deLanguagePack;


/**
 * \brief Language pack builder.
 *
 * Allows applications to produce language pack resources from runtime data not originating
 * from a file in the file system. Usually used by editors to create language pack resources
 * from the edited data.
 */
class DE_DLL_EXPORT deLanguagePackBuilder{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create language pack builder. */
	deLanguagePackBuilder();
	
	/** \brief Clean up language pack builder. */
	virtual ~deLanguagePackBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Build language pack into resource object. */
	virtual void BuildLanguagePack( deLanguagePack &langPack ) = 0;
	/*@}*/
};

#endif
