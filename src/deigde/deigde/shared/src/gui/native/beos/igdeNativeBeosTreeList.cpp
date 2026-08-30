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

#include "igdeNativeBeosTreeList.h"
#include "../../igdeTreeList.h"
#include "../../igdeContainer.h"
#include "../../theme/igdeGuiTheme.h"
#include <dragengine/common/exceptions.h>


// Class igdeNativeBeosTreeList
/////////////////////////////////

igdeNativeBeosTreeList::igdeNativeBeosTreeList() :
BView(BRect(0, 0, 200, 200), "treelist", B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS),
pOwner(nullptr),
pNodes(),
pSelectedNode(nullptr),
pScrollOffset(0)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}

igdeNativeBeosTreeList::~igdeNativeBeosTreeList(){
	pNodes.RemoveAll();
}


igdeNativeBeosTreeList* igdeNativeBeosTreeList::CreateNativeWidget(igdeTreeList &owner){
	// TreeList widget creates a hierarchical list with expand/collapse nodes
	// Implementation uses custom drawing for tree items
	DEASSERT_NOTNULL(owner.GetParent())
	
	auto parent = (BView*)owner.GetParent()->GetNativeContainer();
	DEASSERT_NOTNULL(parent)
	
	auto widget = new igdeNativeBeosTreeList();
	widget->pOwner = &owner;
	parent->AddChild(widget);
	
	return widget;
}

void igdeNativeBeosTreeList::PostCreateNativeWidget(igdeTreeList &owner, void *native){
	// TreeList post-creation setup
	auto widget = (igdeNativeBeosTreeList*)native;
	if(widget){
		widget->pOwner = &owner;
	}
}

void igdeNativeBeosTreeList::DestroyNativeWidget(igdeTreeList &owner, void *native){
	auto widget = (igdeNativeBeosTreeList*)native;
	if(widget){
		widget->RemoveSelf();
		delete widget;
	}
}


// Management
///////////////

void igdeNativeBeosTreeList::Draw(BRect updateRect){
	BView::Draw(updateRect);
	
	try{
		// Draw each node in the tree
		for(int i=0; i<pNodes.CountItems(); i++){
			auto node = (TreeNode*)pNodes.ItemAt(i);
			if(node){
				DrawNode(updateRect, node, 10, 15 + (i * 20));
			}
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE TreeList", e);
		}
	}
}

void igdeNativeBeosTreeList::DrawNode(BRect updateRect, TreeNode *node, float x, float y){
	try{
		// Draw expand/collapse button
		if(node->children.CountItems() > 0){
			BRect btnRect(x, y, x + 10, y + 10);
			if(updateRect.Intersects(btnRect)){
				// Draw expand/collapse triangle
				BPoint points[3];
				if(node->expanded){
					points[0] = BPoint(x, y);
					points[1] = BPoint(x + 10, y);
					points[2] = BPoint(x + 5, y + 10);
					
				}else{
					points[0] = BPoint(x, y);
					points[1] = BPoint(x + 10, y + 5);
					points[2] = BPoint(x, y + 10);
				}
			}
		}
		
		// Draw text
		BRect textRect(x + 20, y, Bounds().right - 10, y + 15);
		if(updateRect.Intersects(textRect)){
			if(node == pSelectedNode){
				SetHighColor(ui_color(B_HIGHLIGHT_BACKGROUND_COLOR));
				FillRect(textRect);
				SetHighColor(ui_color(B_HIGHLIGHT_TEXT_COLOR));
				
			}else{
				SetHighColor(ui_color(B_PANEL_TEXT_COLOR));
			}
			DrawString(node->label.String(), BPoint(x + 25, y + 12));
		}
		
		// Draw children if expanded
		if(node->expanded){
			float childY = y + 20;
			for(int i=0; i<node->children.CountItems(); i++){
				auto child = (TreeNode*)node->children.ItemAt(i);
				if(child){
					DrawNode(updateRect, child, x + 20, childY);
					childY += 20;
				}
			}
		}
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE TreeList", e);
		}
	}
}

void igdeNativeBeosTreeList::MouseDown(BPoint where){
	// Handle node selection and expand/collapse
	BView::MouseDown(where);
	
	// Calculate which node was clicked
	int nodeIndex = (int)(where.y / 20);
	if(nodeIndex >= 0 && nodeIndex < pNodes.CountItems()){
		auto node = (TreeNode*)pNodes.ItemAt(nodeIndex);
		if(node){
			// Check if expand/collapse button was clicked
			if(where.x < 20 && node->children.CountItems() > 0){
				node->expanded = !node->expanded;
				Invalidate();
			}
			
			// Select node
			if(pSelectedNode != node){
				pSelectedNode = node;
				Invalidate();
				
				// Fire selection changed callback
				if(pOwner && pOwner->GetAction()){
					pOwner->GetAction()->OnAction();
				}
			}
		}
	}
}

void igdeNativeBeosTreeList::FrameResized(float width, float height){
	BView::FrameResized(width, height);
	
	try{
		Invalidate();
		
	}catch(const deException &e){
		if(pOwner){
			pOwner->GetLogger()->LogException("IGDE TreeList", e);
		}
	}
}

void igdeNativeBeosTreeList::UpdateNodeSelection(){
	// Synchronize selection from IGDE to native
	Invalidate();
}

void igdeNativeBeosTreeList::UpdateNodes(){
	// Rebuild tree from IGDE model
	pNodes.RemoveAll();
	pSelectedNode = nullptr;
	Invalidate();
}

#endif
