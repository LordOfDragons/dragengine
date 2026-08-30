/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifdef IGDE_TOOLKIT_BEOS

#include "igdeNativeBeosFileDialog.h"
#include "igdeNativeBeosVFSList.h"
#include "igdeNativeBeosVFSDirectoryBox.h"
#include "../../../filedialog/igdeFilePattern.h"
#include "../../../igdeWidget.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>

#include <app/Application.h>
#include <layout/GridLayoutBuilder.h>
#include <layout/GroupLayoutBuilder.h>
#include <interface/Separator.h>
#include <interface/StringView.h>


// Class igdeNativeBeosFileDialog
///////////////////////////////////

igdeNativeBeosFileDialog::igdeNativeBeosFileDialog(igdeWidget &owner,
	deVirtualFileSystem *vfs, const char *name) :
BWindow(BRect(100, 100, 600, 500), name ? name : "Select File",
	B_TITLED_WINDOW, B_ASYNCHRONOUS_CLOSE | B_AUTO_UPDATE_B_RECT),
pOwner(&owner),
pVFS(vfs),
pDirectoryBox(nullptr),
pFileList(nullptr),
pEditFilename(nullptr),
pCBFilter(nullptr),
pBtnOK(nullptr),
pBtnCancel(nullptr),
pResult(B_CANCEL)
{
	DEASSERT_NOTNULL(vfs)
	
	pCreateUI();
	
	// Set initial directory to root
	decPath rootPath;
	rootPath.SetPrefix("/");
	pDirectoryBox->SetPath(rootPath);
	pFileList->SetPath(rootPath);
}

igdeNativeBeosFileDialog::~igdeNativeBeosFileDialog() = default;


// Management
//////////////

decString igdeNativeBeosFileDialog::GetFilename() const {
	if(pEditFilename){
		return decString(pEditFilename->Text());
	}
	return pSelectedFile;
}

void igdeNativeBeosFileDialog::SetFilename(const char *filename){
	DEASSERT_NOTNULL(filename)
	
	auto path = decPath::CreatePathUnix(filename);
	decPath directory;
	pFindExistingDirectory(path, directory);
	directory.SetPrefix("/");
	
	pDirectoryBox->SetPath(directory);
	pFileList->SetPath(directory);
	
	if(pEditFilename){
		pEditFilename->SetText(path.GetPathUnix().GetString());
	}
	pSelectedFile = path.GetPathUnix();
}

void igdeNativeBeosFileDialog::SetFilename(const char *filename, const char *basePath){
	DEASSERT_NOTNULL(filename)
	DEASSERT_NOTNULL(basePath)
	
	if(decPath::IsUnixPathAbsolute(filename)){
		SetFilename(filename);
		return;
	}
	
	decPath absolutePath;
	try{
		absolutePath.SetFromUnix(basePath);
		absolutePath.AddUnixPath(filename);
		
	}catch(const deException &){
		absolutePath.SetFromUnix("/");
	}
	
	SetFilename(absolutePath.GetPathUnix());
}

void igdeNativeBeosFileDialog::SetFilePatternList(const decPointerList &list){
	pFilePatternList = list;
	UpdateFileTypeBox();
}

void igdeNativeBeosFileDialog::UpdateFileTypeBox(){
	if(!pCBFilter){
		return;
	}
	
	auto menu = pCBFilter->Menu();
	if(!menu){
		return;
	}
	
	// Clear existing items
	while(menu->CountItems() > 0){
		delete menu->RemoveItem((int32)0);
	}
	
	// Add file pattern items
	pFilePatternList.Visit([](const igdeFilePattern &fp, void *userData){
		auto menu = (BMenu*)userData;
		menu->AddItem(new BMenuItem(decString::Formatted("{0} ({1})",
			fp.GetName(), fp.GetPattern()).GetString(), new BMessage(ID_CB_FILTER)));
	}, menu);
	
	if(pCBFilter->Menu()->CountItems() == 0){
		pCBFilter->Menu()->AddItem(new BMenuItem("All Files (*)", new BMessage(ID_CB_FILTER)));
	}
	
	if(pCBFilter->Menu()->CountItems() > 0){
		pCBFilter->Menu()->ItemAt(0)->SetMarked(true);
	}
}

int32 igdeNativeBeosFileDialog::ShowModal(){
	// Center window on screen
	BScreen screen(this);
	if(screen.IsValid()){
		BRect frame = screen.Frame();
		BRect winFrame = Frame();
		MoveTo(
			(frame.Width() - winFrame.Width()) / 2,
			(frame.Height() - winFrame.Height()) / 2
		);
	}
	
	Show();
	
	// Run modal loop
	thread_id windowThread = spawn_thread(
		[](void *arg) -> int32 {
			BWindow *window = (BWindow*)arg;
			BLooper::LooperForThread(find_thread(nullptr))->Run();
			return 0;
		}, "file_dialog_thread", B_NORMAL_PRIORITY, this);
	
	// Actually, use synchronous approach with a semaphore
	sem_id semaphore = create_sem(0, "dialog_sem");
	
	// Wait for dialog to complete
	if(acquire_sem(semaphore) == B_OK){
		delete_sem(semaphore);
	}
	
	return pResult;
}

