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

#include "igdeNativeFoxFileDialog.h"
#include "igdeNativeFoxVFSDirectoryBox.h"
#include "igdeNativeFoxVFSList.h"
#include "igdeNativeFoxVFSListItem.h"
#include "../foxIcons.h"
#include "../../../igdeCommonDialogs.h"
#include "../../../igdeWidget.h"
#include "../../../../engine/igdeEngineController.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSContainer.h>



// Events
///////////

FXDEFMAP(igdeNativeFoxFileDialog) igdeNativeFoxFileDialogMap[] = {
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_ACCEPT, igdeNativeFoxFileDialog::onCmdAccept),
	
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_BTNUPDIR, igdeNativeFoxFileDialog::onBtnUpDir),
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_BTNGOROOT, igdeNativeFoxFileDialog::onBtnGoRoot),
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_BTNMKDIR, igdeNativeFoxFileDialog::onBtnMkDir),
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_BTNLISTMODE, igdeNativeFoxFileDialog::onBtnListMode),
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_BTNICONMODE, igdeNativeFoxFileDialog::onBtnIconMode),
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_BTNDETAILMODE, igdeNativeFoxFileDialog::onBtnDetailMode),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxFileDialog::ID_BTNUPDIR, igdeNativeFoxFileDialog::updateBtnUpDir),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxFileDialog::ID_BTNGOROOT, igdeNativeFoxFileDialog::updateBtnGoRoot),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxFileDialog::ID_BTNMKDIR, igdeNativeFoxFileDialog::updateBtnMkDir),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxFileDialog::ID_BTNLISTMODE, igdeNativeFoxFileDialog::updateBtnListMode),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxFileDialog::ID_BTNICONMODE, igdeNativeFoxFileDialog::updateBtnIconMode),
	FXMAPFUNC(SEL_UPDATE, igdeNativeFoxFileDialog::ID_BTNDETAILMODE, igdeNativeFoxFileDialog::updateBtnDetailMode),
	
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_CBDIRECTORY, igdeNativeFoxFileDialog::onCBDirectoryChanged),
	FXMAPFUNC(SEL_COMMAND, igdeNativeFoxFileDialog::ID_CBFILTER, igdeNativeFoxFileDialog::onCBFilterChanged),
	
	FXMAPFUNC(SEL_DOUBLECLICKED, igdeNativeFoxFileDialog::ID_FILELIST, igdeNativeFoxFileDialog::onFLItemDoubleClicked),
	FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, igdeNativeFoxFileDialog::ID_FILELIST, igdeNativeFoxFileDialog::onFLRightMouseUp),
	FXMAPFUNC(SEL_SELECTED, igdeNativeFoxFileDialog::ID_FILELIST, igdeNativeFoxFileDialog::onFLItemSelected),
	FXMAPFUNC(SEL_DESELECTED, igdeNativeFoxFileDialog::ID_FILELIST, igdeNativeFoxFileDialog::onFLItemDeselected),
};



// Class igdeNativeFoxFileDialog
///////////////////////

FXIMPLEMENT(igdeNativeFoxFileDialog, FXDialogBox, igdeNativeFoxFileDialogMap, ARRAYNUMBER(igdeNativeFoxFileDialogMap))

// Constructor, destructor
////////////////////////////

igdeNativeFoxFileDialog::igdeNativeFoxFileDialog(){}

igdeNativeFoxFileDialog::igdeNativeFoxFileDialog(igdeWidget &powner, FXComposite *pparent,
const FXString &name, FXuint opts, FXint x, FXint y, FXint w, FXint h) :
FXDialogBox(pparent, name, opts | DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
x, y, w, h, 0, 0, 0, 0, 4, 4),
pOwner(&powner),
pVFS(powner.GetEnvironment().GetFileSystemGame()),
pFilePatternList(nullptr)
{
	pCreateDialog();
}

