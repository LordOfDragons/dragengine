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

#ifndef _DELGAMECONFIGXML_H_
#define _DELGAMECONFIGXML_H_

#include "../utils/delSharedConfigXML.h"

class delGame;
class decBaseFileReader;
class decBaseFileWriter;
class delLauncher;


/**
 * \brief Load/Save Game Config XML.
 */
class delGameConfigXML : public delSharedConfigXML{
private:
	delLauncher &pLauncher;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game config xml read/save. */
	delGameConfigXML( delLauncher &launcher );
	
	/** \brief Clean up game config xml read/save. */
	virtual ~delGameConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, delGame &game );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const delGame &game );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const delGame &game );
	
	void pReadConfig( const decXmlElementTag &root, delGame &game );
};

#endif
