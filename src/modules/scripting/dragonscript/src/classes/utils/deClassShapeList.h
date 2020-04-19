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

#ifndef _DECLASSSHAPELIST_H_
#define _DECLASSSHAPELIST_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;
class decShapeList;
class decShapeHull;



/**
 * @brief Shape List Script Class.
 */
class deClassShapeList : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassShapeList( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassShapeList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	/** Retrieves the shape list stored in a real object. */
	const decShapeList &GetShapeList( dsRealObject *myself ) const;
	/** Pushes a shape list onto the stack. */
	void PushShapeList( dsRunTime *rt, const decShapeList &shapeList );
	
	/** Retrieves the scripting module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	void HullAddPoints( dsRunTime &rt, decShapeHull &hull, dsValue &pointsArray ) const;
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsShaList, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsVec, *clsQuat, *clsVec2;
		dsClass *clsArray;
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
	DEF_NATFUNC( nfNew2 );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetCount );
	DEF_NATFUNC( nfEmpty );
	DEF_NATFUNC( nfNotEmpty );
	DEF_NATFUNC( nfRemoveAllShapes );
	
	DEF_NATFUNC( nfAddSphere );
	
	DEF_NATFUNC( nfAddBox );
	DEF_NATFUNC( nfAddBox2 );
	
	DEF_NATFUNC( nfAddCylinder );
	DEF_NATFUNC( nfAddCylinder2 );
	DEF_NATFUNC( nfAddCylinder3 );
	DEF_NATFUNC( nfAddCylinder4 );
	DEF_NATFUNC( nfAddCylinder5 );
	DEF_NATFUNC( nfAddCylinder6 );
	DEF_NATFUNC( nfAddCylinder7 );
	DEF_NATFUNC( nfAddCylinder8 );
	DEF_NATFUNC( nfAddCylinder9 );
	
	DEF_NATFUNC( nfAddCapsule );
	DEF_NATFUNC( nfAddCapsule2 );
	DEF_NATFUNC( nfAddCapsule3 );
	DEF_NATFUNC( nfAddCapsule4 );
	DEF_NATFUNC( nfAddCapsule5 );
	DEF_NATFUNC( nfAddCapsule6 );
	DEF_NATFUNC( nfAddCapsule7 );
	DEF_NATFUNC( nfAddCapsule8 );
	DEF_NATFUNC( nfAddCapsule9 );
	
	DEF_NATFUNC( nfAddHull );
	DEF_NATFUNC( nfAddHull2 );
	DEF_NATFUNC( nfAddHull3 );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
