/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDENATIVEFOXSPACER_H_
#define _IGDENATIVEFOXSPACER_H_

#include "foxtoolkit.h"


/**
 * \brief FOX toolkit Native Spacer.
 */
class igdeNativeFoxSpacer : public FXFrame{
	FXDECLARE( igdeNativeFoxSpacer )
protected:
	igdeNativeFoxSpacer();
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container. */
	igdeNativeFoxSpacer( FXComposite *parent, int width, int height, int childFlags );
	
	/** \brief Clean up container. */
	virtual ~igdeNativeFoxSpacer();
	
	/** \brief Set size. */
	virtual void SetSize( int width, int height );
	
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	
	/** \brief Process layout flags. */
	static int LayoutFlags( int childFlags );
	/*@}*/
	
private:
	int pWidth;
	int pHeight;
};

#endif
