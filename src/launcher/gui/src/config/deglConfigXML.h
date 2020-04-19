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

#ifndef _DEGLCONFIGXML_H_
#define _DEGLCONFIGXML_H_

#include "../deglBaseXML.h"

#include <dragengine/common/string/decString.h>

class deglConfigWindow;
class decBaseFileReader;
class decBaseFileWriter;
class deglConfiguration;
class decXmlWriter;
class decXmlElementTag;



/**
 * @brief Load/Save Configuration XML.
 */
class deglConfigXML : public deglBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration xml read/save. */
	deglConfigXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the configuration xml read/save. */
	virtual ~deglConfigXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, deglConfiguration &config );
	/** Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const deglConfiguration &config );
	/*@}*/
	
private:
	void pWriteConfig( decXmlWriter &writer, const deglConfiguration &config );
	void pWriteWindow( decXmlWriter &writer, const deglConfigWindow &window, const char *tagName );
	
	void pReadConfig( const decXmlElementTag &root, deglConfiguration &config );
	void pReadWindow( const decXmlElementTag &root, deglConfigWindow &window );
};

#endif // _DEGLGAME_H_
