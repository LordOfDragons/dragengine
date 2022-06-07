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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullCommonDialogs.h"


// Class igdeNativeNullCommonDialogs
/////////////////////////////////////

// Management
///////////////

igdeCommonDialogs::eButton igdeNativeNullCommonDialogs::Message( igdeWidget*,
igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon, const char *, const char * ){
	switch( buttons ){
	case igdeCommonDialogs::ebsOk:
		return igdeCommonDialogs::ebOk;
		
	case igdeCommonDialogs::ebsOkCancel:
		return igdeCommonDialogs::ebCancel;
		
	case igdeCommonDialogs::ebsYesNo:
		return igdeCommonDialogs::ebNo;
		
	case igdeCommonDialogs::ebsYesNoCancel:
		return igdeCommonDialogs::ebCancel;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



bool igdeNativeNullCommonDialogs::GetInteger( igdeWidget*, const char *, const char *, int& ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFloat( igdeWidget*, const char *, const char *, float& ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetString( igdeWidget*, const char *, const char *, decString& ){
	return false;
}

bool igdeNativeNullCommonDialogs::SelectString( igdeWidget*, const char *, const char *, const decStringList &, int& ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileOpen( igdeWidget*, const char *, const igdeFilePatternList &, decString & ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileOpen( igdeWidget*, const char *, deVirtualFileSystem &,
const igdeFilePatternList &, decString & ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileSave( igdeWidget*, const char *, const igdeFilePatternList &, decString & ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileSave( igdeWidget*, const char *, deVirtualFileSystem &,
const igdeFilePatternList &, decString & ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetDirectory( igdeWidget*, const char *, decString& ){
	return false;
}

bool igdeNativeNullCommonDialogs::GetDirectory( igdeWidget*, const char *, deVirtualFileSystem &, decString & ){
	return false;
}

bool igdeNativeNullCommonDialogs::SelectSystemFont( igdeWidget*, const char *, igdeFont::sConfiguration & ){
	return false;
}

#endif
