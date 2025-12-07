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
	deFontModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	virtual ~deFontModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load font. */
	virtual void LoadFont(decBaseFileReader &reader, deFont &font);
	
	/** \brief Save font. */
	virtual void SaveFont(decBaseFileWriter &writer, const deFont &font);
	/*@}*/

private:
    const char *pGetAttributeString(decXmlElementTag *tag, const char *name);
    bool pHasAttributeString(decXmlElementTag *tag, const char *name);
    int pGetAttributeInt(decXmlElementTag *tag, const char *name);
    float pGetAttributeFloat(decXmlElementTag *tag, const char *name);
    void pParseFont(decXmlElementTag *root, deFont &font);

    void pWriteFont(decXmlWriter &writer, const deFont &font);
};

// end of include only once
#endif
