/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLGAMECONFIGXML_H_
#define _DEGLGAMECONFIGXML_H_

#include "../deglSharedConfigXML.h"

class deglGame;
class deglGameProfile;
class deglGPModule;
class decBaseFileReader;
class decBaseFileWriter;
class deglGameManager;



/**
 * \brief Load/Save Game Config XML.
 */
class deglGameConfigXML : public deglSharedConfigXML{
private:
	deglGameManager *pGameManager;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game config xml read/save. */
	deglGameConfigXML( deLogger *logger, const char *loggingSource, deglGameManager *gameManager );
	
	/** \brief Clean up game config xml read/save. */
	virtual ~deglGameConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, deglGame &game );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const deglGame &game );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const deglGame &game );
	
	void pReadConfig( const decXmlElementTag &root, deglGame &game );
};

#endif
