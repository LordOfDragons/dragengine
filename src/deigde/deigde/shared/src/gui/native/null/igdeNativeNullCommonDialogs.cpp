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

#ifdef IGDE_TOOLKIT_NULL

#include "igdeNativeNullCommonDialogs.h"


// Class igdeNativeNullCommonDialogs
/////////////////////////////////////

// Management
///////////////

igdeCommonDialogs::eButton igdeNativeNullCommonDialogs::Message(igdeWidget*,
igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon, const char *, const char *){
	switch(buttons){
	case igdeCommonDialogs::ebsOk:
		return igdeCommonDialogs::ebOk;
		
	case igdeCommonDialogs::ebsOkCancel:
		return igdeCommonDialogs::ebCancel;
		
	case igdeCommonDialogs::ebsYesNo:
		return igdeCommonDialogs::ebNo;
		
	case igdeCommonDialogs::ebsYesNoCancel:
		return igdeCommonDialogs::ebCancel;
		
	default:
		DETHROW(deeInvalidParam);
	}
}



bool igdeNativeNullCommonDialogs::GetInteger(igdeWidget*, const char *, const char *, int&){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFloat(igdeWidget*, const char *, const char *, float&){
	return false;
}

bool igdeNativeNullCommonDialogs::GetString(igdeWidget*, const char *, const char *, decString&){
	return false;
}

bool igdeNativeNullCommonDialogs::SelectString(igdeWidget*, const char *, const char *, const decStringList &, int&){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileOpen(igdeWidget*, const char *, const igdeFilePatternList &, decString &){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileOpen(igdeWidget*, const char *, deVirtualFileSystem &,
const igdeFilePatternList &, decString &){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileSave(igdeWidget*, const char *, const igdeFilePatternList &, decString &){
	return false;
}

bool igdeNativeNullCommonDialogs::GetFileSave(igdeWidget*, const char *, deVirtualFileSystem &,
const igdeFilePatternList &, decString &){
	return false;
}

bool igdeNativeNullCommonDialogs::GetDirectory(igdeWidget*, const char *, decString&){
	return false;
}

bool igdeNativeNullCommonDialogs::GetDirectory(igdeWidget*, const char *, deVirtualFileSystem &, decString &){
	return false;
}

bool igdeNativeNullCommonDialogs::SelectSystemFont(igdeWidget*, const char *, igdeFont::sConfiguration &){
	return false;
}

#endif
