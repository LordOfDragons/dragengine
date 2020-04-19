/* 
 * Drag[en]gine Language Pack Module
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

#ifndef _DELANGPACKMODULE_H_
#define _DELANGPACKMODULE_H_

#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>

class deLanguagePackEntry;
class decXmlWriter;
class decXmlElementTag;
class decXmlAttValue;
class deLanguagePack;


/**
 * \brief Drag[en]gine Language Pack Module.
 */
class deLangPackModule : public deBaseLanguagePackModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deLangPackModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deLangPackModule();
	/*@}*/
	
	
	
	/** \name Management */
	/** \brief Load  language pack. */
	virtual void LoadLanguagePack( decBaseFileReader &reader, deLanguagePack &languagePack );
	
	/** \brief Save language pack. */
	virtual void SaveLanguagePack( decBaseFileWriter &writer, const deLanguagePack &languagePack );
	/*@}*/
	
private:
	const decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name );
	
	void pParseLangPack( const decXmlElementTag &root, deLanguagePack &languagePack );
	
	void pWriteLangPack( decXmlWriter &writer, const deLanguagePack &languagePack );
	void pWriteLangPackEntry( decXmlWriter &writer, const deLanguagePackEntry &entry );
};

#endif
