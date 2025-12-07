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

#ifndef _IGDETEMPLATEFILE_H_
#define _IGDETEMPLATEFILE_H_

#include "igdeTemplateReplaceList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringList.h>



/**
 * \brief Project template file.
 */
class igdeTemplateFile : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTemplateFile> Ref;


	/** \brief Directories. */
	enum eDirectories{
		/** \brief Project data directory. */
		edData,
		
		/** \brief Project directory. */
		edProject
	};
	
	
	
private:
	decString pPath;
	decString pPathRename;
	decString pPattern;
	eDirectories pDirectory;
	igdeTemplateReplaceList pReplacements;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create template. */
	igdeTemplateFile();
	
	/** \brief Create template. */
	igdeTemplateFile(const igdeTemplateFile &file);
	
protected:
	/** \brief Clean up template. */
	virtual ~igdeTemplateFile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief File path relative to template base path. */
	inline const decString &GetPath() const{return pPath;}
	
	/** \brief Set file path relative to template base path. */
	void SetPath(const char *path);
	
	/** \brief File path rename relative to template base path. */
	inline const decString &GetPathRename() const{return pPathRename;}
	
	/** \brief Set file path rename relative to template base path. */
	void SetPathRename(const char *path);
	
	/** \brief File pattern relative to template base path. */
	inline const decString &GetPattern() const{return pPattern;}
	
	/** \brief Set file pattern relative to template base path. */
	void SetPattern(const char *pattern);
	
	/** \brief Driectory. */
	inline eDirectories GetDirectory() const{return pDirectory;}
	
	/** \brief Set directory. */
	void SetDirectory(eDirectories directory);
	
	/** \brief Replacements. */
	inline igdeTemplateReplaceList &GetReplacements(){return pReplacements;}
	inline const igdeTemplateReplaceList &GetReplacements() const{return pReplacements;}
	/*@}*/
};

#endif
