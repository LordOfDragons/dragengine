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

#ifndef _GDEWPSTIMSKY_H_
#define _GDEWPSTIMSKY_H_

#include "../gdeWPSTreeItemModel.h"

class gdeSky;


/**
 * \brief Sky.
 */
class gdeWPSTIMSky : public gdeWPSTreeItemModel{
private:
	gdeSky *pSky;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMSky(gdeWPSTreeModel &tree, gdeSky *sky);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMSky();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Sky. */
	inline gdeSky *GetSky() const{return pSky;}
	
	
	
	/** \brief Name changed. */
	void NameChanged();
	
	/**
	 * \brief Compare this item with another for sorting.
	 * 
	 * Default implementation returns 0 to keep same order.
	 */
	virtual int Compare(const gdeWPSTreeItemModel &item) const;
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** \brief Select object mest matching name. */
	virtual void SelectBestMatching(const char *string);
	/*@}*/
};

#endif
