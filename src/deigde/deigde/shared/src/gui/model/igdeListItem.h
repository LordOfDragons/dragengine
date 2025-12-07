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

#ifndef _IGDELISTITEM_H_
#define _IGDELISTITEM_H_

#include "../resources/igdeIcon.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief IGDE UI List Item.
 * 
 * Stores a text and optional icon representing a list item an widgets able to show lists.
 * Optionally a data pointer can be assigned.
 */
class DE_DLL_EXPORT igdeListItem : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeListItem> Ref;
	
	
private:
	decString pText;
	decString pDescription;
	igdeIcon::Ref pIcon;
	void *pData;
	decStringList pDetails;
	bool pSelected;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create list item. */
	igdeListItem(const char *text);
	
	igdeListItem(const char *text, igdeIcon *icon);
	
	igdeListItem(const char *text, igdeIcon *icon, const char *description);
	
	/** \brief Create list item. */
	igdeListItem(const char *text, void *data);
	
	igdeListItem(const char *text, igdeIcon *icon, void *data);
	
	igdeListItem(const char *text, igdeIcon *icon, const char *description, void *data);
	
	/** \brief Create copy of list item. */
	igdeListItem(const igdeListItem &listItem);
	
	
	
protected:
	/**
	 * \brief Clean up list item.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeListItem();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Text. */
	inline const decString &GetText() const{return pText;}
	
	/** \brief Set text. */
	void SetText(const char *text);
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{return pDescription;}
	
	/** \brief Set description shown in tool tips. */
	void SetDescription(const char *description);
	
	/** \brief Icon or NULL. */
	inline igdeIcon *GetIcon() const{return pIcon;}
	
	/** \brief Set icon or NULL. */
	void SetIcon(igdeIcon *icon);
	
	/** \brief User data pointer. */
	inline void *GetData() const{return pData;}
	
	/** \brief Set user data pointer. */
	void SetData(void *data);
	
	/**
	 * \brief Details used for igdeListBox only in detail mode.
	 * 
	 * Details are displayed for an item if the owning list box is in detail mode and
	 * the number of columns shown is larger than the detail index. Details exteeding
	 * the column count are ignored. If less details are present than the column count
	 * requires the missing details are considered empty strings.
	 */
	decStringList &GetDetails(){return pDetails;}
	inline const decStringList &GetDetails() const{return pDetails;}
	
	/** \brief Item is selected. */
	inline bool GetSelected() const{return pSelected;}
	
	/** \brief Set if item is selected. */
	void SetSelected(bool selected);
	
	
	
	/** \brief Set from another item. */
	igdeListItem &operator=(const igdeListItem &listItem);
	/*@}*/
};

#endif
