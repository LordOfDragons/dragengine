/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#ifndef _LPELANGPACKBUILDER_H_
#define _LPELANGPACKBUILDER_H_

#include <dragengine/resources/localization/deLanguagePackBuilder.h>

class lpeLangPack;



/**
 * \brief Language pack builder.
 */
class lpeLangPackBuilder : public deLanguagePackBuilder{
private:
	lpeLangPack *pLangPack;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new language pack builder. */
	lpeLangPackBuilder( lpeLangPack *langpack );
	/** \brief Cleans up the language pack builder. */
	virtual ~lpeLangPackBuilder();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Build language pack into the provided language pack object. */
	virtual void BuildLanguagePack( deLanguagePack &langPack );
	/*@}*/
};

#endif
