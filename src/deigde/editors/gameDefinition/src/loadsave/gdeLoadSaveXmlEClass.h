/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDELOADSAVEXMLECLASS_H_
#define _GDELOADSAVEXMLECLASS_H_

#include "../gamedef/property/gdeProperty.h"

#include <deigde/utils/igdeBaseXML.h>
#include <deigde/codec/igdeCodecPropertyString.h>

#include <dragengine/common/string/decString.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeLoadSaveSystem;
class gdeOCComponentTexture;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;


/**
 * \brief Load/Save XML Element Class.
 */
class gdeLoadSaveXmlEClass : public igdeBaseXML{
private:
	gdeLoadSaveSystem &pLoadSaveSystem;
	igdeCodecPropertyString pCodec;
	
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save. */
	gdeLoadSaveXmlEClass( gdeLoadSaveSystem &loadSaveSystem, deLogger *logger,
		const char *loggerSource );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	
	
	/** \brief Load object class from xml element class file. */
	gdeObjectClass *LoadXmlEClass( decBaseFileReader &reader );
	
	/** \brief Save object class as xml element class file. */
	void SaveXmlEClass( const gdeObjectClass &objectClass, decBaseFileWriter &writer );
	/*@}*/
	
	
	
private:
	gdeObjectClass *pReadElementClass( const decXmlElementTag &root );
	
	void pWriteElementClass( decXmlWriter &writer, const gdeObjectClass &objectClass );
	
	void pWritePropertyValue( decXmlWriter &writer, const gdeObjectClass &objectClass,
		bool isMapEntry, const char *name, const decString &value );
	
	void pWritePropertyValue( decXmlWriter &writer, gdeProperty::ePropertyTypes type,
		bool isMapEntry, const char *name, const decString &value );
	
	void pWritePropertyValueType( decXmlWriter &writer, bool isMapEntry,
		const char *tagName, const char *name, const decString &value );
	
	void pWritePropertyTextureReplacement( decXmlWriter &writer, const gdeObjectClass &objectClass,
		const gdeOCComponentTexture &texture );
};

#endif
