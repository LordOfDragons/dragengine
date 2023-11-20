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
#ifndef _DECLASSDYNAMICSKIN_H_
#define _DECLASSDYNAMICSKIN_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deScriptingDragonScript;
class deDynamicSkin;



/**
 * @brief Dynamic Skin Script Class.
 *
 * Dynamic Skin Script Class.
 */
class deClassDynamicSkin : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsDynamicSkinRenderableType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassDynamicSkin( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassDynamicSkin();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the dynamic skin from an object. */
	deDynamicSkin *GetDynamicSkin( dsRealObject *myself ) const;
	/** Pushes a dynamic skin. */
	void PushDynamicSkin( dsRunTime *rt, deDynamicSkin *dynamicSkin );
	
	inline dsClass *GetClassDynamicSkinRenderableType() const{ return pClsDynamicSkinRenderableType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsDSkin, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsClr, *clsImg, *clsCam, *clsVP, *clsCView;
		dsClass *clsDynamicSkinRenderableType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetRenderableCount );
	DEF_NATFUNC( nfIndexOfRenderable );
	DEF_NATFUNC( nfHasRenderable );
	DEF_NATFUNC( nfAddRenderable );
	DEF_NATFUNC( nfRemoveRenderable );
	DEF_NATFUNC( nfRemoveAllRenderables );
	
	DEF_NATFUNC( nfGetTypeAt );
	
	DEF_NATFUNC( nfGetValueAt );
	DEF_NATFUNC( nfSetValueAt );
	
	DEF_NATFUNC( nfGetColorAt );
	DEF_NATFUNC( nfSetColorAt );
	
	DEF_NATFUNC( nfGetImageAt );
	DEF_NATFUNC( nfSetImageAt );
	
	DEF_NATFUNC( nfGetCanvasAt );
	DEF_NATFUNC( nfSetCanvasAt );
	DEF_NATFUNC( nfSetCanvasAt2 );
	DEF_NATFUNC( nfSetCanvasAt3 );
	
	DEF_NATFUNC( nfGetCameraAt );
	DEF_NATFUNC( nfSetCameraAt );
	
	DEF_NATFUNC( nfGetVideoPlayerAt );
	DEF_NATFUNC( nfSetVideoPlayerAt );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

// end of include only once
#endif
