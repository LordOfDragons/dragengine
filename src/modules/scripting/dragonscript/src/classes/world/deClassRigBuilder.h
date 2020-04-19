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

#ifndef _DECLASSRIGBUILDER_H_
#define _DECLASSRIGBUILDER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief Rig builder script class.
 */
class deClassRigBuilder : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsColliderConstraintDof;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassRigBuilder( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassRigBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	inline dsClass *GetClassColliderConstraintDof() const{ return pClsColliderConstraintDof; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsRigBuilder;
		dsClass *clsVoid;
		dsClass *clsBoolean;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsRig;
		dsClass *clsVector;
		dsClass *clsQuaternion;
		dsClass *clsShapeList;
		dsClass *clsColliderConstraintDof;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfBuild );
	DEF_NATFUNC( nfBuildRig );
	DEF_NATFUNC( nfSetCentralMassPoint );
	DEF_NATFUNC( nfSetModelCollision );
	DEF_NATFUNC( nfAddBone );
	DEF_NATFUNC( nfAddBoneShapeProperty );
	DEF_NATFUNC( nfAddBoneConstraint );
	DEF_NATFUNC( nfSetBoneConstraintDof );
	DEF_NATFUNC( nfSetRootBone );
	DEF_NATFUNC( nfSetShapes );
	DEF_NATFUNC( nfAddShapeProperty );
#undef DEF_NATFUNC
};

#endif
