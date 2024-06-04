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

#ifndef _IGDENATIVENULLLISTBOX_H_
#define _IGDENATIVENULLLISTBOX_H_

class igdeListBox;


/**
 * Null list box.
 */
class igdeNativeNullListBox{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullListBox();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullListBox();
	
	/** \brief Create native widget. */
	static igdeNativeNullListBox* CreateNativeWidget( igdeListBox &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void BuildList();
	virtual void UpdateItem( int index );
	virtual void UpdateStyles();
	virtual void UpdateSelection();
	virtual void Focus();
	virtual void MakeItemVisible( int index );
	virtual void InsertItem( int index );
	virtual void RemoveItem( int index );
	virtual void RemoveAllItems();
	virtual void MoveItem( int fromIndex, int toIndex );
	virtual void UpdateEnabled();
	virtual void UpdateRowCount();
	virtual void UpdateDescription();
	/*@}*/
};

typedef igdeNativeNullListBox igdeNativeListBox;

#endif
