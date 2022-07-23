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

// include only once
#ifndef _DEDSCLASSCURVEBEZIER_H_
#define _DEDSCLASSCURVEBEZIER_H_

// includes
#include <libdscript/libdscript.h>

#include <dragengine/common/curve/decCurveBezier.h>

// predefinitions
class deScriptingDragonScript;



/**
 * @brief Bezier-Curve Script Class.
 * Native script class providing access to Bezier-Curves.
 */
class deClassCurveBezier : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsCurveBezierInterpolation;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassCurveBezier( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassCurveBezier();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the curve. */
	const decCurveBezier &GetCurve( dsRealObject *myself ) const;
	/** Pushes the curve. */
	void PushCurve( dsRunTime *rt, const decCurveBezier &curve );
	
	inline dsClass *GetClassCurveBezierInterpolation() const{ return pClsCurveBezierInterpolation; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCBe, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsVec2;
		dsClass *clsCurveBezierInterpolation;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfNewDefaultLinear );
	DEF_NATFUNC( nfNewDefaultBezier );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPointCount );
	DEF_NATFUNC( nfGetPointAt );
	DEF_NATFUNC( nfGetHandle1At );
	DEF_NATFUNC( nfGetHandle2At );
	DEF_NATFUNC( nfFindPointPriorTo );
	DEF_NATFUNC( nfAddPoint );
	DEF_NATFUNC( nfAddPoint2 );
	DEF_NATFUNC( nfAddPoint3 );
	DEF_NATFUNC( nfAddPoint4 );
	DEF_NATFUNC( nfRemovePointFrom );
	DEF_NATFUNC( nfRemoveAllPoints );
	DEF_NATFUNC( nfGetInterpolationMode );
	DEF_NATFUNC( nfSetInterpolationMode );
	DEF_NATFUNC( nfSetDefaultLinear );
	DEF_NATFUNC( nfSetDefaultBezier );
	
	DEF_NATFUNC( nfEvaluateAt );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

// end of include only once
#endif
