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

#ifndef _DERLTASKREADFONTINTERNAL_H_
#define _DERLTASKREADFONTINTERNAL_H_

#include "deResourceLoaderTask.h"
#include "deRLTaskReadFontInternal2.h"
#include "../../font/deFont.h"

class deRLTaskReadFont;


/**
 * \brief Read font resource loader task.
 * 
 * \warning For internal use only.
 */
class DE_DLL_EXPORT deRLTaskReadFontInternal : public deResourceLoaderTask {
public:
	/** \brief Type holding strong reference. */
	typedef deTThreadSafeObjectReference<deRLTaskReadFontInternal> Ref;
	
	
private:
	deFont::Ref pFont;
	bool pSucceeded;
	bool pAlreadyLoaded;
	
	deRLTaskReadFont *pParentTask;
	deRLTaskReadFontInternal2::Ref pInternalTask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	deRLTaskReadFontInternal(deEngine &engine, deResourceLoader &resourceLoader,
		deVirtualFileSystem *vfs, const char *path, deRLTaskReadFont *parentTask);
	
	/** \brief Clean up task. */
	~deRLTaskReadFontInternal() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parallel task implementation. */
	void Run() override;
	
	/** \brief Synchronous processing of task Run() finished. */
	void Finished() override;
	
	/** \brief Font. */
	inline const deFont::Ref &GetFont() const{ return pFont; }
	
	/** \brief Font has been already loaded. */
	inline bool GetAlreadyLoaded() const{ return pAlreadyLoaded; }
	
	/** \brief Second task or NULL. */
	inline const deRLTaskReadFontInternal2::Ref &GetInternalTask() const{ return pInternalTask; }
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	decString GetDebugName() const override;
	/*@}*/
};

#endif
