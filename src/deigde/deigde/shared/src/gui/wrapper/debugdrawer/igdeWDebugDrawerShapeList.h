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

#ifndef _IGDEWDEBUGDRAWERSHAPELIST_H_
#define _IGDEWDEBUGDRAWERSHAPELIST_H_

#include <dragengine/common/collection/decPointerSet.h>

class deDebugDrawer;
class igdeWDebugDrawerShape;


/**
 * \brief Debug drawer shape wrapper List.
 */
class DE_DLL_EXPORT igdeWDebugDrawerShapeList{
private:
	decPointerSet pList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeWDebugDrawerShapeList();
	
	/** \brief Clean up list. */
	~igdeWDebugDrawerShapeList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of wrappers. */
	int GetCount() const;
	
	/** \brief Wrapper by index. */
	igdeWDebugDrawerShape *GetAt(int index) const;
	
	/** \brief Determines if an wrapper exists. */
	bool Has(igdeWDebugDrawerShape *wrapper) const;
	
	/** \brief Adds a wrapper. */
	void Add(igdeWDebugDrawerShape *wrapper);
	
	/** \brief Removes a wrapper. */
	void Remove(igdeWDebugDrawerShape *wrapper);
	
	/** \brief Removes all wrappers. */
	void RemoveAll();
	
	/** \brief Set parent debug drawer or nullptr for all wrappers. */
	void SetParentDebugDrawerAll(deDebugDrawer *debugDrawer);
	
	/** \brief Set visible for all wrappers. */
	void SetVisibleAll(bool visible);
	/*@}*/
};

#endif
