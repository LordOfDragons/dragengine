/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALENGINEMODULEXML_H_
#define _DEALENGINEMODULEXML_H_

#include "dealEngineModule.h"
#include "../../dealBaseXML.h"
#include "../../common/string/decString.h"

class decBaseFileReader;
class decBaseFileWriter;



/**
 * @brief Load Engine Module XML.
 */
class dealEngineModuleXML : public dealBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine module xml read. */
	dealEngineModuleXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the engine module xml read. */
	virtual ~dealEngineModuleXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, dealEngineModule &module );
	/*@}*/
	
private:
	void pReadModule( const decXmlElementTag &root, dealEngineModule &module );
	void pReadModuleLibrary( const decXmlElementTag &root, dealEngineModule &module );
	dealEngineModule::eModuleTypes pModuleTypeFromString( const char *typeString ) const;
};

#endif // _DEGLGAME_H_
