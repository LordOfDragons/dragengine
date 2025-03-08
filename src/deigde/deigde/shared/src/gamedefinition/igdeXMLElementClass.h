/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
class DE_DLL_EXPORT igdeXMLElementClass : public igdeBaseXML{
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
	igdeGDClass *LoadElementClass(decBaseFileReader &reader, const char *filename);
	
	/** \brief Load XML Element Classes from directory and add them to game definition. */
	void LoadElementClasses(igdeGDClassManager &classes, deVirtualFileSystem &vfs,
		const decPath &directory, const char *pattern = "*.deeclass", bool recursive = true);
	
	
	
protected:
	class DE_DLL_EXPORT cMap : public deObject{
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
	igdeGDClass *pReadElementClass(const decXmlElementTag &root, const char *filename);
	bool pReadPropertyValue( const decXmlElementTag &root, decString &value,
		cMap *map, const char *filename );
	void pReadList( const decXmlElementTag &root, decStringList &list, const char *filename );
	void pReadMap( const decXmlElementTag &root, cMap &map, const char *filename );
	void pProcessTextureReplacements( const cMap &map, igdeGDClass &gdClass, const char *basePath );
	//igdeGDCComponent &pGetLoadedComponent( igdeGDClass &gdClass );
};

#endif
