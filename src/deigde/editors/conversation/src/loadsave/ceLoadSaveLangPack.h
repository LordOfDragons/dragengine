/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _CELOADSAVELANGPACK_H_
#define _CELOADSAVELANGPACK_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deBaseLanguagePackModule;
class ceLangPack;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * Load/Saves langpacks using an engine module.
 */
class ceLoadSaveLangPack : public deObject{
public:
	typedef deTObjectReference<ceLoadSaveLangPack> Ref;
	
	
	
private:
	deBaseLanguagePackModule &pModule;
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create load/save object. */
	ceLoadSaveLangPack( deBaseLanguagePackModule &module );
	
protected:
	/** Clean up load/save object. */
	virtual ~ceLoadSaveLangPack() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** Load langpack. */
	void LoadLangPack( ceLangPack &langpack, decBaseFileReader &file );
	
	/** Save langpack. */
	void SaveLangPack( const ceLangPack &langpack, decBaseFileWriter &file );
	/*@}*/
};

#endif
