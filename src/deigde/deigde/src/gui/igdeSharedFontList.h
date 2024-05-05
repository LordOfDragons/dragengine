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

#ifndef _IGDESHAREDFONTSLIST_H_
#define _IGDESHAREDFONTSLIST_H_

#include <deigde/gui/resources/igdeFont.h>
#include <deigde/gui/resources/igdeFontReference.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class igdeEnvironment;



/**
 * \brief IGDE Shared font list.
 */
class igdeSharedFontList{
private:
	class cFont : public deObject{
	private:
		igdeFont::sConfiguration pConfig;
		igdeFontReference pFont;
		
	public:
		cFont( const igdeFont::sConfiguration &config, igdeFont *font );
		virtual ~cFont();
		
		inline igdeFont *GetFont() const{ return pFont; }
		inline const igdeFont::sConfiguration &GetConfig() const{ return pConfig; }
	};
	
	igdeEnvironment &pEnvironment;
	decObjectList pFonts;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared font list. */
	igdeSharedFontList( igdeEnvironment &environment );
	
	/** \brief Clean up shared font list. */
	~igdeSharedFontList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get font matching configuration. */
	igdeFont *GetFontWith( const igdeFont::sConfiguration &configuration );
	/*@}*/
};

#endif
