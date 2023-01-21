/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _IGDENATIVENULLRENDERVIEW_H_
#define _IGDENATIVENULLRENDERVIEW_H_

#include <dragengine/common/math/decMath.h>

class igdeViewRenderWindow;


/**
 * Null Render View.
 */
class igdeNativeNullRenderView{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	igdeNativeNullRenderView();
	
	/** \brief Clean up widget. */
	virtual ~igdeNativeNullRenderView();
	
	/** \brief Create native widget. */
	static igdeNativeNullRenderView* CreateNativeWidget( igdeViewRenderWindow &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline bool GetCanRender() const{ return true; }
	inline bool GetRenderWindowAttached() const{ return true; }
	
	virtual void DropNativeWindow();
	virtual bool IsReallyVisible() const;
	virtual bool IsShown() const;
	virtual decPoint GetSize() const;
	virtual void OnFrameUpdate();
	virtual void AttachRenderWindow();
	virtual void DetachRenderWindow();
	virtual void GrabInput();
	virtual void ReleaseInput();
	/*@}*/
};

typedef igdeNativeNullRenderView igdeNativeRenderView;

#endif
