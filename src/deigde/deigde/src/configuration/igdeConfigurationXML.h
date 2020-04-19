/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDECONFIGURATIONXML_H_
#define _IGDECONFIGURATIONXML_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class decBaseFileReader;
class decBaseFileWriter;
class igdeConfiguration;
class decXmlWriter;
class decXmlElementTag;
class igdeWindowMain;



/**
 * @brief Load/Save Configuration XML.
 */
class igdeConfigurationXML : public igdeBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration xml read/save. */
	igdeConfigurationXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the configuration xml read/save. */
	virtual ~igdeConfigurationXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, igdeConfiguration &config );
	/** Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const igdeConfiguration &config );
	/*@}*/
	
private:
	void pReadConfig( const decXmlElementTag &root, igdeConfiguration &config );
	void pReadWindowMain( const decXmlElementTag &root, igdeWindowMain &window );
	
	void pWriteConfig( decXmlWriter &writer, const igdeConfiguration &config );
	void pWriteWindowMain( decXmlWriter &writer, const igdeWindowMain &window, const char *tagName );
};

#endif
