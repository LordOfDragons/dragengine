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

#ifdef IGDE_TOOLKIT_FOX

#include "foxtoolkit.h"
#include "igdeNativeFoxCommonDialogs.h"
#include "dialog/igdeNativeFoxFileDialog.h"
#include "../../igdeWidget.h"
#include "../../igdeApplication.h"
#include "../../../environment/igdeEnvironment.h"
#include "../../../localization/igdeTranslationManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decGlobalFunctions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeFoxCommonDialogs
/////////////////////////////////////

// Management
///////////////

igdeCommonDialogs::eButton igdeNativeFoxCommonDialogs::Message(igdeWidget &owner,
igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon icon,
const char *title, const char *text){
	DEASSERT_NOTNULL(title)
	DEASSERT_NOTNULL(text)
	DEASSERT_NOTNULL(owner.GetNativeWidget())
	
	int foxButtons;
	
	switch(buttons){
	case igdeCommonDialogs::ebsOk:
		foxButtons = FX::MBOX_OK;
		break;
		
	case igdeCommonDialogs::ebsOkCancel:
		foxButtons = FX::MBOX_OK_CANCEL;
		break;
		
	case igdeCommonDialogs::ebsYesNo:
		foxButtons = FX::MBOX_YES_NO;
		break;
		
	case igdeCommonDialogs::ebsYesNoCancel:
		foxButtons = FX::MBOX_YES_NO_CANCEL;
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	int foxResult;
	
	const decString ttitle(owner.TranslateIf(title).ToUTF8());
	const decString ttext(owner.TranslateIf(text).ToUTF8());
	
	switch(icon){
	case igdeCommonDialogs::eiQuestion:
		if(foxOwner){
			foxResult = FXMessageBox::question(foxOwner, foxButtons, ttitle.GetString(), ttext.GetString(), "");
			
		}else{
			foxResult = FXMessageBox::question(FXApp::instance(), foxButtons, ttitle.GetString(), ttext.GetString(), "");
		}
		break;
		
	case igdeCommonDialogs::eiWarning:
		if(foxOwner){
			foxResult = FXMessageBox::warning(foxOwner, foxButtons, ttitle.GetString(), ttext.GetString(), "");
			
		}else{
			foxResult = FXMessageBox::warning(FXApp::instance(), foxButtons, ttitle.GetString(), ttext.GetString(), "");
		}
		break;
		
	case igdeCommonDialogs::eiError:
		if(foxOwner){
			foxResult = FXMessageBox::error(foxOwner, foxButtons, ttitle.GetString(), ttext.GetString(), "");
			
		}else{
			foxResult = FXMessageBox::error(FXApp::instance(), foxButtons, ttitle.GetString(), ttext.GetString(), "");
		}
		break;
		
	case igdeCommonDialogs::eiInfo:
	case igdeCommonDialogs::eiNone:
		if(foxOwner){
			foxResult = FXMessageBox::information(foxOwner, foxButtons, ttitle.GetString(), ttext.GetString(), "");
			
		}else{
			foxResult = FXMessageBox::information(FXApp::instance(), foxButtons, ttitle.GetString(), ttext.GetString(), "");
		}
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
	
	switch(foxResult){
	case FX::MBOX_CLICKED_OK:
		return igdeCommonDialogs::ebOk;
		
	case FX::MBOX_CLICKED_CANCEL:
		return igdeCommonDialogs::ebCancel;
		
	case FX::MBOX_CLICKED_YES:
		return igdeCommonDialogs::ebYes;
		
	case FX::MBOX_CLICKED_NO:
		return igdeCommonDialogs::ebNo;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void igdeNativeFoxCommonDialogs::FatalError(const char *title, const char *text){
	DEASSERT_NOTNULL(title)
	DEASSERT_NOTNULL(text)
	
	FXMessageBox::error(FXApp::instance(), FX::MBOX_OK, title, text, "");
}



bool igdeNativeFoxCommonDialogs::GetInteger(igdeWidget &owner, const char *title,
const char *text, int &value){
	if(!owner.GetNativeWidget() || !title || !text){
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	const decString ttext(tm.TranslateIf(decUnicodeString::NewFromUTF8(text)).ToUTF8());
	
	FXInputDialog dialog(foxOwner, ttitle.GetString(), ttext.GetString(), nullptr,
		INPUTDIALOG_INTEGER, 0, 0, igdeApplication::app().DisplayScaled(300));
	
	decString initialValue;
	initialValue.Format("%d", value);
	dialog.setText(initialValue.GetString());
	
	if(dialog.execute(PLACEMENT_OWNER)){
		value = decString(dialog.getText().text()).ToInt();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetFloat(igdeWidget &owner, const char *title,
const char *text, float &value){
	if(!owner.GetNativeWidget() || !title || !text){
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	const decString ttext(tm.TranslateIf(decUnicodeString::NewFromUTF8(text)).ToUTF8());
	
	FXInputDialog dialog(foxOwner, ttitle.GetString(), ttext.GetString(), nullptr,
		INPUTDIALOG_REAL, 0, 0, igdeApplication::app().DisplayScaled(300));
	
	decString initialValue;
	initialValue.Format("%g", value);
	dialog.setText(initialValue.GetString());
	
	if(dialog.execute(PLACEMENT_OWNER)){
		value = decString(dialog.getText().text()).ToFloat();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetString(igdeWidget &owner, const char *title,
const char *text, decString &value){
	if(!owner.GetNativeWidget() || !title || !text){
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	const decString ttext(tm.TranslateIf(decUnicodeString::NewFromUTF8(text)).ToUTF8());
	
	FXInputDialog dialog(foxOwner, ttitle.GetString(), ttext.GetString(), nullptr,
		INPUTDIALOG_STRING, 0, 0, igdeApplication::app().DisplayScaled(400));
	
	dialog.setText(value.GetString());
	
	if(dialog.execute(PLACEMENT_OWNER)){
		value = dialog.getText().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::SelectString(igdeWidget &owner, const char *title,
const char *text, const decStringList &list, int &selection){
	if(!owner.GetNativeWidget() || !title || !text || list.GetCount() == 0){
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	const decString ttext(tm.TranslateIf(decUnicodeString::NewFromUTF8(text)).ToUTF8());
	
	FXChoiceBox dialog(foxOwner, ttitle.GetString(), ttext.GetString(), nullptr,
		DEJoin(list, "\n").GetString(), 0, 0, 0, igdeApplication::app().DisplayScaled(400),
		igdeApplication::app().DisplayScaled(400));
	
	//dialog.setCurrentItem( selection );
	// TODO fox does not expose "list" member to set selection. create an own dialog anyways
	
	selection = dialog.execute(PLACEMENT_OWNER);
	return selection != -1;
}



bool igdeNativeFoxCommonDialogs::GetFileOpen(igdeWidget &owner, const char *title,
const igdeFilePattern::List &filePatterns, decString &filename){
	if(!owner.GetNativeWidget() || !title){
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	
	FXFileDialog dialog(foxOwner, ttitle.GetString());
	dialog.setPatternList(igdeUIFoxHelper::FilePatternListToFOX(owner, filePatterns));
	dialog.setCurrentPattern(0);
	dialog.setFilename(filename.GetString());
	
	if(dialog.execute(PLACEMENT_OWNER)){
		filename = dialog.getFilename().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetFileOpen(igdeWidget &owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePattern::List &filePatterns, decString &filename){
	if(!owner.GetNativeWidget() || !title){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const foxOwner = (FXComposite*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	
	igdeNativeFoxFileDialog dialog(owner, &vfs, foxOwner, ttitle.GetString());
	dialog.SetFilePatternList(&filePatterns);
	dialog.SetFilename(filename);
	
	if(dialog.execute(PLACEMENT_OWNER)){
		filename = dialog.GetFilename().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetFileSave(igdeWidget &owner, const char *title,
const igdeFilePattern::List &filePatterns, decString &filename){
	if(!owner.GetNativeWidget() || !title){
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	
	FXFileDialog dialog(foxOwner, ttitle.GetString());
	dialog.setPatternList(igdeUIFoxHelper::FilePatternListToFOX(owner, filePatterns));
	dialog.setCurrentPattern(0);
	dialog.setFilename(filename.GetString());
	
	if(!dialog.execute(PLACEMENT_OWNER)){
		return false;
	}
	
	filename = dialog.getFilename().text();
	
	// TEMP
	if(true){
		const igdeFilePattern &pattern = *filePatterns.GetAt(dialog.getCurrentPattern());
		if(!filename.MatchesPattern(pattern.GetPattern())){
			filename.Append(pattern.GetDefaultExtension());
		}
	}
	// TEMP
	
	return true;
}

bool igdeNativeFoxCommonDialogs::GetFileSave(igdeWidget &owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePattern::List &filePatterns, decString &filename){
	if(!owner.GetNativeWidget() || !title){
		DETHROW(deeInvalidParam);
	}
	
	FXComposite * const foxOwner = (FXComposite*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	
	igdeNativeFoxFileDialog dialog(owner, &vfs, foxOwner, ttitle.GetString());
	dialog.SetFilePatternList(&filePatterns);
	dialog.SetFilename(filename);
	
	if(!dialog.execute(PLACEMENT_OWNER)){
		return false;
	}
	
	filename = dialog.GetFilename().text();
	return true;
}

bool igdeNativeFoxCommonDialogs::GetDirectory(igdeWidget &owner, const char *title, decString &dirname){
	if(!owner.GetNativeWidget() || !title){
		DETHROW(deeInvalidParam);
	}
	
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	
	FXDirDialog dialog(foxOwner, ttitle.GetString());
	dialog.setDirectory(dirname.GetString());
	
	if(dialog.execute(PLACEMENT_OWNER)){
		dirname = dialog.getDirectory().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetDirectory(igdeWidget &owner, const char *title,
deVirtualFileSystem &vfs, decString &dirname){
	if(!owner.GetNativeWidget() || !title){
		DETHROW(deeInvalidParam);
	}
	
	// not supported yet
	DETHROW_INFO(deeInvalidAction, "Directory selection with virtual file system is not supported yet.");
	
	/*
	FXComposite * const foxOwner = (FXComposite*)owner->GetNativeWidget();
	
	igdeDirDialog dialog(owner->GetEnvironment(), &vfs, foxOwner, title);
	dialog.SetDirectory(dirname);
	
	if(dialog.execute(PLACEMENT_OWNER)){
		dirname = dialog.GetDirectory().text();
		return true;
		
	}else{
		return false;
	}
	*/
}

bool igdeNativeFoxCommonDialogs::SelectSystemFont(igdeWidget &owner,
const char *title, igdeFont::sConfiguration &config){
	FXWindow * const foxOwner = (FXWindow*)owner.GetNativeWidget();
	
	const igdeTranslationManager &tm = owner.GetEnvironment().GetTranslationManager();
	const decString ttitle(tm.TranslateIf(decUnicodeString::NewFromUTF8(title)).ToUTF8());
	
	FXFontDialog dialog(foxOwner, ttitle.GetString());
	
	FXFontDesc fdesc;
	memset(&fdesc, '\0', sizeof(FXFontDesc));
	
	#ifdef OS_W32_VS
	strcpy_s(fdesc.face, sizeof(fdesc.face), config.name.GetString());
	#else
	strcpy(fdesc.face, config.name.GetString());
	#endif

	fdesc.size = (int)(config.size * 10 + 0.5f);
	fdesc.weight = config.bold ? FXFont::Bold : FXFont::Normal,
	fdesc.slant = config.italic ? FXFont::Italic : FXFont::Straight;
	dialog.setFontDesc(fdesc);
	
	if(!dialog.execute(PLACEMENT_OWNER)){
		return false;
	}
	
	const FXFontDesc &newFDesc = dialog.getFontDesc();
	config.name = newFDesc.face;
	config.size = (float)newFDesc.size * 0.1f;
	config.bold = newFDesc.weight == FXFont::Bold;
	config.italic = newFDesc.slant == FXFont::Italic;
	return true;
}

#endif
