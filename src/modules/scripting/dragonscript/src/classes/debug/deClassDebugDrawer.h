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

#ifndef _DECLASSDEBUGDRAWER_H_
#define _DECLASSDEBUGDRAWER_H_

#include <libdscript/libdscript.h>

class deDebugDrawer;
class deScriptingDragonScript;



/**
 * \brief Debug drawer script class.
 */
class deClassDebugDrawer : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new script class. */
	deClassDebugDrawer( deScriptingDragonScript *ds );
	/** \brief Cleans up the script class. */
	virtual ~deClassDebugDrawer();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the debug drawer or NULL if myself is NULL. */
	deDebugDrawer *GetDebugDrawer( dsRealObject *myself ) const;
	/** \brief Pushes the debug drawer on the stack. debugDrawer can be NULL to push a null object. */
	void PushDebugDrawer( dsRunTime *rt, deDebugDrawer *debugDrawer );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsDD, *clsVoid, *clsInt, *clsStr, *clsObj, *clsBool, *clsFlt;
		dsClass *clsClr, *clsVec, *clsDVec, *clsQuat, *clsShapeList;
		dsClass *clsModel;
		dsClass *clsMatrix;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetScale );
	DEF_NATFUNC( nfSetScale );
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC( nfGetVisible );
	DEF_NATFUNC( nfSetVisible );
	DEF_NATFUNC( nfGetXRay );
	DEF_NATFUNC( nfSetXRay );
	
	DEF_NATFUNC( nfGetShapeCount );
	DEF_NATFUNC( nfSetShapeCount );
	
	DEF_NATFUNC( nfShapeSetPosition );
	DEF_NATFUNC( nfShapeSetOrientation );
	DEF_NATFUNC( nfShapeSetScale );
	
	DEF_NATFUNC( nfShapeSetShapes );
	DEF_NATFUNC( nfShapeRemoveAllShapes );
	
	DEF_NATFUNC( nfShapeAddFace );
	DEF_NATFUNC( nfShapeFaceAddVertex );
	DEF_NATFUNC( nfShapeFaceCalculateNormal );
	DEF_NATFUNC( nfShapeRemoveAllFaces );
	DEF_NATFUNC( nfShapeFinishedFaces );
	DEF_NATFUNC( nfShapeSetFacesFromModel );
	DEF_NATFUNC( nfShapeSetFacesFromModel2 );
	
	DEF_NATFUNC( nfShapeSetEdgeColor );
	DEF_NATFUNC( nfShapeSetFillColor );
#undef DEF_NATFUNC
};

#endif
