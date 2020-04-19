/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CELOADSAVECTGS_H_
#define _CELOADSAVECTGS_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class cePlayback;
class ceLoadSaveSystem;
class ceConversation;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save conversation test game state.
 */
class ceLoadSaveCTGS : public igdeBaseXML{
private:
	ceLoadSaveSystem &pLoadSaveSystem;
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	ceLoadSaveCTGS( ceLoadSaveSystem &loadSaveSystem, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Load conversation test game state. */
	void LoadCTGS( ceConversation &conversation, decBaseFileReader &reader );
	
	/** \brief Save conversation test game state. */
	void SaveCTGS( const ceConversation &conversation, decBaseFileWriter &writer );
	
	
	
	/** \brief Write game state tag. */
	void WriteGameState( decXmlWriter &writer, const cePlayback &playback, const char *tagName );
	
	/** \brief Read game state tag. */
	void ReadGameState( const decXmlElementTag &root, cePlayback &playback );
	/*@}*/
	
	
	
private:
};

#endif
