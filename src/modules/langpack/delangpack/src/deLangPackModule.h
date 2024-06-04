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

#ifndef _DELANGPACKMODULE_H_
#define _DELANGPACKMODULE_H_

#include <dragengine/systems/modules/langpack/deBaseLanguagePackModule.h>

class deLanguagePackEntry;
class decXmlWriter;
class decXmlElementTag;
class decXmlAttValue;
class deLanguagePack;


/**
 * \brief Drag[en]gine Language Pack Module.
 */
class deLangPackModule : public deBaseLanguagePackModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deLangPackModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deLangPackModule();
	/*@}*/
	
	
	
	/** \name Management */
	/** \brief Load  language pack. */
	virtual void LoadLanguagePack( decBaseFileReader &reader, deLanguagePack &languagePack );
	
	/** \brief Save language pack. */
	virtual void SaveLanguagePack( decBaseFileWriter &writer, const deLanguagePack &languagePack );
	/*@}*/
	
private:
	const decXmlAttValue *pFindAttribute( const decXmlElementTag &tag, const char *name );
	const char *pGetAttributeString( const decXmlElementTag &tag, const char *name );
	
	void pParseLangPack( const decXmlElementTag &root, deLanguagePack &languagePack );
	
	void pWriteLangPack( decXmlWriter &writer, const deLanguagePack &languagePack );
	void pWriteLangPackEntry( decXmlWriter &writer, const deLanguagePackEntry &entry );
};

#endif
