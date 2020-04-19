/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SECONFIGURATIONXML_H_
#define _SECONFIGURATIONXML_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class decBaseFileReader;
class decBaseFileWriter;
class seConfiguration;
class decXmlWriter;
class decXmlElementTag;



/**
 * @brief Load/Save Configuration XML.
 */
class seConfigurationXML : public igdeBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration xml read/save. */
	seConfigurationXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the configuration xml read/save. */
	virtual ~seConfigurationXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, seConfiguration &config );
	/** Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const seConfiguration &config );
	/*@}*/
	
private:
	void pWriteConfig( decXmlWriter &writer, const seConfiguration &config );
	
	void pReadConfig( const decXmlElementTag &root, seConfiguration &config );
};

#endif
