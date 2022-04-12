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

#ifndef _DEBASELANGUAGEPACKMODULE_H_
#define _DEBASELANGUAGEPACKMODULE_H_

#include "../deBaseModule.h"

class decBaseFileWriter;
class decBaseFileReader;
class deLanguagePack;


/**
 * \brief Base language pack module providing load/save support for language packs.
 */
class DE_DLL_EXPORT deBaseLanguagePackModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseLanguagePackModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseLanguagePackModule();
	/*@}*/
	
	
	
	/** \name Management */
	/** \brief Load  language pack. */
	virtual void LoadLanguagePack( decBaseFileReader &reader, deLanguagePack &languagePack ) = 0;
	
	/** \brief Save language pack. */
	virtual void SaveLanguagePack( decBaseFileWriter &writer, const deLanguagePack &languagePack ) = 0;
	
	/**
	 * \brief File is compressed.
	 * \version 1.12
	 * \note Call has to be thread safe.
	 */
	virtual bool IsCompressed( decBaseFileReader &reader );
	/*@}*/
};

#endif
