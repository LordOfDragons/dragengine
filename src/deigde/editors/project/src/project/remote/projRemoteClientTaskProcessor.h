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

#ifndef _PROJREMOTECLIENTTASKPROCESSOR_H_
#define _PROJREMOTECLIENTTASKPROCESSOR_H_

#include "projRemoteServer.h"

#include <deremotelauncher/processor/derlTaskProcessorRemoteClient.h>

#include <dragengine/common/file/decBaseFileReader.h>

class projRemoteClient;
class decPath;


/**
 * \brief Remote client task processor.
 */
class projRemoteClientTaskProcessor : public derlTaskProcessorRemoteClient{
private:
	projRemoteServer::TaskProfileData::Ref pTaskProfileData;
	decBaseFileReader::Ref pVfsFileReader;
	std::string pVfsFilePath;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create task processor. */
	projRemoteClientTaskProcessor(projRemoteClient &client);
	
	/** \brief Clean up task processor. */
	~projRemoteClientTaskProcessor() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List all files in directory. */
	void ListDirectoryFiles(ListDirEntries &entries, const std::string &pathDir) override;
	
	/** \brief Truncate file. */
	void TruncateFile(const std::string &path) override;
	
	/** \brief Open file for reading or writing. */
	void OpenFile(const std::string &path, bool write) override;
	
	/** \brief Get size of open file. */
	uint64_t GetFileSize() override;
	
	/** \brief Read data from open file. */
	void ReadFile(void *data, uint64_t offset, uint64_t size) override;
	
	/** \brief Close open file. */
	void CloseFile() override;
	/*@}*/
	
	
protected:
	/**
	 * \brief Prepare run task.
	 * \note Client mutex is held while this method is called.
	 */
	void PrepareRunTask() override;
	
	
private:
	bool pExcludedByBaseGameDefPath(const decPath &path);
	bool pExcludedByPattern(const decPath &path);
};

#endif
