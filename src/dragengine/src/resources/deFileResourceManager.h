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

#ifndef _DEFILERESOURCEMANAGER_H_
#define _DEFILERESOURCEMANAGER_H_

#include "deResourceManager.h"
#include "../common/utils/decDateTime.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"

class decPath;
class deVirtualFileSystem;


/**
 * \brief Base File Resource Manager Class.
 *
 * Provides basic support for resource managers. The actual management
 * of resource the subclass should implement using the resource list
 * class provided (although you are free to do differently if you really
 * know what you are doing). The RemoveResource function is protected
 * and has to be used only by a deResource derived object to notify the
 * resource manager that this object has to be freed from memory. Do not
 * do any reference counting inside the subclass. If a resource is ready
 * to be freed it will tell you by calling RemoveResource.
 */
class DE_DLL_EXPORT deFileResourceManager : public deResourceManager{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file resource manager. */
	deFileResourceManager(deEngine *engine, eResourceType type);
	
	/** \brief Clean up resource manager. */
	~deFileResourceManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Path to file relative to base path.
	 * \returns true if found and \em path has been set otherwise false.
	 */
	bool FindFileForReading(decPath &path, const deVirtualFileSystem &vfs,
		const char *filename, const char *basePath) const;
	
	/** \brief Open file for reading using. */
	decBaseFileReader::Ref OpenFileForReading(const deVirtualFileSystem &vfs,
		const char *filename) const;
	
	/** \brief Open file for writing using. */
	decBaseFileWriter::Ref OpenFileForWriting(const deVirtualFileSystem &vfs,
		const char *filename) const;
	/*@}*/
};

#endif
