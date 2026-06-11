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

#ifndef _IGDEDIALOGSETSELECT_H_
#define _IGDEDIALOGSETSELECT_H_

#include "igdeDialog.h"
#include "../igdeListBoxFilter.h"

#include <dragengine/common/string/decStringSet.h>


class igdeIcon;


/**
 * \brief IGDE dialog supporting selection of multiple values from a set.
 */
class igdeDialogSetSelect : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeDialogSetSelect>;
	
	
private:
	igdeListBoxFilter::Ref pListBox;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogSetSelect(igdeEnvironment &environment, const char *title,
		const char *message, const decStringSet &set);
	
protected:
	/** \brief Clean up dialog. */
	~igdeDialogSetSelect() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selection. */
	decStringSet GetSelection() const;
	
	/** \brief Set selection. */
	void SetSelection(const decStringSet &selection);
	
	/**
	 * \brief Mark items.
	 * 
	 * This is useful to mark items which are present or absent in a set for the user to know
	 * better what selection to make.
	 */
	void MarkItems(const decStringSet &items, igdeIcon *iconPresent, igdeIcon *iconAbsent);
	
	/** \brief List box. */
	inline const igdeListBoxFilter::Ref &GetListBox() const{ return pListBox; }
	/*@}*/
};

#endif
