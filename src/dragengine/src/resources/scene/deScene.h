/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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
#include "../deResource.h"
#include "../../common/collection/decTDictionary.h"
#include "../../common/file/decMemoryFile.h"
#include "../../common/string/decStringList.h"

class deSceneManager;


/**
 * \brief Scene Resource.
 *
 * Container resource for scene file formats like FBX or GLB containing multiple objects
 * such as models, skins, rigs and animations. Allows importing entire scenes from another
 * application for later use.
 *
 * The resources dictionary maps unique names to loaded engine resources (deModel, deSkin,
 * deRig, deAnimation and similar). The name can be an identifier, a name or a path
 * depending on the scene file type. Objects not convertible to engine resources but
 * convertible to other file formats (like *.deworld) are stored as memory files.
 */
class DE_DLL_EXPORT deScene : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deScene>;
	
	
	
private:
	decTDictionary<decString, deResource::Ref> pResources;
	decTDictionary<decString, decMemoryFile::Ref> pMemoryFiles;
	
	
	
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
	/** \brief Count of resources. */
	int GetResourceCount() const;
	
	/** \brief Resource keys. */
	decStringList GetResourceKeys() const;
	
	/** \brief Resource with key or NULL if absent. */
	deResource *GetResourceNamed(const char *name) const;
	
	/** \brief Resource with key is present. */
	bool HasResourceNamed(const char *name) const;
	
	/**
	 * \brief Add resource with key.
	 * \throws deeInvalidParam \em name is NULL.
	 * \throws deeInvalidParam \em resource is NULL.
	 * \throws deeInvalidParam Resource with \em name is already present.
	 */
	void AddResource(const char *name, deResource *resource);
	
	/**
	 * \brief Remove resource with key.
	 * \throws deeInvalidParam \em name is NULL.
	 * \throws deeInvalidParam Resource with \em name is absent.
	 */
	void RemoveResource(const char *name);
	
	/** \brief Remove all resources. */
	void RemoveAllResources();
	/*@}*/
	
	
	
	/** \name Memory File Management */
	/*@{*/
	/** \brief Count of memory files. */
	int GetMemoryFileCount() const;
	
	/** \brief Memory file keys. */
	decStringList GetMemoryFileKeys() const;
	
	/** \brief Memory file with key or NULL if absent. */
	decMemoryFile *GetMemoryFileNamed(const char *name) const;
	
	/** \brief Memory file with key is present. */
	bool HasMemoryFileNamed(const char *name) const;
	
	/**
	 * \brief Add memory file with key.
	 * \throws deeInvalidParam \em name is NULL.
	 * \throws deeInvalidParam \em file is NULL.
	 * \throws deeInvalidParam Memory file with \em name is already present.
	 */
	void AddMemoryFile(const char *name, decMemoryFile *file);
	
	/**
	 * \brief Remove memory file with key.
	 * \throws deeInvalidParam \em name is NULL.
	 * \throws deeInvalidParam Memory file with \em name is absent.
	 */
	void RemoveMemoryFile(const char *name);
	
	/** \brief Remove all memory files. */
	void RemoveAllMemoryFiles();
	/*@}*/
};

#endif