bool igdeNativeBeosFileDialog::QuitRequested(){
	pResult = B_CANCEL;
	return true;
}

void igdeNativeBeosFileDialog::MessageReceived(BMessage *message){
	switch(message->what){
	case ID_BTN_OK:
		pHandleOK();
		break;
		
	case ID_BTN_CANCEL:
		pHandleCancel();
		break;
		
	case ID_LIST_DOUBLE_CLICK:
		pHandleFileListDoubleClick();
		break;
		
	case ID_CB_FILTER:
		pUpdateFromDirectory();
		break;
		
	default:
		BWindow::MessageReceived(message);
	}
}


// Private
///////////

void igdeNativeBeosFileDialog::pCreateUI(){
	BView *mainView = new BView(Bounds(), "main", B_FOLLOW_ALL, B_WILL_DRAW);
	mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(mainView);
	
	// Create directory box
	BRect dirBoxRect(10, 10, 580, 40);
	pDirectoryBox = new igdeNativeBeosVFSDirectoryBox(pVFS, dirBoxRect, "dir_box");
	mainView->AddChild(pDirectoryBox);
	
	// Create file list
	BRect listRect(10, 50, 580, 350);
	pFileList = new igdeNativeBeosVFSList(*pOwner->GetEnvironment(), pVFS, listRect, "file_list");
	mainView->AddChild(pFileList->GetScrollView());
	
	// Create filename input
	BRect filenameRect(10, 360, 580, 390);
	pEditFilename = new BTextControl(filenameRect, "filename", "Filename:", "", 
		new BMessage(ID_TEXT_FILENAME), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	mainView->AddChild(pEditFilename);
	
	// Create filter combo box
	BMenu *filterMenu = new BMenu("All Files");
	filterMenu->AddItem(new BMenuItem("All Files (*)", new BMessage(ID_CB_FILTER)));
	BRect filterRect(10, 395, 300, 415);
	pCBFilter = new BMenuField(filterRect, "filter", "Filter:", filterMenu, 
		B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	mainView->AddChild(pCBFilter);
	
	// Create buttons
	BRect okRect(350, 395, 440, 415);
	pBtnOK = new BButton(okRect, "ok", "OK", new BMessage(ID_BTN_OK),
		B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	mainView->AddChild(pBtnOK);
	
	BRect cancelRect(450, 395, 540, 415);
	pBtnCancel = new BButton(cancelRect, "cancel", "Cancel", new BMessage(ID_BTN_CANCEL),
		B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	mainView->AddChild(pBtnCancel);
	
	// Set targets
	pBtnOK->SetTarget(this);
	pBtnCancel->SetTarget(this);
}

void igdeNativeBeosFileDialog::pUpdateFromDirectory(){
	if(pDirectoryBox){
		pDirectoryBox->SetPathFromText();
		if(pFileList){
			pFileList->SetPath(pDirectoryBox->GetPath());
		}
	}
}

void igdeNativeBeosFileDialog::pHandleOK(){
	if(pEditFilename){
		pSelectedFile = pEditFilename->Text();
	}
	pResult = B_OK;
	PostMessage(B_QUIT_REQUESTED);
}

void igdeNativeBeosFileDialog::pHandleCancel(){
	pResult = B_CANCEL;
	PostMessage(B_QUIT_REQUESTED);
}

void igdeNativeBeosFileDialog::pHandleFileListDoubleClick(){
	if(!pFileList){
		return;
	}
	
	igdeNativeBeosVFSListItem * const item = pFileList->GetSelectedItem();
	if(!item){
		return;
	}
	
	if(item->IsDirectory()){
		pFileList->SetPath(item->GetPath());
		if(pDirectoryBox){
			pDirectoryBox->SetPath(item->GetPath());
		}
	} else {
		if(pEditFilename){
			pEditFilename->SetText(item->GetFileName().GetString());
		}
		pHandleOK();
	}
}

void igdeNativeBeosFileDialog::pFindExistingDirectory(const decPath &path, decPath &directory){
	directory.SetEmpty();
	
	// Try to find an existing parent directory
	decPath checkPath = path;
	while(checkPath.GetComponentCount() > 0){
		try {
			if(pVFS->ExistsFile(checkPath)){
				directory = checkPath;
				directory.RemoveLastComponent();
				directory.SetPrefix("/");
				return;
			}
			
		} catch(...){
		}
		checkPath.RemoveLastComponent();
	}
	
	// Default to root
	directory.SetPrefix("/");
}

#endif
