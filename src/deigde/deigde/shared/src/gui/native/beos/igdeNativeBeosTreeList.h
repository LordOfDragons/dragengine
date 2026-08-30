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

#ifndef _IGDENATIVEBEOSTREELIST_H_
#define _IGDENATIVEBEOSTREELIST_H_

#include <interface/View.h>
#include <support/List.h>
#include <support/String.h>

class igdeTreeList;


/**
 * BeOS tree list.
 */
class igdeNativeBeosTreeList : public BView {
private:
	igdeTreeList *pOwner;
	BList pNodes;
	TreeNode *pSelectedNode;
	int pScrollOffset;
	
public:
	struct TreeNode {
		BString label;
		bool expanded;
		BList children;
		TreeNode() : expanded(false) {}
	};
	
	igdeNativeBeosTreeList();
	virtual ~igdeNativeBeosTreeList();
	
	static igdeNativeBeosTreeList* CreateNativeWidget(igdeTreeList &owner);
	static void PostCreateNativeWidget(igdeTreeList &owner, void *native);
	static void DestroyNativeWidget(igdeTreeList &owner, void *native);
	
	// Drawing and interaction
	virtual void Draw(BRect updateRect);
	void DrawNode(BRect updateRect, TreeNode *node, float x, float y);
	virtual void MouseDown(BPoint where);
	virtual void FrameResized(float width, float height);
	
	// Management
	void UpdateNodeSelection();
	void UpdateNodes();
};

typedef igdeNativeBeosTreeList igdeNativeTreeList;

#endif