igdeNativeFoxFileDialog::igdeNativeFoxFileDialog(igdeWidget &powner, deVirtualFileSystem *vfs,
FXComposite *pparent, const FXString& name, FXuint opts, FXint x, FXint y, FXint w, FXint h) :
FXDialogBox(pparent, name, opts | DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
x, y, w, h, 0, 0, 0, 0, 4, 4),
pOwner(&powner),
pVFS(vfs),
pFilePatternList(nullptr)
{
	if(!vfs){
		DETHROW(deeInvalidParam);
	}
	pCreateDialog();
}

igdeNativeFoxFileDialog::~igdeNativeFoxFileDialog(){
	delete pIconUpDir;
	delete pIconListMode;
	delete pIconIconMode;
	delete pIconDetailMode;
	delete pIconGoRoot;
	delete pIconMkdir;
	delete pIconDelete;
	delete pIconMove;
	delete pIconCopy;
	delete pIconLink;
}



// Management
///////////////

void igdeNativeFoxFileDialog::hide(){
	FXDialogBox::hide();
	
	getApp()->reg().writeIntEntry("File Dialog", "width", getWidth());
	getApp()->reg().writeIntEntry("File Dialog", "height", getHeight());
//	getApp()->reg().writeUnsignedEntry( "File Dialog", "style", getFileBoxStyle() );
//	getApp()->reg().writeUnsignedEntry( "File Dialog", "showhidden", showHiddenFiles() );
}

FXString igdeNativeFoxFileDialog::GetFilename() const{
	return pEditFilename->getText();
}

void igdeNativeFoxFileDialog::SetFilename(const char *filename){
	decPath path(decPath::CreatePathUnix(filename));
	decPath directory;
	pFindExistingDirectory(path, directory);
	directory.SetPrefix("/");
	
	pList->SetPath(directory);
	pCBDirectory->SetPath(directory);
	
	pEditFilename->setText(path.GetPathUnix().GetString());
}

void igdeNativeFoxFileDialog::SetFilename(const char *filename, const char *basePath){
	if(!filename || !basePath){
		DETHROW(deeInvalidParam);
	}
	
	if(decPath::IsUnixPathAbsolute(filename)){
		SetFilename(filename);
		return;
	}
	
	decPath absolutePath;
	
	try{
		absolutePath.SetFromUnix(basePath);
		absolutePath.AddUnixPath(filename);
		
	}catch(const deException &){
		// combined path is invalid. use last path element only
		const decString fn(filename);
		const int deli = fn.FindReverse('/');
		
		if(deli == -1){
			absolutePath.SetFromUnix("/");
			
		}else{
			absolutePath.SetFromUnix(fn.GetMiddle(deli + 1));
		}
	}
	
	SetFilename(absolutePath.GetPathUnix());
}

void igdeNativeFoxFileDialog::SetFilePatternList(const igdeFilePattern::List *filePatternList){
	pFilePatternList = filePatternList;
	
	UpdateFileTypeBox();
}

void igdeNativeFoxFileDialog::UpdateFileTypeBox(){
	pCBFilter->clearItems();
	
	if(pFilePatternList){
		decString text;
		
		pFilePatternList->Visit([&](const igdeFilePattern &fp){
			text.Format("%s (%s)", fp.GetName().GetString(), fp.GetPattern().GetString());
			pCBFilter->appendItem(text.GetString());
		});
	}
	
	pCBFilter->appendItem("All Files (*)");
	
	if(pFilePatternList->IsNotEmpty()){
		pList->SetPattern(pFilePatternList->First()->GetPattern());
		
	}else{
		pList->SetPattern("*");
	}
}



// Callbacks
//////////////

