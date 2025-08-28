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

#ifndef _DEBASEFONTMODULE_H_
#define _DEBASEFONTMODULE_H_

#include "../deBaseModule.h"
#include "../../../resources/font/deFont.h"
#include "../../../resources/font/deFontSize.h"
#include "../../../common/file/decBaseFileReader.h"

class decBaseFileWriter;


/**
 * \brief Base font module providing load/save support for font files.
 */
class DE_DLL_EXPORT deBaseFontModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseFontModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	~deBaseFontModule() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load font. */
	virtual void LoadFont(decBaseFileReader &reader, deFont &font) = 0;
	
	/**
	 * \brief Load font size.
	 * 
	 * If loading font size is not fast a parallel load task has to be assigned. The load task
	 * has to update the font size instance until loading finished. Users must not touch the
	 * font size until the loader task is finished.
	 * 
	 * Module does creates a new deFontSize instance if the requested font size can be loaded
	 * successfully. If the requested font size is too large the module can create a deFontSize
	 * of smaller line height. In general font modules use a line height limit to avoid creating
	 * huge textures or font render engines not supporting large sizes. If a deFontSize with
	 * the largest supported size exists already the module has to return this one.
	 * 
	 * Default implementation throws deeInvalidAction exception.
	 */
	virtual deFontSize::Ref LoadFontSize(const decBaseFileReader::Ref &reader,
		const deFont::Ref &font, int lineHeight);
	
	/** \brief Save font. */
	virtual void SaveFont(decBaseFileWriter &writer, const deFont &font) = 0;
	/*@}*/
};

#endif
