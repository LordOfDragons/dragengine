/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLLSCONFIGURATION_H_
#define _DEOGLLSCONFIGURATION_H_

class deoglConfiguration;
class decBaseFileReader;
class decXmlElementTag;
class decXmlAttValue;
class deGraphicOpenGl;



/**
 * \brief Load/Save Configurating from/to XML file.
 */
class deoglLSConfiguration{
private:
	deGraphicOpenGl &pOgl;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save configuration. */
	deoglLSConfiguration( deGraphicOpenGl &ogl );
	
	/** \brief Clean up load/save configuration. */
	~deoglLSConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load configuration from XML files. */
	void LoadConfig( deoglConfiguration &configuration );
	
	/** \brief Save configuration to XML file. */
	void SaveConfig( const deoglConfiguration &configuration );
	/*@}*/
	
private:
	void pLoadConfigOpenGL( deoglConfiguration &configuration, decBaseFileReader &file );
	decXmlElementTag *pGetTagAt( decXmlElementTag *tag, int index );
	decXmlAttValue *pFindAttribute( decXmlElementTag *tag, const char *name );
	const char *pGetAttributeString( decXmlElementTag *tag, const char *name );
	int pGetAttributeInt( decXmlElementTag *tag, const char *name );
	float pGetAttributeFloat( decXmlElementTag *tag, const char *name );
};

#endif