long igdeNativeFoxFileDialog::onCmdAccept(FXObject *sender, FXSelector selector, void *pdata){
	const FXString filename(pEditFilename->getText());
	
	// only do something if a selection was made
	if(filename.empty()){
		return 1; // say in dialog
	}
	
	// create absolute path
	decPath path;
	
	if(decPath::IsUnixPathAbsolute(filename.text())){
		path.SetFromUnix(filename.text());
		
	}else{
		path.SetFrom(pList->GetPath());
		path.AddUnixPath(filename.text());
	}
	
	// if this is a directory browse it
	if(pVFS->ExistsFile(path) && pVFS->GetFileType(path) == deVFSContainer::eftDirectory){
		pList->SetPath(path);
		pCBDirectory->SetPath(path);
		return 1; // stay in dialog
	}
	
	// get directory part of the file
	decPath directory;
	
	if(path.HasComponents()){
		directory.SetFrom(path);
		directory.RemoveLastComponent();
		
	}else{
		directory.SetFromUnix("/");
	}
	
	// if the directory exists this is enough ( for the time being )
	if(pVFS->ExistsFile(directory) && pVFS->GetFileType(directory) == deVFSContainer::eftDirectory){
		pSetFilenameAndAppendExtension(path.GetPathUnix());
		return FXDialogBox::onCmdAccept(sender, selector, pdata); // close dialog
	}
	
	// otherwise if the file exists this is good too ( for the time being )
	if(pVFS->ExistsFile(path)){
		pSetFilenameAndAppendExtension(path.GetPathUnix());
		return FXDialogBox::onCmdAccept(sender, selector, pdata); // close dialog
	}
	
	// go the the latest directory still existing
	SetFilename(path.GetPathUnix());
	pEditFilename->selectAll();
	return 1;
}



long igdeNativeFoxFileDialog::onBtnUpDir(FXObject*, FXSelector, void*){
	decPath path(pList->GetPath());
	if(!path.HasComponents()){
		return 0;
	}
	
	path.RemoveLastComponent();
	pList->SetPath(path);
	pCBDirectory->SetPath(path);
	return 1;
}

long igdeNativeFoxFileDialog::onBtnGoRoot(FXObject*, FXSelector, void*){
	const decPath path(decPath::CreatePathUnix("/"));
	pList->SetPath(path);
	pCBDirectory->SetPath(path);
	return 1;
}

long igdeNativeFoxFileDialog::onBtnMkDir(FXObject*, FXSelector, void*){
	FXString dirname("Directory");
	
	while(true){
		if(!FXInputDialog::getString(dirname, this, "Create directory", "Name:", nullptr)){
			return 0;
		}
		
		try{
			decPath path(pList->GetPath());
			path.AddComponent(dirname.text());
			
			if(pVFS->ExistsFile(path)){
				FXMessageBox::information(this, MBOX_OK, "Create directory",
					"There exists already file with this name.");
				
			}else if(!pVFS->CanWriteFile(path)){
				FXMessageBox::information(this, MBOX_OK, "Create directory",
					"Can not create directory here: Read-only VFS container.");
				
			}else{
				break;
			}
			
		}catch(const deException &e){
			igdeCommonDialogs::Exception(pOwner, e);
			return 0;
		}
	}
	
	// we can only create a cirect by touching a file inside and removing it again
	try{
		decPath path(pList->GetPath());
		path.AddComponent(dirname.text());
		path.AddComponent("_delete_me_");
		
		pVFS->TouchFile(path);
		pVFS->DeleteFile(path);
		
	}catch(const deException &e){
		igdeCommonDialogs::Exception(pOwner, e);
		return 0;
	}
	
	// change to new directory and update listing
	decPath path(pList->GetPath());
	path.AddComponent(dirname.text());
	pList->SetPath(path);
	pCBDirectory->SetPath(path);
	
	pList->UpdateListing();
	return 1;
}

long igdeNativeFoxFileDialog::onBtnListMode(FXObject*, FXSelector, void*){
	return 0;
}

long igdeNativeFoxFileDialog::onBtnIconMode(FXObject*, FXSelector, void*){
	return 0;
}

long igdeNativeFoxFileDialog::onBtnDetailMode(FXObject*, FXSelector, void*){
	return 0;
}

