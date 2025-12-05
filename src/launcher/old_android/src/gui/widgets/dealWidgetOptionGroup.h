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

#ifndef _DEALWIDGETOPTIONGROUP_H_
#define _DEALWIDGETOPTIONGROUP_H_

#include "../../deObject.h"
#include "../../common/collection/decPointerSet.h"

class dealWidgetOptionBox;


/**
 * \brief Widget option box group.
 */
class dealWidgetOptionGroup : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dealWidgetOptionGroup> Ref;


private:
	decPointerSet pWidgets;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create option group. */
	dealWidgetOptionGroup();
	
	/** \brief Clean up option group. */
	virtual ~dealWidgetOptionGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of widgets. */
	int GetCount() const;
	
	/** \brief Widget at index. */
	dealWidgetOptionBox *GetAt( int index ) const;
	
	/** \brief Add widget. */
	void Add( dealWidgetOptionBox *widget );
	
	/** \brief Remove widget. */
	void Remove( dealWidgetOptionBox *widget );
	
	/** \brief Remove all widgets. */
	void RemoveAll();
	
	/** \brief Selected option or \em NULL if none in the group is selected. */
	dealWidgetOptionBox *GetSelected() const;
	
	/** \brief Select option and deselect all others or \em NULL to deselect all. */
	void Select( dealWidgetOptionBox *option );
	/*@}*/
};

#endif
