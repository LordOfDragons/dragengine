/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAELOADSAVESANIMATION_H_
#define _SAELOADSAVESANIMATION_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class saeSAnimation;
class saePhoneme;
class saeWord;
class saeLoadSaveSystem;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * Load/Save Speech Animation.
 */
class saeLoadSaveSAnimation : public igdeBaseXML{
private:
	saeLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	saeLoadSaveSAnimation( saeLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** Loads the speech animation from file. */
	void LoadSAnimation( saeSAnimation &sanimation, decBaseFileReader &reader );
	/** Saves the speech animation to file. */
	void SaveSAnimation( const saeSAnimation &sanimation, decBaseFileWriter &writer );
	/*@}*/
	
private:
	void pWriteSAnimation( decXmlWriter &writer, const saeSAnimation &sanimation );
	void pWriteDisplay( decXmlWriter &writer, const saeSAnimation &sanimation );
	void pWritePhoneme( decXmlWriter &writer, const saePhoneme &phoneme );
	void pWriteWord( decXmlWriter &writer, const saeWord &word );
	
	void pReadSAnimation( const decXmlElementTag &root, saeSAnimation &sanimation );
	void pReadDisplay( const decXmlElementTag &root, saeSAnimation &sanimation );
	void pReadPhoneme( const decXmlElementTag &root, saeSAnimation &sanimation );
	void pReadWord( const decXmlElementTag &root, saeSAnimation &sanimation );
};

#endif
