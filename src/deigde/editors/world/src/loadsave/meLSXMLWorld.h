/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MELSXMLWORLD_H_
#define _MELSXMLWORLD_H_

#include "meLSWorld.h"
#include <deigde/utils/igdeBaseXML.h>

class decStringDictionary;
class meDecal;
class meObject;
class meObjectTexture;
class meNavigationSpace;

class decXmlElementTag;
class decXmlWriter;



/**
 * Load and save world in the Drag[en]gine World XML format.
 */
class meLSXMLWorld : public meLSWorld, igdeBaseXML{
private:
	meLoadSaveSystem *pLSSys;
	
public:
	// constructor, destructor
	meLSXMLWorld( meLoadSaveSystem *lssys );
	virtual ~meLSXMLWorld();
	// loading and saving
	virtual void SaveWorld( meLoadSaveSystem &lssys, const meWorld &world, decBaseFileWriter *file );
	
	/** Creates a stepable loader. */
	virtual igdeStepableTask *CreateLoadTask( meWorld *world, decBaseFileReader *file );
	
	
	
private:
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name ) const;
	int pGetAttributeInt( const decXmlElementTag &tag, const char *name ) const;
	float pGetAttributeFloat( const decXmlElementTag &tag, const char *name ) const;
	
	void pWriteWorld( decXmlWriter &writer, const meWorld &world );
	void pWriteWorldEditor( decXmlWriter &writer, const meWorld &world );
	void pWriteObject( decXmlWriter &writer, const meObject &object );
	void pWriteObjectTexture( decXmlWriter &writer, const meObjectTexture &texture );
	void pWriteDecal( decXmlWriter &writer, const meDecal &decal );
	void pWriteNavigationSystem( decXmlWriter &writer, const meNavigationSpace &navspace );
	void pWriteProperties( decXmlWriter &writer, const decStringDictionary &properties );
};

#endif
