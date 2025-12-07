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

#ifndef _DEALWIDGETLAYOUT_H_
#define _DEALWIDGETLAYOUT_H_

#include "dealWidget.h"
#include "../../common/collection/decObjectOrderedSet.h"



/**
 * \brief Widget layout.
 */
class dealWidgetLayout : public dealWidget{
private:
	decObjectOrderedSet pWidgets;
	bool pDirtyLayout;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLayout(dealDisplay &display);
	
	/** \brief Create widget. */
	dealWidgetLayout(dealDisplay &display, const decPoint &position, const decPoint &size);
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLayout();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of widgets. */
	int GetWidgetCount() const;
	
	/** \brief Widget at index. */
	dealWidget *GetWidgetAt(int index) const;
	
	/** \brief Add widget. */
	void AddWidget(dealWidget *widget);
	
	/** \brief Remove widget. */
	void RemoveWidget(dealWidget *widget);
	
	/** \brief Remove all widgets. */
	void RemoveAllWidgets();
	
	
	
	/** \brief Widget position in dialog coordinates. */
	virtual decPoint GetDialogPosition() const;
	
	/** \brief Widget containing position or \em null if not found. */
	virtual dealWidget *WidgetAtPosition(const decPoint &point) const;
	
	
	
	/** \brief Layout is dirty. */
	inline bool GetLayoutDirty() const{ return pDirtyLayout; }
	
	/** \brief Dirty layout. */
	void DirtyLayout();
	
	/** \brief Layout child widgets if dirty. */
	void Layout();
	
	/** \brief Layout child widgets. */
	virtual void LayoutWidgets();
	
	
	
	/** \brief Render content. */
	virtual void RenderContent(const sRenderContext &context);
	
	
	
	/** \brief Position changed. */
	virtual void OnPositionChanged();
	
	/** \brief Size changed. */
	virtual void OnSizeChanged();
	
	/** \brief Padding changed. */
	virtual void OnPaddingChanged();
	/*@}*/
};

#endif
