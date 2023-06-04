/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AECONFIGURATIONXML_H_
#define _AECONFIGURATIONXML_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>
#include <dragengine/input/deInputEvent.h>

class decBaseFileReader;
class decBaseFileWriter;
class aeConfiguration;
class decXmlWriter;
class decXmlElementTag;



/**
 * Load/Save Configuration XML.
 */
class aeConfigurationXML : public igdeBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new configuration xml read/save. */
	aeConfigurationXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the configuration xml read/save. */
	virtual ~aeConfigurationXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, aeConfiguration &config );
	/** Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const aeConfiguration &config );
	/*@}*/
	
private:
	void pWriteConfig( decXmlWriter &writer, const aeConfiguration &config );
	void pWriteKey( decXmlWriter &writer, const char *name, deInputEvent::eKeyCodes key );
	
	void pReadConfig( const decXmlElementTag &root, aeConfiguration &config );
	deInputEvent::eKeyCodes pReadKey( const decXmlElementTag &root, deInputEvent::eKeyCodes defaultKey );
};

#endif
