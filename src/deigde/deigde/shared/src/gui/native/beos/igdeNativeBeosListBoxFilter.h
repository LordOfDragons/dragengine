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

#ifndef _IGDENATIVEBEOSLISTBOXFILTER_H_
#define _IGDENATIVEBEOSLISTBOXFILTER_H_

class igdeListBoxFilter;


/**
 * \brief BeOS native list box filter.
 */
class igdeNativeBeosListBoxFilter{
public:
	igdeNativeBeosListBoxFilter();
	virtual ~igdeNativeBeosListBoxFilter();
	
	static void *CreateNativeWidget(igdeListBoxFilter &owner);
	static void PostCreateNativeWidget(igdeListBoxFilter &owner, void *native);
	static void DestroyNativeWidget(igdeListBoxFilter &owner, void *native);
};

typedef igdeNativeBeosListBoxFilter igdeNativeListBoxFilter;

#endif
