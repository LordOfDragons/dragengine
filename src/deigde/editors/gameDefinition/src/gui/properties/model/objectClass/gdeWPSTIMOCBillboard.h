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

#ifndef _GDEWPSTIMOCBILLBOARD_H_
#define _GDEWPSTIMOCBILLBOARD_H_

#include "gdeWPSTIMOCSubObject.h"

class gdeOCBillboard;


/**
 * \brief Object class billboard.
 */
class gdeWPSTIMOCBillboard : public gdeWPSTIMOCSubObject{
private:
	gdeOCBillboard::Ref pBillboard;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMOCBillboard(gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
		gdeOCBillboard *billboard, int index);
	
protected:
	/** \brief Clean up tree item model. */
	~gdeWPSTIMOCBillboard() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Billboard. */
	inline const gdeOCBillboard::Ref &GetOCBillboard() const{ return pBillboard; }
	
	/** \brief Validate and update state accordingly. */
	void Validate();
	
	/** \brief Verify if valid. */
	bool IsValid() const override;
	
	
	
	/** \brief Added to tree. */
	void OnAddedToTree() override;
	
	/** \brief User selected item. */
	void OnSelected() override;
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	/*@}*/
};

#endif
