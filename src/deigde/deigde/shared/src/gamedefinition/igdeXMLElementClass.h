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

#ifndef _IGDEXMLELEMENTCLASS_H_
#define _IGDEXMLELEMENTCLASS_H_

#include "../utils/igdeBaseXML.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectDictionary.h>

class igdeGameDefinition;
class igdeGDClass;
class igdeGDClassManager;
class igdeGDCComponent;

class deLogger;
class decPath;
class decBaseFileReader;
class decBaseFileWriter;
class decXmlElementTag;
class decStringDictionary;
class deVirtualFileSystem;



/**
 * \brief Import XML Element Classes.
 * 
 * Imports possible and understood data. Unsupported data is ignored.
 */
class igdeXMLElementClass : public igdeBaseXML{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	igdeXMLElementClass( deLogger *logger );
	
	/** \brief Clean up loader. */
	virtual ~igdeXMLElementClass();
	/*@}*/
	
	
	
	/** \name Loading */
	/*@{*/
	/** \brief Load XML Element Class. */
	igdeGDClass *LoadElementClass( decBaseFileReader &reader, const char *filename );
	
	/** \brief Load XML Element Classes from directory and add them to game definition. */
	void LoadElementClasses( igdeGDClassManager &classes, deVirtualFileSystem &vfs,
		const decPath &directory, const char *pattern = "*.deeclass", bool recursive = true );
	
	
	
protected:
	class cMap : public deObject{
	public:
		decString value;
		decObjectDictionary map;
		cMap();
		
	protected:
		virtual ~cMap();
	};
	
	decBoundary ReadBorderSize( const decXmlElementTag &root );
	decBoundary ReadRectArea( const decXmlElementTag &root );
	decVector4 ReadFloatRectArea( const decXmlElementTag &root );
	void pReadECColor( const decXmlElementTag &root, decColor &color, const char *filename );
	
	
	
private:
	igdeGDClass *pReadElementClass( const decXmlElementTag &root, const char *filename );
	bool pReadPropertyValue( const decXmlElementTag &root, decString &value,
		cMap *map, const char *filename );
	void pReadList( const decXmlElementTag &root, decStringList &list, const char *filename );
	void pReadMap( const decXmlElementTag &root, cMap &map, const char *filename );
	void pProcessTextureReplacements( const cMap &map, igdeGDClass &gdClass, const char *basePath );
	//igdeGDCComponent &pGetLoadedComponent( igdeGDClass &gdClass );
};

#endif
