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

#ifndef _DELENGINEMODULEXML_H_
#define _DELENGINEMODULEXML_H_

#include "../../utils/delBaseXML.h"

#include <dragengine/common/string/decString.h>

class delEngineModule;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load Engine Module XML.
 */
class DE_DLL_EXPORT delEngineModuleXML : public delBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine module xml read. */
	delEngineModuleXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up engine module xml read. */
	virtual ~delEngineModuleXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( const char *filename, decBaseFileReader &reader, delEngineModule &module );
	/*@}*/
	
	
	
private:
	void pReadModule( const decXmlElementTag &root, delEngineModule &module );
	void pReadModuleLibrary( const decXmlElementTag &root, delEngineModule &module );
};

#endif
