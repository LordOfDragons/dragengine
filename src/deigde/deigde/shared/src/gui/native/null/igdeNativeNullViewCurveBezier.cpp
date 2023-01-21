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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullViewCurveBezier.h"


// class igdeNativeFoxViewCurveBezierView
///////////////////////////////////////////

igdeNativeFoxViewCurveBezierView::igdeNativeFoxViewCurveBezierView(){
}

igdeNativeFoxViewCurveBezierView::~igdeNativeFoxViewCurveBezierView(){
}

void igdeNativeFoxViewCurveBezierView::UpdateEnabled(){
}

void igdeNativeFoxViewCurveBezierView::UpdateDefaultSize(){
}

void igdeNativeFoxViewCurveBezierView::UpdateCurve(){
}

void igdeNativeFoxViewCurveBezierView::UpdateSelectedPoint(){
}

void igdeNativeFoxViewCurveBezierView::UpdateClamp(){
}

void igdeNativeFoxViewCurveBezierView::ResetView(){
}

void igdeNativeFoxViewCurveBezierView::FitViewToCurve(){
}



// class igdeNativeNullViewCurveBezier
////////////////////////////////////////

igdeNativeNullViewCurveBezier::igdeNativeNullViewCurveBezier() :
pView( new igdeNativeFoxViewCurveBezierView ){
}

igdeNativeNullViewCurveBezier::~igdeNativeNullViewCurveBezier(){
	delete pView;
}

igdeNativeNullViewCurveBezier *igdeNativeNullViewCurveBezier::CreateNativeWidget( igdeViewCurveBezier& ){
	return new igdeNativeNullViewCurveBezier;
}

void igdeNativeNullViewCurveBezier::PostCreateNativeWidget(){
}

void igdeNativeNullViewCurveBezier::DestroyNativeWidget(){
	delete this;
}

#endif
