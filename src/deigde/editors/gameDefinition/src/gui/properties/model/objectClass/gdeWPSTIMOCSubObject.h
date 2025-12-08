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

#ifndef _GDEWPSTIMOCSUBOBJECT_H_
#define _GDEWPSTIMOCSUBOBJECT_H_

#include "../gdeWPSTreeItemModel.h"

class gdeObjectClass;


/**
 * \brief Base class for object class sub objects.
 */
class gdeWPSTIMOCSubObject : public gdeWPSTreeItemModel{
private:
	gdeObjectClass::Ref pObjectClass;
	int pIndex;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMOCSubObject(gdeWPSTreeModel &tree, eTypes type, gdeObjectClass *objectClass, int index);
	
protected:
	/** \brief Clean up tree item model. */
	~gdeWPSTIMOCSubObject() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Object class. */
	inline gdeObjectClass *GetObjectClass() const{ return pObjectClass; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Verify if component is valid. */
	virtual bool IsValid() const;
	
	/** \brief Validate due to change in object class names or existance. */
	virtual void ValidateObjectClassName();
	
	
	
	/**
	 * \brief Compare this item with another for sorting.
	 * 
	 * Default implementation returns 0 to keep same order.
	 */
	int Compare(const gdeWPSTreeItemModel &item) const override;
	
	/** \brief Select object mest matching name. */
	void SelectBestMatching(const char *string) override;
	/*@}*/
};

#endif
