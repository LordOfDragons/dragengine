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

#ifndef _DELENGINECONFIGXML_H_
#define _DELENGINECONFIGXML_H_

#include "../utils/delSharedConfigXML.h"

#include <dragengine/common/string/decString.h>

class delLauncher;
class delGameProfile;
class delGPModule;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load/Save Engine Config XML.
 */
class DE_DLL_EXPORT delEngineConfigXML : public delSharedConfigXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create engine config xml read/save. */
	delEngineConfigXML( deLogger *logger, const char *loggerSource );
	
	/** \brief Clean up engine config xml read/save. */
	virtual ~delEngineConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, delLauncher &launcher );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const delLauncher &launcher );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const delLauncher &launcher );
	void pWriteProfiles( decXmlWriter &writer, const delLauncher &launcher );
	
	void pReadConfig( const decXmlElementTag &root, delLauncher &launcher );
	void pReadProfiles( const decXmlElementTag &root, delLauncher &launcher );
};

#endif