long igdeNativeFoxFileDialog::updateBtnUpDir(FXObject *sender, FXSelector, void*){
	const bool enable = pList->GetPath().GetComponentCount() > 0;
	sender->handle(this, FXSEL(SEL_COMMAND, enable ? ID_ENABLE : ID_DISABLE), nullptr);
	return 1;
}

long igdeNativeFoxFileDialog::updateBtnGoRoot(FXObject *sender, FXSelector, void*){
	sender->handle(this, FXSEL(SEL_COMMAND, ID_ENABLE), nullptr);
	return 1;
}

long igdeNativeFoxFileDialog::updateBtnMkDir(FXObject *sender, FXSelector, void*){
	sender->handle(this, FXSEL(SEL_COMMAND, ID_ENABLE), nullptr);
	return 1;
}

long igdeNativeFoxFileDialog::updateBtnListMode(FXObject *sender, FXSelector, void*){
	sender->handle(this, FXSEL(SEL_COMMAND, ID_DISABLE), nullptr);
	return 1;
}

long igdeNativeFoxFileDialog::updateBtnIconMode(FXObject *sender, FXSelector, void*){
	sender->handle(this, FXSEL(SEL_COMMAND, ID_DISABLE), nullptr);
	return 1;
}

long igdeNativeFoxFileDialog::updateBtnDetailMode(FXObject *sender, FXSelector, void*){
	sender->handle(this, FXSEL(SEL_COMMAND, ID_DISABLE), nullptr);
	return 1;
}



long igdeNativeFoxFileDialog::onCBDirectoryChanged(FXObject*, FXSelector, void*){
	decPath path;
	pCBDirectory->GetPath(path);
	
	pList->SetPath(path);
	pCBDirectory->SetPath(path);
	return 1;
}

long igdeNativeFoxFileDialog::onCBFilterChanged(FXObject*, FXSelector, void*){
	const int index = pCBFilter->getCurrentItem();
	if(index == -1){
		return 0;
	}
	
	int filePatternCount = 0;
	
	if(pFilePatternList){
		filePatternCount = pFilePatternList->GetCount();
	}
	
	if(index < filePatternCount){
		pList->SetPattern(pFilePatternList->GetAt(index)->GetPattern());
		
	}else{
		pList->SetPattern("*");
	}
	
	return 1;
}



long igdeNativeFoxFileDialog::onFLItemDoubleClicked(FXObject*, FXSelector, void*){
	igdeNativeFoxVFSListItem * const item = pList->GetSelectedItem();
	if(!item){
		return 0;
	}
	
	if(item->IsDirectory()){
		// if this is a directoy browse it
		pList->SetPath(item->GetPath());
		pCBDirectory->SetPath(pList->GetPath());
		
	}else{
		// otherwise it's a file
		handle(this, FXSEL(SEL_COMMAND, ID_ACCEPT), nullptr);
	}
	
	return 1;
}

long igdeNativeFoxFileDialog::onFLRightMouseUp(FXObject*, FXSelector, void*){
	return 0;
}

long igdeNativeFoxFileDialog::onFLItemSelected(FXObject*, FXSelector, void*){
	const igdeNativeFoxVFSListItem * const item = pList->GetSelectedItem();
	if(!item){
		return 0;
	}
	
	if(item->IsDirectory()){
		// do not set filename if a directory is selected. this allows the user to keep
		// the last selected filename while finding an appropriate directory
		return 0;
	}
	
	//pEditFilename->setText( item->GetPath().GetPathUnix().GetString() );
	pEditFilename->setText(item->GetFileName());
	return 1;
}

long igdeNativeFoxFileDialog::onFLItemDeselected(FXObject*, FXSelector, void*){
// 	pEditFilename->setText( "" );
// 	return 1;
	// do not clear the filename. can be annoying to save files
	return 0;
}



// Private Functions
//////////////////////

