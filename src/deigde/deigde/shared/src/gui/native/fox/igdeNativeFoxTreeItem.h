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

#ifndef _IGDENATIVEFOXTREEITEM_H_
#define _IGDENATIVEFOXTREEITEM_H_

#include "foxtoolkit.h"
#include "../../model/igdeTreeItem.h"
#include "../../resources/igdeFont.h"


/**
 * \brief FOX toolkit Native tree item.
 */
class igdeNativeFoxTreeItem : public FXTreeItem{
	FXDECLARE(igdeNativeFoxTreeItem)
protected:
	   igdeNativeFoxTreeItem();
	
private:
	igdeTreeItem::Ref pOwner;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxTreeItem(igdeTreeItem &owner);
	
	/** \brief Clean up native widget. */
	~igdeNativeFoxTreeItem() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner. */
	inline const igdeTreeItem::Ref &GetOwner() const{ return pOwner; }
	
	/** \brief Tip text. */
	FXString getTipText() const override;
	/*@}*/
};

#endif
