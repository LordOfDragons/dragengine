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

#ifndef _DECLASSCOLLISIONFILTER_H_
#define _DECLASSCOLLISIONFILTER_H_

#include <libdscript/libdscript.h>

class decCollisionFilter;
class deScriptingDragonScript;



/**
 * \brief Immutable collision filter script class. Wraps a constant decCollisionFilter instance.
 */
class deClassCollisionFilter : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassCollisionFilter( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassCollisionFilter();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	/** \brief Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the collision filter in the given real object. */
	const decCollisionFilter &GetCollisionFilter( dsRealObject *myself ) const;
	/** \brief Pushes the collision filter onto the stack. */
	void PushCollisionFilter( dsRunTime *rt, const decCollisionFilter &collisionFilter );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCF, *clsLyM, *clsVoid, *clsInt, *clsStr, *clsObj, *clsBool;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfNewMask );
	DEF_NATFUNC( nfNewCategoryFilter );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetCategory );
	DEF_NATFUNC( nfGetFilter );
	
	DEF_NATFUNC( nfCollides );
	DEF_NATFUNC( nfCollidesNot );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

#endif
