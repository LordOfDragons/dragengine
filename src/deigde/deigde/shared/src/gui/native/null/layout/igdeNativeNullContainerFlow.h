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

#ifndef _IGDENATIVENULLCONTAINERFLOW_H_
#define _IGDENATIVENULLCONTAINERFLOW_H_

class igdeContainerFlow;


/**
 * Null button.
 */
class igdeNativeNullContainerFlow{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	igdeNativeNullContainerFlow();
	virtual ~igdeNativeNullContainerFlow();
	
	/** \brief Create native widget. */
	static void* CreateNativeWidget(igdeContainerFlow &owner);
	
	/** \brief Post create native widget. */
	static void PostCreateNativeWidget(igdeContainerFlow &owner, void *native);
	
	/** \brief Destroy native widget. */
	static void DestroyNativeWidget(igdeContainerFlow &owner, void *native);
	
	/** \brief Child removed. */
	static void ChildRemoved(igdeContainerFlow &owner, void *native);
	
	/** \brief Native container. */
	static void *GetNativeContainer(const igdeContainerFlow &owner, void *native);
	/*@}*/
};

typedef igdeNativeNullContainerFlow igdeNativeContainerFlow;

#endif
