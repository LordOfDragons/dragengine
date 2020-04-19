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

#ifndef _DEGLENGINECONFIGXML_H_
#define _DEGLENGINECONFIGXML_H_

#include "../deglSharedConfigXML.h"

#include <dragengine/common/string/decString.h>

class deglLauncher;
class deglGameProfile;
class deglGPModule;
class decBaseFileReader;
class decBaseFileWriter;



/**
 * @brief Load/Save Engine Config XML.
 */
class deglEngineConfigXML : public deglSharedConfigXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine config xml read/save. */
	deglEngineConfigXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the engine config xml read/save. */
	virtual ~deglEngineConfigXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, deglLauncher &launcher );
	/** Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const deglLauncher &launcher );
	/*@}*/
	
private:
	void pWriteConfig( decXmlWriter &writer, const deglLauncher &launcher );
	void pWriteProfiles( decXmlWriter &writer, const deglLauncher &launcher );
	
	void pReadConfig( const decXmlElementTag &root, deglLauncher &launcher );
	void pReadProfiles( const decXmlElementTag &root, deglLauncher &launcher );
};

#endif
