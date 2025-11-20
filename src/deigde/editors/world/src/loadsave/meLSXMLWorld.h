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
	void pWriteWorldEditorBackgroundObject(decXmlWriter &writer, const meWorld &world);
	void pWriteWorldEditorLimitBox(decXmlWriter &writer, const meWorld &world);
	void pWriteObject( decXmlWriter &writer, const meObject &object );
	void pWriteObjectTexture( decXmlWriter &writer, const meObjectTexture &texture );
	void pWriteDecal( decXmlWriter &writer, const meDecal &decal );
	void pWriteNavigationSystem( decXmlWriter &writer, const meNavigationSpace &navspace );
	void pWriteProperties( decXmlWriter &writer, const decStringDictionary &properties );
};

#endif
