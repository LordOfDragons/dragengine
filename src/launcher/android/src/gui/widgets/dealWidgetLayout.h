/* 
 * Drag[en]gine Android Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	dealWidgetLayout( dealDisplay &display );
	
	/** \brief Create widget. */
	dealWidgetLayout( dealDisplay &display, const decPoint &position, const decPoint &size );
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLayout();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of widgets. */
	int GetWidgetCount() const;
	
	/** \brief Widget at index. */
	dealWidget *GetWidgetAt( int index ) const;
	
	/** \brief Add widget. */
	void AddWidget( dealWidget *widget );
	
	/** \brief Remove widget. */
	void RemoveWidget( dealWidget *widget );
	
	/** \brief Remove all widgets. */
	void RemoveAllWidgets();
	
	
	
	/** \brief Widget position in dialog coordinates. */
	virtual decPoint GetDialogPosition() const;
	
	/** \brief Widget containing position or \em null if not found. */
	virtual dealWidget *WidgetAtPosition( const decPoint &point ) const;
	
	
	
	/** \brief Layout is dirty. */
	inline bool GetLayoutDirty() const{ return pDirtyLayout; }
	
	/** \brief Dirty layout. */
	void DirtyLayout();
	
	/** \brief Layout child widgets if dirty. */
	void Layout();
	
	/** \brief Layout child widgets. */
	virtual void LayoutWidgets();
	
	
	
	/** \brief Render content. */
	virtual void RenderContent( const sRenderContext &context );
	
	
	
	/** \brief Position changed. */
	virtual void OnPositionChanged();
	
	/** \brief Size changed. */
	virtual void OnSizeChanged();
	
	/** \brief Padding changed. */
	virtual void OnPaddingChanged();
	/*@}*/
};

#endif
