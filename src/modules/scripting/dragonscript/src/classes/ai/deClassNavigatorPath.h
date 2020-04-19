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

#ifndef _DEDSCLASSNAVIGATORPATH_H_
#define _DEDSCLASSNAVIGATORPATH_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deNavigatorPath;



/**
 * \brief Navigator path script class.
 */
class deClassNavigatorPath : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassNavigatorPath( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassNavigatorPath();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Navigator path or throws exception if myself is \em NULL. */
	deNavigatorPath &GetNavigatorPath( dsRealObject *myself ) const;
	
	/** \brief Push navigator path. */
	void PushNavigatorPath( dsRunTime *rt, const deNavigatorPath &path );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsNavPath;
		
		dsClass *clsBool;
		dsClass *clsFloat;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsString;
		dsClass *clsVoid;
		
		dsClass *clsDVector;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
		dsClass *clsDebugDrawer;
		dsClass *clsDMatrix;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetCount );
	DEF_NATFUNC( nfGetAt );
	DEF_NATFUNC( nfSetAt );
	DEF_NATFUNC( nfAdd );
	DEF_NATFUNC( nfAddPath );
	DEF_NATFUNC( nfRemoveFrom );
	DEF_NATFUNC( nfRemoveAll );
	DEF_NATFUNC( nfTransform );
	DEF_NATFUNC( nfTransformed );
	DEF_NATFUNC( nfGetLength );
	DEF_NATFUNC( nfGetLengthTo );
	DEF_NATFUNC( nfGetLengthFrom );
	DEF_NATFUNC( nfGetLengthBetween );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfUpdateDebugDrawer );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
