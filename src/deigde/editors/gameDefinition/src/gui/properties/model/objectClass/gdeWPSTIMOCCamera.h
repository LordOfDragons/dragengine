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

#ifndef _GDEWPSTIMOCCAMERA_H_
#define _GDEWPSTIMOCCAMERA_H_

#include "gdeWPSTIMOCSubObject.h"

class gdeOCCamera;


/**
 * \brief Object class camera.
 */
class gdeWPSTIMOCCamera : public gdeWPSTIMOCSubObject{
private:
	gdeOCCamera::Ref pCamera;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMOCCamera(gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
		gdeOCCamera::Ref camera, int index);
	
protected:
	/** \brief Clean up tree item model. */
	~gdeWPSTIMOCCamera() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Camera. */
	inline gdeOCCamera *GetOCCamera() const{ return pCamera; }
	
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
