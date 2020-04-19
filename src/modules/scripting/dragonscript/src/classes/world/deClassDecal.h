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

#ifndef _DECLASSDECAL_H_
#define _DECLASSDECAL_H_

#include <libdscript/libdscript.h>

class deEngine;
class deDecal;
class deScriptingDragonScript;



/**
 * \brief Decal Script Class.
 */
class deClassDecal : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassDecal( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassDecal();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Decal from object. */
	deDecal *GetDecal( dsRealObject *myself ) const;
	
	/** \brief Pushe decal. */
	void PushDecal( dsRunTime *rt, deDecal *decal );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsDecal;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsBool;
		dsClass *clsObject;
		
		dsClass *clsVector;
		dsClass *clsQuaternion;
		dsClass *clsSkin;
		dsClass *clsDynamicSkin;
		dsClass *clsTextureMatrix2;
		dsClass *clsComponent;
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
	DEF_NATFUNC( nfGetSize );
	DEF_NATFUNC( nfSetSize );
	DEF_NATFUNC( nfGetTransform );
	DEF_NATFUNC( nfSetTransform );
	
	DEF_NATFUNC( nfGetSkin );
	DEF_NATFUNC( nfSetSkin );
	DEF_NATFUNC( nfGetTexture );
	DEF_NATFUNC( nfSetTexture );
	DEF_NATFUNC( nfGetDynamicSkin );
	DEF_NATFUNC( nfSetDynamicSkin );
	
	DEF_NATFUNC( nfGetVisible );
	DEF_NATFUNC( nfSetVisible );
	
	DEF_NATFUNC( nfGetParentComponent );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
