/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELGAMEXML_H_
#define _DELGAMEXML_H_

#include "../utils/delBaseXML.h"

class delGame;
class decBaseFileReader;


/**
 * \brief Load Game XML.
 */
class delGameXML : public delBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game xml read. */
	delGameXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up game xml read. */
	virtual ~delGameXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, delGame &game );
	/*@}*/
	
	
	
private:
	void pReadGame( const decXmlElementTag &root, delGame &game );
};

#endif
