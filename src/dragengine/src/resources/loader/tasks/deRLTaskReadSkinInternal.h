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

#ifndef _DERLTASKREADSKININTERNAL_H_
#define _DERLTASKREADSKININTERNAL_H_

#include "deResourceLoaderTask.h"
#include "../../skin/deSkin.h"
#include "../../../common/collection/decThreadSafeObjectOrderedSet.h"

class deSkinPropertyImage;
class deSkinPropertyNodeImage;
class deSkinPropertyNodeText;


/**
 * \brief Read skin resource loader task.
 * 
 * \warning For internal use only.
 */
class DE_DLL_EXPORT deRLTaskReadSkinInternal : public deResourceLoaderTask {
public:
	/** \brief Type holding strong reference. */
	typedef deTThreadSafeObjectReference<deRLTaskReadSkinInternal> Ref;
	
	
	/**
	 * \brief For internal use only.
	 * 
	 * Used to store tasks obtained by AddLoadRequest. This class holds a reference to
	 * the task. Do not release the reference on the task returned by AddLoadRequest after
	 * creating a cInternalTask instance since AddLoadRequest does not add a reference
	 * for the caller
	 */
	class DE_DLL_EXPORT cInternalTask : public deThreadSafeObject{
	public:
		/** \brief Type holding strong reference. */
		typedef deTThreadSafeObjectReference<cInternalTask> Ref;
		
		
	private:
		deSkinPropertyImage *pPropertyImage;
		deSkinPropertyNodeImage *pNodeImage;
		deSkinPropertyNodeText *pNodeText;
		deResourceLoaderTask::Ref pTask;
		
	public:
		cInternalTask(deSkinPropertyImage *propertyImage, deResourceLoaderTask *task);
		cInternalTask(deSkinPropertyNodeImage *nodeImage, deResourceLoaderTask *task);
		cInternalTask(deSkinPropertyNodeText *nodeText, deResourceLoaderTask *task);
		~cInternalTask() override;
		
		inline deSkinPropertyImage *GetPropertyImage() const{ return pPropertyImage; }
		inline deSkinPropertyNodeImage *GetNodeImage() const{ return pNodeImage; }
		inline deSkinPropertyNodeText *GetNodeText() const{ return pNodeText; }
		inline const deResourceLoaderTask::Ref &GetTask() const{ return pTask; }
	};
	
	
	
private:
	deSkin::Ref pSkin;
	decThreadSafeObjectOrderedSet pInternalTasks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task. */
	deRLTaskReadSkinInternal(deEngine &engine, deResourceLoader &resourceLoader,
		deVirtualFileSystem *vfs, const char *path);
	
	/** \brief Clean up task. */
	~deRLTaskReadSkinInternal() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parallel task implementation. */
	void Run() override;
	
	/** \brief Synchronous processing of task Run() finished. */
	void Finished() override;
	
	/** \brief Skin. */
	inline const deSkin::Ref &GetSkin() const{ return pSkin; }
	/*@}*/
	
	
	
	/** \name Internal use only */
	/*@{*/
	void AddInternalTask(cInternalTask *task);
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Short task name for debugging. */
	decString GetDebugName() const override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pPrepare();
	void pLoadInternal();
	bool pApplyInternal();
};

#endif
