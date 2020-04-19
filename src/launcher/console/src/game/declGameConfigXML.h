/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLGAMECONFIGXML_H_
#define _DECLGAMECONFIGXML_H_

#include "../declSharedConfigXML.h"

class declGame;
class declGameProfile;
class declGPModule;
class decBaseFileReader;
class decBaseFileWriter;
class declGameManager;



/**
 * \brief Load/Save Game Config XML.
 */
class declGameConfigXML : public declSharedConfigXML{
private:
	declGameManager *pGameManager;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game config xml read/save. */
	declGameConfigXML( deLogger *logger, const char *loggingSource, declGameManager *gameManager );
	
	/** \brief Clean up game config xml read/save. */
	virtual ~declGameConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, declGame &game );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const declGame &game );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const declGame &game );
	
	void pReadConfig( const decXmlElementTag &root, declGame &game );
};

#endif