void igdeNativeFoxFileDialog::pCreateDialog(){
	// icons
	pIconUpDir = new FXGIFIcon(getApp(), FoxIcons::dirupicon);
	pIconListMode = new FXGIFIcon(getApp(), FoxIcons::showsmallicons);
	pIconIconMode = new FXGIFIcon(getApp(), FoxIcons::showbigicons);
	pIconDetailMode = new FXGIFIcon(getApp(), FoxIcons::showdetails);
	pIconGoRoot = new FXGIFIcon(getApp(), FoxIcons::gotohome);
	pIconMkdir = new FXGIFIcon(getApp(), FoxIcons::foldernew);
	pIconDelete = new FXGIFIcon(getApp(), FoxIcons::filedelete);
	pIconMove = new FXGIFIcon(getApp(), FoxIcons::filemove);
	pIconCopy = new FXGIFIcon(getApp(), FoxIcons::filecopy);
	pIconLink = new FXGIFIcon(getApp(), FoxIcons::filelink);
	
	// create dialog
	FXVerticalFrame *content = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y
		| LAYOUT_TOP | LAYOUT_LEFT);
	
	// navigaiton line
	FXHorizontalFrame *frameNav = new FXHorizontalFrame(content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, DEFAULT_SPACING, 0, 0);
	
	new FXLabel(frameNav, "Directory:", nullptr, LAYOUT_CENTER_Y);
	pCBDirectory = new igdeNativeFoxVFSDirectoryBox(pVFS, frameNav, this, ID_CBDIRECTORY, DIRBOX_NO_OWN_ASSOC
		| FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X | LAYOUT_CENTER_Y, 0, 0, 0, 0, 1, 1, 1, 1);
	pCBDirectory->setNumVisible(5);
	
	new FXFrame(frameNav, LAYOUT_FIX_WIDTH, 0, 0, 4, 1);
	new FXButton(frameNav, "\tGo up one directory\tMove up to higher directory.", pIconUpDir,
		this, ID_BTNUPDIR, BUTTON_TOOLBAR | FRAME_RAISED, 0, 0, 0, 0, 3, 3, 3, 3);
	new FXButton(frameNav, "\tGo to root directory\tBack to root directory.", pIconGoRoot,
		this, ID_BTNGOROOT, BUTTON_TOOLBAR | FRAME_RAISED, 0, 0, 0, 0, 3, 3, 3, 3);
	new FXButton(frameNav, "\tCreate new directory\tCreate new directory.", pIconMkdir,
		this, ID_BTNMKDIR, BUTTON_TOOLBAR | FRAME_RAISED, 0, 0, 0, 0, 3, 3, 3, 3);
	
	// file list
	FXHorizontalFrame *frameLine = new FXHorizontalFrame(content, LAYOUT_SIDE_TOP
		| LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN | FRAME_THICK, 0, 0, 0, 0, 0, 0, 0, 0);
	pList = new igdeNativeFoxVFSList(pOwner->GetEnvironment(), pVFS, frameLine, this,
		ID_FILELIST, ICONLIST_MINI_ICONS | ICONLIST_BROWSESELECT | ICONLIST_AUTOSIZE
		| LAYOUT_FILL_X | LAYOUT_FILL_Y);
	
	// navigation line continued
	new FXButton(frameNav, "\tShow list\tDisplay directory with small icons.", pIconListMode, pList,
		igdeNativeFoxVFSList::ID_SHOW_MINI_ICONS, BUTTON_TOOLBAR | FRAME_RAISED, 0, 0, 0, 0, 3, 3, 3, 3);
	new FXButton(frameNav, "\tShow icons\tDisplay directory with big icons.", pIconIconMode, pList,
		igdeNativeFoxVFSList::ID_SHOW_BIG_ICONS, BUTTON_TOOLBAR | FRAME_RAISED, 0, 0, 0, 0, 3, 3, 3, 3);
	new FXButton(frameNav, "\tShow details\tDisplay detailed directory listing.", pIconDetailMode, pList,
		igdeNativeFoxVFSList::ID_SHOW_DETAILS, BUTTON_TOOLBAR | FRAME_RAISED, 0, 0, 0, 0, 3, 3, 3, 3);
	
	// entry block
	FXMatrix *entryBlock = new FXMatrix(content, 3, MATRIX_BY_COLUMNS | LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X);
	
	new FXLabel(entryBlock, "&File Name:");
	pEditFilename = new FXTextField(entryBlock, 25, this, ID_ACCEPT, TEXTFIELD_ENTER_ONLY
		| LAYOUT_FILL_COLUMN | LAYOUT_FILL_X | FRAME_SUNKEN | FRAME_THICK);
	new FXButton(entryBlock, "&OK", nullptr, this, ID_ACCEPT, BUTTON_INITIAL | BUTTON_DEFAULT
		| FRAME_RAISED | FRAME_THICK | LAYOUT_FILL_X, 0, 0, 0, 0, 20, 20);
	
	new FXLabel(entryBlock, "File F&ilter:", nullptr, JUSTIFY_LEFT | LAYOUT_CENTER_Y);
	pCBFilter = new FXComboBox(entryBlock, 10, this, ID_CBFILTER, COMBOBOX_STATIC
		| LAYOUT_FILL_COLUMN | LAYOUT_FILL_X | FRAME_SUNKEN | FRAME_THICK);
	pCBFilter->setNumVisible(4);
	pCBFilter->appendItem("All Files (*)");
	new FXButton(entryBlock, "&Cancel", nullptr, this, ID_CANCEL, BUTTON_DEFAULT | FRAME_RAISED
		| FRAME_THICK | LAYOUT_FILL_X, 0, 0, 0, 0, 20, 20);
	
	
	
	//filebox=new FXFileSelector(this,nullptr,0,LAYOUT_FILL_X|LAYOUT_FILL_Y);
	//filebox->acceptButton()->setTarget(this);
	//filebox->acceptButton()->setSelector(FXDialogBox::ID_ACCEPT);
	//filebox->cancelButton()->setTarget(this);
	//filebox->cancelButton()->setSelector(FXDialogBox::ID_CANCEL);
	
	setWidth(getApp()->reg().readIntEntry("File Dialog", "width", getWidth()));
	setHeight(getApp()->reg().readIntEntry("File Dialog", "height", getHeight()));
