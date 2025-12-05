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

#ifndef _IGDECONTAINERBORDER_H_
#define _IGDECONTAINERBORDER_H_

#include "../igdeContainer.h"



/**
 * \brief IGDE UI Container Widget grouping widgets in border layout.
 * 
 * The layout contains 5 areas: top, left, right, bottom and center. Each area can contain
 * at most one widget. Thus in total at most 5 Widgets can be added to this layout. The
 * widgets in the side areas are stretched sidewards to close gaps with the center oriented
 * size being set to the desired widget size. The center widget is stretched in both axis
 * to fill the available space. The top and bottom sides stretch along the entire widget
 * size. The left and right areas touch up to the top and bottom widget. Hence the top and
 * bottom sides have priority over the left and side side.
 */
class DE_DLL_EXPORT igdeContainerBorder : public igdeContainer{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeContainerBorder> Ref;
	
	
	/** \brief Area. */
	enum eArea{
		/** \brief Top side. */
		eaTop,
		
		/** \brief Bottom side. */
		eaBottom,
		
		/** \brief Left side. */
		eaLeft,
		
		/** \brief Right side. */
		eaRight,
		
		/** \brief Center. */
		eaCenter
	};
	
	
	
private:
	int pSpacing;
	igdeWidget *pWidgetTop;
	igdeWidget *pWidgetBottom;
	igdeWidget *pWidgetLeft;
	igdeWidget *pWidgetRight;
	igdeWidget *pWidgetCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeContainerBorder( igdeEnvironment &environment, int spacing = 0 );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeContainerBorder();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Spacing. */
	inline int GetSpacing() const{ return pSpacing; }
	
	/** \brief Add child to area. */
	virtual void AddChild( igdeWidget *child, eArea area );
	
	/** \brief Add child not possible without specifying an area. */
	virtual void AddChild( igdeWidget *child );
	
	/** \brief Remove child. */
	virtual void RemoveChild( igdeWidget *child );
	
	/** \brief Remove all children. */
	virtual void RemoveAllChildren();
	
	/** \brief Widget in area or NULL. */
	igdeWidget *GetWidgetIn( eArea area ) const;
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	/*@}*/
};

#endif
