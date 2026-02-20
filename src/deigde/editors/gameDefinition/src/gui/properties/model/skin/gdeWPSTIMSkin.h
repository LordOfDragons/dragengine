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

#ifndef _GDEWPSTIMSKIN_H_
#define _GDEWPSTIMSKIN_H_

#include "../gdeWPSTreeItemModel.h"

#include "../../../../gamedef/skin/gdeSkin.h"


/**
 * \brief Skin.
 */
class gdeWPSTIMSkin : public gdeWPSTreeItemModel{
private:
	gdeSkin::Ref pSkin;
	
	
public:
	typedef deTObjectReference<gdeWPSTIMSkin> Ref;

	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMSkin(gdeWPSTreeModel &tree, gdeSkin *skin);
	
protected:
	/** \brief Clean up tree item model. */
	~gdeWPSTIMSkin() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Skin. */
	inline const gdeSkin::Ref &GetSkin() const{ return pSkin; }
	
	
	
	/** \brief Name changed. */
	void NameChanged();
	
	/**
	 * \brief Compare this item with another for sorting.
	 * 
	 * Default implementation returns 0 to keep same order.
	 */
	int Compare(const gdeWPSTreeItemModel &item) const override;
	
	/** \brief User selected item. */
	void OnSelected() override;
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	
	/** \brief Select object mest matching name. */
	void SelectBestMatching(const char *string) override;
	/*@}*/
};

#endif
