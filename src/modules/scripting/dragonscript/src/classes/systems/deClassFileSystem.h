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

#ifndef _DECLASSFILESYSTEM_H_
#define _DECLASSFILESYSTEM_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;
class decPath;


/**
 * @brief File System Script Class.
 */
class deClassFileSystem : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsFileType;
	dsClass *pClsFileExtension;
	dsClass *pClsResourceLoaderType;
	
	dsValue *pTypeFile;
	dsValue *pTypeDirectory;
	dsValue *pTypeSpecial;
	bool pTypesReady;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassFileSystem(deScriptingDragonScript *ds);
	/** Cleans up the class. */
	~deClassFileSystem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	inline dsClass *GetClassFileType() const{ return pClsFileType; }
	inline dsClass *GetClassFileExtension() const{ return pClsFileExtension; }
	inline dsClass *GetClassResourceLoaderType() const{ return pClsResourceLoaderType; }
	
	void PrepareTypes();
	
	inline dsValue *GetTypeFile() const{ return pTypeFile; }
	inline dsValue *GetTypeDirectory() const{ return pTypeDirectory; }
	inline dsValue *GetTypeSpecial() const{ return pTypeSpecial; }
	
	void BrowseNativeDirectory(const decPath &path) const;
	void OpenUrl(const char *url) const;
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsFileSys;
		dsClass *clsVoid;
		dsClass *clsByte;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsStr;
		dsClass *clsBlock;
		dsClass *clsFileType;
		dsClass *clsFileExtension;
		dsClass *clsResourceLoaderType;
		dsClass *clsArray;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfGetSeparator);
	DEF_NATFUNC(nfGetPathSeparator);
	DEF_NATFUNC(nfGetCurrentPrefix);
	DEF_NATFUNC(nfGetGamePath);
	
	DEF_NATFUNC(nfExistsFile);
	DEF_NATFUNC(nfCanReadFile);
	DEF_NATFUNC(nfCanWriteFile);
	DEF_NATFUNC(nfCanDeleteFile);
	DEF_NATFUNC(nfDeleteFile);
	DEF_NATFUNC(nfSearchFiles);
	DEF_NATFUNC(nfGetFileType);
	DEF_NATFUNC(nfPathMatchesPattern);
	DEF_NATFUNC(nfBrowseOverlay);
	DEF_NATFUNC(nfBrowseCapture);
	DEF_NATFUNC(nfBrowseConfig);
	DEF_NATFUNC(nfGetFileExtensions);
	DEF_NATFUNC(nfOpenUrl);
#undef DEF_NATFUNC
};

#endif
