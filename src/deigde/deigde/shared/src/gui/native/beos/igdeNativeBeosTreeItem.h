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

#ifndef _IGDENATIVEBEOSTREEITEM_H_
#define _IGDENATIVEBEOSTREEITEM_H_

#include <interface/ListItem.h>
#include <dragengine/common/string/decString.h>

/**
 * \brief Tree node item for tree widget.
 */
class igdeNativeBeosTreeItem : public BListItem{
private:
	decString pText;
	int32 pLevel;
	bool pExpanded;
	igdeNativeBeosTreeItem *pParent;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create tree item. */
	explicit igdeNativeBeosTreeItem(const char *text, int32 level = 0);
	
	/** \brief Clean up tree item. */
	~igdeNativeBeosTreeItem() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Get text. */
	inline const char *GetText() const{ return pText.GetString(); }
	
	/** \brief Set text. */
	void SetText(const char *text);
	
	/** \brief Get level (indentation). */
	inline int32 GetLevel() const{ return pLevel; }
	
	/** \brief Set level (indentation). */
	void SetLevel(int32 level);
	
	/** \brief Get expanded state. */
	inline bool IsExpanded() const{ return pExpanded; }
	
	/** \brief Set expanded state. */
	void SetExpanded(bool expanded);
	
	/** \brief Get parent item. */
	inline igdeNativeBeosTreeItem *GetParent() const{ return pParent; }
	
	/** \brief Set parent item. */
	void SetParent(igdeNativeBeosTreeItem *parent);
	
	/** \brief Draw item. */
	void DrawItem(BView *owner, BRect frame, bool complete) override;
	
	/** \brief Get item height. */
	void GetHeight(BView *owner, int32 *height) const override;
	/*@}*/
};

#endif
