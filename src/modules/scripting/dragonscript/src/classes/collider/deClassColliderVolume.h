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

#ifndef _DECLASSCOLLIDERVOLUME_H_
#define _DECLASSCOLLIDERVOLUME_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class deColliderVolume;
class deScriptingDragonScript;



/**
 * \brief Collider script class.
 */
class deClassColliderVolume : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new script class. */
	deClassColliderVolume( deScriptingDragonScript &ds );
	/** \brief Clean up the script class. */
	virtual ~deClassColliderVolume();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Script module owning the script class. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create script class members. */
	virtual void CreateClassMembers( dsEngine *engine );
	
	/**
	 * \brief Retrieve collider from a script object.
	 * \details Returns NULL if \em myself is NULL.
	 */
	deColliderVolume *GetCollider( dsRealObject *myself ) const;
	
	/**
	 * \brief Push collider onto the stack.
	 * \details Pushes a \em null object to the stack if \em collider is NULL.
	 */
	void PushCollider( dsRunTime *rt, deColliderVolume *collider );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsColVol, *clsVoid, *clsInt, *clsFlt, *clsStr, *clsQuat;
		dsClass *clsVec, *clsObj, *clsBool, *clsDVec, *clsDMat, *clsShapeList;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetShape );
	DEF_NATFUNC( nfSetShape );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfEquals2 );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
