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

#include "gdeWPSTreeItemModel.h"
#include "gdeWPSTreeModel.h"
#include "../../../gamedef/gdeGameDefinition.h"

#include <deigde/gui/igdeTreeList.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

gdeWPSTreeItemModel::gdeWPSTreeItemModel(gdeWPSTreeModel &tree, eTypes type) :
igdeTreeItem(""),
pTree(tree),
pType(type){
}

gdeWPSTreeItemModel::~gdeWPSTreeItemModel(){
}



// Management
///////////////

gdeWindowMain &gdeWPSTreeItemModel::GetWindowMain() const{
	return pTree.GetWindowMain();
}

gdeGameDefinition &gdeWPSTreeItemModel::GetGameDefinition() const{
	return *pTree.GetGameDefinition();
}



void gdeWPSTreeItemModel::SortChildren(){
	pTree.GetTreeList().SortItems(this);
}

void gdeWPSTreeItemModel::ParentSortItems(){
	if(GetParent()){
		pTree.GetTreeList().SortItems(GetParent());
	}
}

void gdeWPSTreeItemModel::SetAsCurrentItem(){
	pTree.GetTreeList().SetSelection(this);
}

void gdeWPSTreeItemModel::AppendModel(igdeTreeItem *item){
	pTree.GetTreeList().AppendItem(this, item);
	((gdeWPSTreeItemModel*)item)->OnAddedToTree();
}

void gdeWPSTreeItemModel::RemoveModel(igdeTreeItem *item){
	pTree.GetTreeList().RemoveItem(item);
}

void gdeWPSTreeItemModel::RemoveAllModels(){
	pTree.GetTreeList().RemoveAllItems(this);
}

void gdeWPSTreeItemModel::ItemChanged(){
	pTree.GetTreeList().ItemChanged(this);
}



void gdeWPSTreeItemModel::OnAddedToTree(){
}

int gdeWPSTreeItemModel::Compare(const gdeWPSTreeItemModel &item) const{
	return 0;
}

void gdeWPSTreeItemModel::OnSelected(){
	GetGameDefinition().SetSelectedObjectType(gdeGameDefinition::eotNoSelection);
}

void gdeWPSTreeItemModel::OnContextMenu(igdeMenuCascade&){
}

void gdeWPSTreeItemModel::SelectBestMatching(const char*){
}
