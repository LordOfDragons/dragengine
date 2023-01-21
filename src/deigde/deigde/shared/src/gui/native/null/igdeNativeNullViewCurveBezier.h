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

#ifndef _IGDENATIVENULLVIEWCURVEBEZIER_H_
#define _IGDENATIVENULLVIEWCURVEBEZIER_H_

class igdeViewCurveBezier;


/**
 * Null list box view.
 */
class igdeNativeFoxViewCurveBezierView{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxViewCurveBezierView();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxViewCurveBezierView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateEnabled();
	virtual void UpdateDefaultSize();
	virtual void UpdateCurve();
	virtual void UpdateSelectedPoint();
	virtual void UpdateClamp();
	
	virtual void ResetView();
	virtual void FitViewToCurve();
	/*@}*/
};


/**
 * Null view curve bezier.
 */
class igdeNativeNullViewCurveBezier{
private:
	igdeNativeFoxViewCurveBezierView *pView;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullViewCurveBezier();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullViewCurveBezier();
	
	/** \brief Create native widget. */
	static igdeNativeNullViewCurveBezier* CreateNativeWidget( igdeViewCurveBezier &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	
	/** \brief View curve bezier. */
	inline igdeNativeFoxViewCurveBezierView &GetView() const{ return *pView; }
	/*@}*/
};

typedef igdeNativeNullViewCurveBezier igdeNativeViewCurveBezier;

#endif
