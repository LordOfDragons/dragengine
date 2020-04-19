/* 
 * Drag[en]gine Font Module
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

// include only once
#ifndef _DEFONTMODULE_H_
#define _DEFONTMODULE_H_

// includes
#include <dragengine/systems/modules/font/deBaseFontModule.h>

// predefinitions
class deFontGlyph;

class decXmlElementTag;
class decXmlAttValue;
class decXmlWriter;



/**
 * @brief Drag[en]gine Font Module.
 * Font module for loading and saving font files in the Drag[en]gine
 * XML Font Format.
 */
class deFontModule : public deBaseFontModule {
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deFontModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deFontModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load font. */
	virtual void LoadFont( decBaseFileReader &reader, deFont &font );
	
	/** \brief Save font. */
	virtual void SaveFont( decBaseFileWriter &writer, const deFont &font );
	/*@}*/

private:
    const char *pGetAttributeString( decXmlElementTag *tag, const char *name );
    bool pHasAttributeString( decXmlElementTag *tag, const char *name );
    int pGetAttributeInt( decXmlElementTag *tag, const char *name );
    float pGetAttributeFloat( decXmlElementTag *tag, const char *name );
    void pParseFont( decXmlElementTag *root, deFont &font );

    void pWriteFont( decXmlWriter &writer, const deFont &font );
};

// end of include only once
#endif
