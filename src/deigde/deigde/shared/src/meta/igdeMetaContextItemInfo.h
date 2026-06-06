/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDETMETACONTEXTITEMINFO_H_
#define _IGDETMETACONTEXTITEMINFO_H_

#include "../gui/resources/igdeIcon.h"

#include <dragengine/common/string/decString.h>


/**
 * \brief Meta context item information.
 * 
 * For use by meta context property widgets to retrieve information about an object to be shown
 * in the widget specific item representation.
 */
class DE_DLL_EXPORT igdeMetaContextItemInfo{
private:
	decString pText, pDescription;
	igdeIcon::Ref pIcon;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create converter. */
	igdeMetaContextItemInfo();
	
	/** \brief Clean up converter. */
	~igdeMetaContextItemInfo();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const decString &name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const { return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const decString &description);
	
	/** \brief Icon. */
	inline const igdeIcon::Ref &GetIcon() const{ return pIcon; }
	
	/** \brief Set icon. */
	void SetIcon(const igdeIcon::Ref &icon);
	
	/** \brief Clear. */
	void Clear();
	
	/** \brief Set all information. */
	void SetAll(const char *text);
	void SetAll(const char *text, const char *description);
	void SetAll(const char *text, const char *description, const igdeIcon::Ref &icon);
};

#endif
