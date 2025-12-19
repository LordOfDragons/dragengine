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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeRecentFiles.h"
#include "../environment/igdeEnvironment.h"
#include "../gui/igdeEditorWindow.h"
#include "../gui/igdeUIHelper.h"
#include "../gui/event/igdeAction.h"
#include "../gui/menu/igdeMenuCascade.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Events
///////////

namespace {

class igdeRecentFiles_ActionOpenFile : public igdeAction{
	igdeRecentFiles &pRecentFiles;
	decString pFilename;
	
public:
	typedef deTObjectReference<igdeRecentFiles_ActionOpenFile> Ref;
	
	igdeRecentFiles_ActionOpenFile(igdeRecentFiles &recentFiles, const char *filename) :
	pRecentFiles(recentFiles), pFilename(filename){
		/*
		decPath path;
		
		if(recentFiles.GetUnixPath()){
			path.SetFromUnix(filename);
			
		}else{
			path.SetFromNative(filename);
		}
		
		SetText(path.GetLastComponent());
		*/
		
		SetText(filename);
		SetDescription(decString("Load ") + filename);
	}
	
	void OnAction() override{
		const Ref guard(this);
		pRecentFiles.OpenFile(pFilename);
	}
};

class igdeRecentFiles_ActionClear : public igdeAction{
	igdeRecentFiles &pRecentFiles;
	
public:
	typedef deTObjectReference<igdeRecentFiles_ActionClear> Ref;
	
	igdeRecentFiles_ActionClear(igdeRecentFiles &recentFiles) : igdeAction("Clear List",
	recentFiles.GetEnvironment().GetStockIcon(igdeEnvironment::esiDelete),
	"Clear List"), pRecentFiles(recentFiles){
	}
	
	void OnAction() override{
		pRecentFiles.RemoveAllFiles();
	}
	
	void Update() override{
		SetEnabled(pRecentFiles.GetFiles().GetCount() > 0);
	}
};

}


// Class igdeRecentFiles
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

igdeRecentFiles::igdeRecentFiles(igdeEnvironment &environment, bool unixPath, int size) :
pEnvironment(environment),
pEditorWindow(nullptr),
pSize(0),
pUnixPath(unixPath)
{
	SetSize(size);
}

igdeRecentFiles::igdeRecentFiles(igdeEditorWindow &editorWindow, bool unixPath, int size) :
pEnvironment(editorWindow.GetEnvironment()),
pEditorWindow(&editorWindow),
pSize(0),
pUnixPath(unixPath)
{
	SetSize(size);
}

igdeRecentFiles::~igdeRecentFiles(){
}



// Management
///////////////

void igdeRecentFiles::SetSize(int size){
	if(size < 0){
		DETHROW(deeInvalidParam);
	}
	if(size == pSize){
		return;
	}
	
	pSize = size;
	EnsureSize();
	UpdateMenu();
}

void igdeRecentFiles::SetFiles(const decStringList &files){
	if(files == pFiles){
		return;
	}
	
	pFiles = files;
	EnsureSize();
	UpdateMenu();
	FilesChanged();
}

void igdeRecentFiles::SetMenu(igdeMenuCascade *menu){
	if(menu == pMenu){
		return;
	}
	
	if(pMenu){
		pMenu->RemoveAllChildren();
	}
	
	pMenu = menu;
	
	UpdateMenu();
}

void igdeRecentFiles::AddFile(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	if(pUnixPath){
		if(decPath::CreatePathUnix(filename).GetComponentCount() == 0){
			DETHROW(deeInvalidParam);
		}
		
	}else{
		if(decPath::CreatePathNative(filename).GetComponentCount() == 0){
			DETHROW(deeInvalidParam);
		}
	}
	
	const int index = pFiles.IndexOf(filename);
	if(index == 0){
		return;
	}
	
	if(index != -1){
		pFiles.RemoveFrom(index);
	}
	pFiles.Insert(filename, 0);
	EnsureSize();
	UpdateMenu();
	FilesChanged();
	
	// add to environment recent editor files if attached to an editor window
	if(pEditorWindow){
		pEnvironment.AddRecentEditorFile(filename);
	}
}

void igdeRecentFiles::RemoveAllFiles(){
	if(pFiles.GetCount() == 0){
		return;
	}
	
	pFiles.RemoveAll();
	UpdateMenu();
	FilesChanged();
}

void igdeRecentFiles::ReadFromXml(const decXmlElementTag &root){
	const int count = root.GetElementCount();
	int i;
	
	pFiles.RemoveAll();
	
	const decXmlAttValue * const attributeSize = root.FindAttribute("size");
	if(attributeSize){
		SetSize(attributeSize->GetValue().ToInt());
	}
	
	for(i=0; i<count; i++){
		const decXmlElementTag * const tag = root.GetElementIfTag(i);
		if(!tag){
			continue;
		}
		
		if(tag->GetName() == "filename"){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if(!cdata){
				DETHROW(deeInvalidParam);
			}
			pFiles.Add(cdata->GetData());
			
		}else{
			DETHROW(deeInvalidParam);
		}
	}
	
	EnsureSize();
	UpdateMenu();
}

void igdeRecentFiles::WriteToXml(decXmlWriter &writer, const char *tagName) const{
	const int count = pFiles.GetCount();
	int i;
	
	writer.WriteOpeningTagStart(tagName);
	writer.WriteAttributeInt("size", pSize);
	writer.WriteOpeningTagEnd();
	
	for(i=0; i<count; i++){
		writer.WriteDataTagString("filename", pFiles.GetAt(i));
	}
	
	writer.WriteClosingTag(tagName);
}



void igdeRecentFiles::OpenFile(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	if(pEditorWindow){
		pEditorWindow->LoadDocument(filename);
	}
}

void igdeRecentFiles::UpdateMenu(){
	if(!pMenu){
		return;
	}
	
	pMenu->RemoveAllChildren();
	
	igdeUIHelper &helper = pMenu->GetEnvironment().GetUIHelper();
	const int count = pFiles.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		helper.MenuCommand(pMenu, igdeRecentFiles_ActionOpenFile::Ref::New(*this, pFiles.GetAt(i)));
	}
	
	helper.MenuSeparator(pMenu);
	helper.MenuCommand(pMenu, igdeRecentFiles_ActionClear::Ref::New(*this));
}

void igdeRecentFiles::FilesChanged(){
	if(pEditorWindow){
		pEditorWindow->RecentFilesChanged();
	}
}



// Protected Functions
////////////////////////

void igdeRecentFiles::EnsureSize(){
	while(pSize < pFiles.GetCount()){
		pFiles.RemoveFrom(pFiles.GetCount() - 1);
	}
}
