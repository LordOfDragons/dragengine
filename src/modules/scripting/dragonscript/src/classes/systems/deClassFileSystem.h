/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	deClassFileSystem( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	~deClassFileSystem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	inline dsClass *GetClassFileType() const{ return pClsFileType; }
	inline dsClass *GetClassFileExtension() const{ return pClsFileExtension; }
	inline dsClass *GetClassResourceLoaderType() const{ return pClsResourceLoaderType; }
	
	void PrepareTypes();
	
	inline dsValue *GetTypeFile() const{ return pTypeFile; }
	inline dsValue *GetTypeDirectory() const{ return pTypeDirectory; }
	inline dsValue *GetTypeSpecial() const{ return pTypeSpecial; }
	
	void BrowseNativeDirectory( const decPath &path ) const;
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
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfGetSeparator );
	DEF_NATFUNC( nfGetPathSeparator );
	DEF_NATFUNC( nfGetCurrentPrefix );
	DEF_NATFUNC( nfGetGamePath );
	
	DEF_NATFUNC( nfExistsFile );
	DEF_NATFUNC( nfCanReadFile );
	DEF_NATFUNC( nfCanWriteFile );
	DEF_NATFUNC( nfCanDeleteFile );
	DEF_NATFUNC( nfDeleteFile );
	DEF_NATFUNC( nfSearchFiles );
	DEF_NATFUNC( nfGetFileType );
	DEF_NATFUNC( nfPathMatchesPattern );
	DEF_NATFUNC( nfBrowseOverlay );
	DEF_NATFUNC( nfBrowseCapture );
	DEF_NATFUNC( nfBrowseConfig );
	DEF_NATFUNC( nfGetFileExtensions );
#undef DEF_NATFUNC
};

#endif
