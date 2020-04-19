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

#ifndef _CELOADSAVECTS_H_
#define _CELOADSAVECTS_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class ceProp;
class ceCoordSystem;
class ceActorController;
class ceLoadSaveSystem;
class ceConversation;
class ceConversationActor;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save Conversation Test Setup.
 */
class ceLoadSaveCTS : public igdeBaseXML{
private:
	ceLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	ceLoadSaveCTS( ceLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** Loads the conversation test setup from file. */
	void LoadCTS( ceConversation &conversation, decBaseFileReader &reader );
	/** Saves the conversation test setup to file. */
	void SaveCTS( const ceConversation &conversation, decBaseFileWriter &writer );
	/*@}*/
	
private:
	void pWriteCTS( decXmlWriter &writer, const ceConversation &conversation );
	void pWriteCoordSystem( decXmlWriter &writer, const ceCoordSystem &coordSystem );
	void pWriteProp( decXmlWriter &writer, const ceProp &prop );
	
	void pReadCTS( const decXmlElementTag &root, ceConversation &conversation );
	void pReadCoordSystem( const decXmlElementTag &root, ceConversation &conversation );
	void pReadProp( const decXmlElementTag &root, ceConversation &conversation );
};

#endif
