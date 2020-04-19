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

#ifndef _DECLASSSKINBUILDER_H_
#define _DECLASSSKINBUILDER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief Skin builder script class.
 */
class deClassSkinBuilder : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSkinPropertyNodeCombineMode;
	dsClass *pClsSkinPropertyMappedInputType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSkinBuilder( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassSkinBuilder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	inline dsClass *GetClassSkinPropertyNodeCombineMode() const{ return pClsSkinPropertyNodeCombineMode; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkinBuilder;
		dsClass *clsVoid;
		dsClass *clsBoolean;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsSkin;
		dsClass *clsColor;
		dsClass *clsImage;
		dsClass *clsVideo;
		dsClass *clsPoint;
		dsClass *clsPoint3;
		dsClass *clsVector2;
		dsClass *clsFont;
		dsClass *clsCurveBezier;
		dsClass *clsSkinPropertyNodeCombineMode;
		dsClass *clsSkinPropertyMappedInputType;
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
	DEF_NATFUNC( nfBuildSkin );
	DEF_NATFUNC( nfAddTexture );
	DEF_NATFUNC( nfAddPropertyValue );
	DEF_NATFUNC( nfAddPropertyColor );
	DEF_NATFUNC( nfAddPropertyImage );
	DEF_NATFUNC( nfAddPropertyVideo );
	DEF_NATFUNC( nfAddPropertyConstructed );
	DEF_NATFUNC( nfAddPropertyMapped );
	DEF_NATFUNC( nfConstructedOpenContent );
	DEF_NATFUNC( nfAddNodeImage );
	DEF_NATFUNC( nfAddNodeText );
	DEF_NATFUNC( nfAddNodeShape );
	DEF_NATFUNC( nfAddNodeGroup );
	DEF_NATFUNC( nfNextNodeAsMask );
	DEF_NATFUNC( nfCloseNode );
#undef DEF_NATFUNC
};

#endif
