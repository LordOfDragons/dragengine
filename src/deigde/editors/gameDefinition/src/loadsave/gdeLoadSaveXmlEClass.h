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
class gdeOCComponentTextureList;

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
	gdeLoadSaveXmlEClass(gdeLoadSaveSystem &loadSaveSystem, deLogger *logger,
		const char *loggerSource);
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	
	
	/** \brief Load object class from xml element class file. */
	gdeObjectClass *LoadXmlEClass(decBaseFileReader &reader);
	
	/** \brief Save object class as xml element class file. */
	void SaveXmlEClass(const gdeGameDefinition &gameDefinition,
		const gdeObjectClass &objectClass, decBaseFileWriter &writer);
	/*@}*/
	
	
	
private:
	gdeObjectClass *pReadElementClass(const decXmlElementTag &root);
	
	void pWriteElementClass(decXmlWriter &writer, const gdeGameDefinition &gameDefinition,
		const gdeObjectClass &objectClass);
	
	void pCollectTextures(const gdeGameDefinition &gameDefinition,
		const gdeObjectClass &objectClass, gdeOCComponentTextureList &list);
	
	void pWritePropertyValue(decXmlWriter &writer, const gdeObjectClass &objectClass,
		bool isMapEntry, const char *name, const decString &value);
	
	void pWritePropertyValue(decXmlWriter &writer, gdeProperty::ePropertyTypes type,
		bool isMapEntry, const char *name, const decString &value);
	
	void pWritePropertyValueType(decXmlWriter &writer, bool isMapEntry,
		const char *tagName, const char *name, const decString &value);
	
	void pWritePropertyTextureReplacement(decXmlWriter &writer, const gdeObjectClass &objectClass,
		const gdeOCComponentTexture &texture);
};

#endif
