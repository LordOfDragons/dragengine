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

#ifndef _DECLASSMODEL_H_
#define _DECLASSMODEL_H_

#include <libdscript/libdscript.h>

class deEngine;
class deModel;
class deScriptingDragonScript;
class deClassResourceListener;



/**
 * \brief Model script class.
 */
class deClassModel : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassModel( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassModel();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the model from an object. */
	deModel *GetModel( dsRealObject *myself ) const;
	/** \brief Pushes a model. */
	void PushModel( dsRunTime *rt, deModel *model );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsMdl, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsVec, *clsRN;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfLoad );
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	
	DEF_NATFUNC( nfGetLodCount );
	DEF_NATFUNC( nfGetTextureCount );
	DEF_NATFUNC( nfGetTextureNameAt );
	DEF_NATFUNC( nfGetFaceCount );
	DEF_NATFUNC( nfGetVertexCount );
	DEF_NATFUNC( nfGetMinimumExtend );
	DEF_NATFUNC( nfGetMaximumExtend );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
