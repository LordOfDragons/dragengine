/* 
 * Drag[en]gine Live Launcher
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

#ifndef _DELLGAMECONFIGXML_H_
#define _DELLGAMECONFIGXML_H_

#include "../dellSharedConfigXML.h"

class dellGame;
class dellGameProfile;
class dellGPModule;
class decBaseFileReader;
class decBaseFileWriter;
class dellGameManager;



/**
 * \brief Load/Save Game Config XML.
 */
class dellGameConfigXML : public dellSharedConfigXML{
private:
	dellGameManager &pGameManager;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game config xml read/save. */
	dellGameConfigXML( deLogger *logger, const char *loggingSource, dellGameManager &gameManager );
	
	/** \brief Clean up game config xml read/save. */
	virtual ~dellGameConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, dellGame &game );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const dellGame &game );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const dellGame &game );
	
	void pReadConfig( const decXmlElementTag &root, dellGame &game );
};

#endif
