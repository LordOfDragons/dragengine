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

#ifndef _DESCENE_H_
#define _DESCENE_H_

#include "../deFileResource.h"
#include "../../common/collection/decTDictionary.h"
#include "../../common/file/decMemoryFile.h"
#include "../../common/string/decStringList.h"

class deSceneManager;


/**
 * \brief Scene Resource.
 *
 * Container resource storing multiple resource and non-resource objects for use by scene
 * file formats like FBX or GLB. Such scenes are used to transfer data from one application
 * to another. For this reason scenes are fully loaded from files and the user is supposed
 * to store the actually used resources in Drag[en]gine optimized files.
 *
 * The resources dictionary maps unique names to loaded engine file resources. The name can
 * be an identifier, a name or a path depending on the scene file type. Some non-file resources
 * are stored as memory files (for example world files). This allows accessing entire scenes
 * to convert them properly into Drag[en]gine optimized files. The memory files dictionary
 * maps unique names to memory files similar to the resource dictionary. The keys used in each
 * of the two dictionaries are unique across both dictionaries.
 */
class DE_DLL_EXPORT deScene : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deScene>;
	
	
	
private:
	decTObjectDictionary<deResource> pResources;
	decTObjectDictionary<decMemoryFile> pFiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create scene resource with the given resource manager and filename. */
	deScene(deSceneManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up scene.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deScene() override;
	/*@}*/
	
	
	
public:
	/** \name Resource Management */
	/*@{*/
	/** \brief Resources. */
	inline const decTObjectDictionary<deResource> &GetResources() const{ return pResources; }
	
	/**
	 * \brief Add resource with key.
	 * \throws deeInvalidParam \em name is nullptr.
	 * \throws deeInvalidParam \em resource is nullptr.
	 * \throws deeInvalidParam Resource with \em name is already present.
	 */
	void AddResource(const char *name, deResource *resource);
	
	/**
	 * \brief Remove resource with key.
	 * \throws deeInvalidParam \em name is nullptr.
	 * \throws deeInvalidParam Resource with \em name is absent.
	 */
	void RemoveResource(const char *name);
	
	/** \brief Remove all resources. */
	void RemoveAllResources();
	/*@}*/
	
	
	
	/** \name File Management */
	/*@{*/
	/** \brief Count of files. */
	inline const decTObjectDictionary<decMemoryFile> &GetFiles() const{ return pFiles; }
	
	/**
	 * \brief Add file with key.
	 * \throws deeInvalidParam \em name is nullptr.
	 * \throws deeInvalidParam \em file is nullptr.
	 * \throws deeInvalidParam File with \em name is already present.
	 */
	void AddFile(const char *name, decMemoryFile *file);
	
	/**
	 * \brief Remove file with key.
	 * \throws deeInvalidParam \em name is nullptr.
	 * \throws deeInvalidParam File with \em name is absent.
	 */
	void RemoveFile(const char *name);
	
	/** \brief Remove all files. */
	void RemoveAllFiles();
	/*@}*/
};

#endif
