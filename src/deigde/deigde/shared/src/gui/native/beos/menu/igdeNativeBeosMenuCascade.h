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

#ifndef _IGDENATIVEBEOSMENUCASCADE_H_
#define _IGDENATIVEBEOSMENUCASCADE_H_

class igdeMenuCascade;
class igdeWidget;


/**
 * \brief BeOS native cascade (submenu) menu item.
 * 
 * Uses void* pattern for native container support.
 */
class igdeNativeBeosMenuCascade{
public:
	/** \name Management */
	/*@{*/
	static void* CreateNativeWidget(igdeMenuCascade &owner);
	static void PostCreateNativeWidget(igdeMenuCascade &owner, void *native);
	static void DestroyNativeWidget(igdeMenuCascade &owner, void *native);
	static void* GetNativeContainer(igdeMenuCascade &owner, void *native);
	static void UpdateTitle(igdeMenuCascade &owner, void *native);
	static void UpdateDescription(igdeMenuCascade &owner, void *native);
	static void UpdateHotKey(igdeMenuCascade &owner, void *native);
	static void UpdateIcon(igdeMenuCascade &owner, void *native);
	static void UpdateEnabled(igdeMenuCascade &owner, void *native);
	
	static void *CreateNativePopup(igdeMenuCascade &owner, igdeWidget &widgetOwner);
	static void PostCreateNativePopup(igdeMenuCascade &owner, void *native);
	static void ShowPopupWindow(igdeMenuCascade &owner, igdeWidget &widgetOwner,
		const decPoint &position);
	static void DestroyNativePopup(igdeMenuCascade &owner, void *native);
	/*@}*/
};

typedef igdeNativeBeosMenuCascade igdeNativeMenuCascade;

#endif
