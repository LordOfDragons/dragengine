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

#include "igdeNativeBeosVFSList.h"
#include "igdeNativeBeosVFSListItem.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCollectDirectorySearchVisitor.h>
#include <dragengine/filesystem/deCollectFileSearchVisitor.h>

#include <algorithm>
#include <interface/ListView.h>
#include <interface/ScrollView.h>
#include <interface/StringItem.h>


// Class igdeNativeBeosVFSListFinder
/////////////////////////////////////

class igdeNativeBeosVFSListFinder : public deFileSearchVisitor {
private:
	std::vector<igdeNativeBeosVFSListItem*> &pItems;
	const char * const pPattern;
	
public:
	igdeNativeBeosVFSListFinder(std::vector<igdeNativeBeosVFSListItem*> &items,
		const char *const pattern) :
	pItems(items), pPattern(pattern){
	}
	
	bool VisitFile(const deVirtualFileSystem &, const decPath &path) override {
		if(!path.GetPathUnix().MatchesPattern(pPattern)){
			return true;
		}
		
		try{
			auto item = new igdeNativeBeosVFSListItem(path, path.GetLastComponent());
			item->SetFileType(deVFSContainer::eftRegularFile);
			pItems.push_back(item);
			
		} catch(const deException &){}
		return true;
	}
	
	bool VisitDirectory(const deVirtualFileSystem &, const decPath &path) override {
		try{
			auto item = new igdeNativeBeosVFSListItem(path, path.GetLastComponent());
			item->SetFileType(deVFSContainer::eftDirectory);
			pItems.push_back(item);
			
		} catch(const deException &){}
		return true;
	}
	
	bool VisitSpecial(const deVirtualFileSystem &, const decPath &path) override {
		if(!path.GetPathUnix().MatchesPattern(pPattern)){
			return true;
		}
		
		try{
			auto item = new igdeNativeBeosVFSListItem(path, path.GetLastComponent());
			item->SetFileType(deVFSContainer::eftSpecial);
			pItems.push_back(item);
			
		} catch(const deException &){}
		return true;
	}
};


// Class igdeNativeBeosVFSList
///////////////////////////////

igdeNativeBeosVFSList::igdeNativeBeosVFSList(igdeEnvironment &environment,
	deVirtualFileSystem *vfs, BRect frame, const char *name) :
