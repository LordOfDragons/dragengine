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

#ifndef _IGDENATIVEFOXSLIDER_H_
#define _IGDENATIVEFOXSLIDER_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFontReference.h"

class igdeSlider;
class igdeEnvironment;
class igdeGuiTheme;
class igdeWidget;


/**
 * FOX Native slider.
 */
class igdeNativeFoxSlider : public FXSlider{
	FXDECLARE( igdeNativeFoxSlider )
	
protected:
	igdeNativeFoxSlider();
	
public:
	enum eFoxIDs{
		ID_SELF = FXSlider::ID_LAST,
	};
	
private:
	igdeSlider *pOwner;
	float pScale;
	float pInvScale;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxSlider( igdeSlider &owner, FXComposite *parent, int layoutFlags );
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxSlider();
	
	/** \brief Create native widget. */
	static igdeNativeFoxSlider* CreateNativeWidget( igdeSlider &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void Focus();
	virtual void UpdateScale();
	virtual void UpdateRange();
	virtual void UpdateValue();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	
	static int SliderFlags( const igdeSlider &owner );
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand( FXObject*, FXSelector, void* );
	long onChanged( FXObject*, FXSelector, void* );
	/*@}*/
};

typedef igdeNativeFoxSlider igdeNativeSlider;

#endif
