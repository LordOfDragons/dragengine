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

#ifndef _IGDEDIALOGTEXTUREPROPERTYLIST_H
#define _IGDEDIALOGTEXTUREPROPERTYLIST_H

#include "igdeDialog.h"
#include "../igdeListBox.h"
#include "../igdeTextField.h"
#include "../igdeTextArea.h"


/**
 * \brief Texture Property List Dialog.
 * 
 * Dialog showing content of texture property list.
 */
class DE_DLL_EXPORT igdeDialogTexturePropertyList : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeDialogTexturePropertyList> Ref;
	
	
private:
	igdeListBox::Ref pListProperties;
	igdeTextField::Ref pEditName;
	igdeTextArea::Ref pEditDescription;
	igdeTextField::Ref pEditType;
	igdeTextField::Ref pEditComponentCount;
	igdeTextField::Ref pEditDefault;
	igdeTextField::Ref pEditAffectedModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	explicit igdeDialogTexturePropertyList(igdeEnvironment &environment);
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogTexturePropertyList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update list of properties. */
	void UpdatePropertyList();
	
	/** \brief Update selected property. */
	void UpdateProperty();
	/*@}*/
};

#endif