//	setFileBoxStyle( getApp()->reg().readUnsignedEntry( "File Dialog", "style", getFileBoxStyle() ) );
//	showHiddenFiles( getApp()->reg().readUnsignedEntry( "File Dialog", "showhidden", showHiddenFiles() ) );
}

void igdeNativeFoxFileDialog::pFindExistingDirectory(decPath &path, decPath &directory){
	// init the directory with the path to check
	directory.SetFrom(path);
	
	// travel up the directories until one is existing
	while(directory.HasComponents()){
		// if this is a directory we found what we are looking for
		if(pVFS->ExistsFile(directory) && pVFS->GetFileType(directory) == deVFSContainer::eftDirectory){
			// remove components from the path until it matches and make it relative
			const int count = directory.GetComponentCount();
			int i;
			
			for(i=0; i<count; i++){
				path.RemoveComponentFrom(0);
			}
			path.SetPrefix("");
			
			// finished
			break;
		}
		
		// otherwise step up to the parent directory
		directory.RemoveLastComponent();
	}
}

void igdeNativeFoxFileDialog::pSetFilenameAndAppendExtension(const char *filename){
	FXString finalFilename(filename);
	igdeFilePattern *pattern = nullptr;
	
	const int selection = pCBFilter->getCurrentItem();
	if(selection >= 0 && selection < pFilePatternList->GetCount()){
		pattern = pFilePatternList->GetAt(selection);
	}
	
	if(pattern){
		decPath tempPath, testPath, patternPath;
		
		tempPath.SetFromUnix(filename);
		testPath.AddComponent(tempPath.GetLastComponent());
		patternPath.AddComponent(pattern->GetPattern().GetString());
		
		if(!testPath.MatchesPattern(patternPath)){
			finalFilename.append(pattern->GetDefaultExtension().GetString());
		}
	}
	
	pEditFilename->setText(finalFilename.text());
}

#endif
