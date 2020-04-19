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

#ifndef _DELLGAMEXML_H_
#define _DELLGAMEXML_H_

#include "../dellBaseXML.h"

#include <dragengine/common/string/decString.h>

class dellGame;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * @brief Load Game XML.
 */
class dellGameXML : public dellBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new game xml read. */
	dellGameXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the game xml read. */
	virtual ~dellGameXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, dellGame &game );
	/*@}*/
	
private:
	void pReadGame( const decXmlElementTag &root, dellGame &game );
	void pReadRequiredFormat( const decXmlElementTag &root, dellGame &game );
};

#endif
