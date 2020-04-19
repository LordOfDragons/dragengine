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

#ifndef _DEGLENGINEMODULEXML_H_
#define _DEGLENGINEMODULEXML_H_

#include "../../deglBaseXML.h"

#include <dragengine/common/string/decString.h>

class deglEngineModule;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * @brief Load Engine Module XML.
 */
class deglEngineModuleXML : public deglBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine module xml read. */
	deglEngineModuleXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the engine module xml read. */
	virtual ~deglEngineModuleXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( const char *filename, decBaseFileReader &reader, deglEngineModule &module );
	/*@}*/
	
private:
	void pReadModule( const decXmlElementTag &root, deglEngineModule &module );
	void pReadModuleLibrary( const decXmlElementTag &root, deglEngineModule &module );
};

#endif // _DEGLGAME_H_
