/* 
 * Drag[en]gine Basic Network Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEBNLSCONFIGURATION_H_
#define _DEBNLSCONFIGURATION_H_

class debnConfiguration;
class decBaseFileReader;
class decXmlElementTag;
class decXmlAttValue;
class deNetworkBasic;


/**
 * Load/Save Configurating from/to XML file.
 */
class debnLoadConfiguration{
private:
	deNetworkBasic &pNetwork;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create load configuration. */
	debnLoadConfiguration( deNetworkBasic &network );
	
	/** Clean up load configuration. */
	~debnLoadConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Load configuration from XML files. */
	void LoadConfig( debnConfiguration &configuration );
	/*@}*/
	
	
	
private:
	void pLoadConfiguration( debnConfiguration &configuration, decBaseFileReader &reader );
	decXmlElementTag *pGetTagAt( decXmlElementTag *tag, int index );
	decXmlAttValue *pFindAttribute( decXmlElementTag *tag, const char *name );
	const decString &pGetAttributeString( decXmlElementTag *tag, const char *name );
	int pGetAttributeInt( decXmlElementTag *tag, const char *name );
	float pGetAttributeFloat( decXmlElementTag *tag, const char *name );
};

#endif
