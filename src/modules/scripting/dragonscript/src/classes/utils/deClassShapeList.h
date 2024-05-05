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
	dsClass *pClsShapeType;
	
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
	
	inline dsClass *GetClassShapeType() const{ return pClsShapeType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsShaList, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsObj, *clsBool;
		dsClass *clsVec, *clsQuat, *clsVec2;
		dsClass *clsArray;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
		dsClass *clsShapeType;
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
	
	DEF_NATFUNC( nfGetTypeAt );
	DEF_NATFUNC( nfGetPositionAt );
	DEF_NATFUNC( nfGetRadiusAt );
	DEF_NATFUNC( nfGetHalfExtendsAt );
	DEF_NATFUNC( nfGetOrientationAt );
	DEF_NATFUNC( nfGetHalfHeightAt );
	DEF_NATFUNC( nfGetTopRadiusAt );
	DEF_NATFUNC( nfGetBottomRadiusAt );
	DEF_NATFUNC( nfGetTopAxisScalingAt );
	DEF_NATFUNC( nfGetBottomAxisScalingAt );
	DEF_NATFUNC( nfGetPointCountAt );
	DEF_NATFUNC( nfGetPointAt );
	
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
