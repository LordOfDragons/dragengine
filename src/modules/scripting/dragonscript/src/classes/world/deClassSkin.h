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

#ifndef _DECLASSSKIN_H_
#define _DECLASSSKIN_H_

#include <libdscript/libdscript.h>

class deEngine;
class deSkin;
class deScriptingDragonScript;



/**
 * \brief Skin script class.
 */
class deClassSkin : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSkinPropertyType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassSkin( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassSkin();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Skin from object. */
	deSkin *GetSkin( dsRealObject *myself ) const;
	
	/** \brief Pushe skin. */
	void PushSkin( dsRunTime *rt, deSkin *skin );
	
	inline dsClass *GetClassSkinPropertyType() const{ return pClsSkinPropertyType; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkin;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsObject;
		
		dsClass *clsResourceListener;
		dsClass *clsColor;
		dsClass *clsImage;
		dsClass *clsVideo;
		dsClass *clsSkinPropertyType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfLoad );
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetFilename );
	DEF_NATFUNC( nfGetTextureCount );
	DEF_NATFUNC( nfIndexOfTextureNamed );
	
	DEF_NATFUNC( nfTextureGetName );
	DEF_NATFUNC( nfTextureGetPropertyCount );
	DEF_NATFUNC( nfTextureIndexOfPropertyNamed );
	
	DEF_NATFUNC( nfPropertyGetName );
	DEF_NATFUNC( nfPropertyGetRenderable );
	DEF_NATFUNC( nfPropertyGetType );
	DEF_NATFUNC( nfPropertyGetValue );
	DEF_NATFUNC( nfPropertyGetColor );
	DEF_NATFUNC( nfPropertyGetImage );
	DEF_NATFUNC( nfPropertyGetVideo );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