BView(frame, name, B_FOLLOW_ALL, B_WILL_DRAW),
pEnvironment(&environment),
pVFS(vfs),
pListView(nullptr),
pScrollView(nullptr),
pPattern("*"),
pSorting(esNameAsc)
{
	DEASSERT_NOTNULL(vfs)
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// Create list view
	BRect listRect = frame;
	listRect.OffsetTo(0, 0);
	pListView = new BListView(listRect, "vfs_list", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	
	// Create scroll view
	pScrollView = new BScrollView("vfs_scroll", pListView, B_FOLLOW_ALL, B_WILL_DRAW, false, true);
	
	AddChild(pScrollView);
}

igdeNativeBeosVFSList::~igdeNativeBeosVFSList(){
	pClearItems();
}


// Management
//////////////

void igdeNativeBeosVFSList::UpdateListing(){
	pClearItems();
	
	// Add current directory entry
	auto item = new igdeNativeBeosVFSListItem(pCurPath, ".");
	item->SetFileType(deVFSContainer::eftDirectory);
	pItems.push_back(item);
	pListView->AddItem(new BStringItem(item->UpdateText("n")));
	
	// Add parent directory if not at root
	if(pCurPath.GetComponentCount() > 0){
		decPath path;
		path.SetFrom(pCurPath);
		path.RemoveLastComponent();
		item = new igdeNativeBeosVFSListItem(path, "..");
		item->SetFileType(deVFSContainer::eftDirectory);
		pItems.push_back(item);
		pListView->AddItem(new BStringItem(item->UpdateText("n")));
	}
	
	// Add files and directories from VFS
	igdeNativeBeosVFSListFinder visitor(pItems, pPattern.GetString());
	pVFS->SearchFiles(pCurPath, visitor);
	
	// Add items to list and sort
	for(size_t i=(pCurPath.GetComponentCount() > 0 ? 2 : 1); i<pItems.size(); i++){
		pListView->AddItem(new BStringItem(pItems[i]->UpdateText("n|s|m")));
	}
	
	pSortItems();
}

void igdeNativeBeosVFSList::SetPath(const decPath &path){
	if(path != pCurPath){
		pCurPath.SetFrom(path);
		UpdateListing();
	}
}

void igdeNativeBeosVFSList::SetPattern(const char *pattern){
	DEASSERT_NOTNULL(pattern)
	
	if(pPattern != pattern){
		pPattern = pattern;
		UpdateListing();
	}
}

void igdeNativeBeosVFSList::SetSorting(eSorting sorting){
	if(sorting != pSorting){
		pSorting = sorting;
		pSortItems();
	}
}

igdeNativeBeosVFSListItem *igdeNativeBeosVFSList::GetSelectedItem() const {
	int32 index = pListView->CurrentSelection();
	if(index >= 0 && index < (int32)pItems.size()){
		return pItems[index];
	}
	return nullptr;
}

decPath igdeNativeBeosVFSList::GetSelectedPath() const {
	auto item = GetSelectedItem();
	if(item){
		return item->GetPath();
	}
	return decPath();
}

void igdeNativeBeosVFSList::AttachedToWindow(){
	BView::AttachedToWindow();
	pListView->SetTarget(this);
}

void igdeNativeBeosVFSList::MessageReceived(BMessage *message){
	BView::MessageReceived(message);
}


// Private functions
/////////////////////

void igdeNativeBeosVFSList::pClearItems(){
	for(igdeNativeBeosVFSListItem * const item : pItems){
		delete item;
	}
	pItems.clear();
	pListView->MakeEmpty();
}

bool igdeNativeBeosVFSList::pMatchesPattern(const char *filename) const {
	if(!filename){
		return false;
	}
	return decString(filename).MatchesPattern(pPattern);
}

bool igdeNativeBeosVFSList::pSortCompareNameAsc(const igdeNativeBeosVFSListItem *i1, const igdeNativeBeosVFSListItem *i2){
	return i1->GetFileName().Compare(i2->GetFileName()) < 0;
}

bool igdeNativeBeosVFSList::pSortCompareNameDesc(const igdeNativeBeosVFSListItem *i1, const igdeNativeBeosVFSListItem *i2){
	return i2->GetFileName().Compare(i1->GetFileName()) < 0;
}

bool igdeNativeBeosVFSList::pSortCompareSizeAsc(const igdeNativeBeosVFSListItem *i1, const igdeNativeBeosVFSListItem *i2){
	return i1->GetFileSize() < i2->GetFileSize();
}

bool igdeNativeBeosVFSList::pSortCompareSizeDesc(const igdeNativeBeosVFSListItem *i1, const igdeNativeBeosVFSListItem *i2){
	return i1->GetFileSize() > i2->GetFileSize();
}

bool igdeNativeBeosVFSList::pSortCompareTimeAsc(const igdeNativeBeosVFSListItem *i1, const igdeNativeBeosVFSListItem *i2){
	return i1->GetFileModificationTime() < i2->GetFileModificationTime();
}

bool igdeNativeBeosVFSList::pSortCompareTimeDesc(const igdeNativeBeosVFSListItem *i1, const igdeNativeBeosVFSListItem *i2){
	return i1->GetFileModificationTime() > i2->GetFileModificationTime();
}

void igdeNativeBeosVFSList::pSortItems(){
	// Sort items vector based on current sorting mode
	bool(*sortFunc)(const igdeNativeBeosVFSListItem*, const igdeNativeBeosVFSListItem*) = nullptr;
	
	switch(pSorting){
	case esNameAsc:
		sortFunc = pSortCompareNameAsc;
		break;
		
	case esNameDesc:
		sortFunc = pSortCompareNameDesc;
		break;
		
	case esSizeAsc:
		sortFunc = pSortCompareSizeAsc;
		break;
		
	case esSizeDesc:
		sortFunc = pSortCompareSizeDesc;
		break;
		
	case esModifyTimeAsc:
		sortFunc = pSortCompareTimeAsc;
		break;
		
	case esModifyTimeDesc:
		sortFunc = pSortCompareTimeDesc;
		break;
	}
	
	if(sortFunc){
		// Keep first 1 or 2 items (. and ..) at the top
		int fixedCount = pCurPath.GetComponentCount() > 0 ? 2 : 1;
		if(pItems.size() > (size_t)fixedCount){
			std::sort(pItems.begin() + fixedCount, pItems.end(), sortFunc);
		}
	}
	
	// Refresh list display
	pListView->MakeEmpty();
	for(const igdeNativeBeosVFSListItem * const item : pItems){
		pListView->AddItem(new BStringItem(item->UpdateText("n|s|m")));
	}
}

#endif
