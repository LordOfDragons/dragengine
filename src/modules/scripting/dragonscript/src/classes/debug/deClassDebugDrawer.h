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
