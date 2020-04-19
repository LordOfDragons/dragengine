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

#ifndef _DEDSCLASSSKYLAYER_H_
#define _DEDSCLASSSKYLAYER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deSky;



/**
 * \brief Sky layer script class.
 */
class deClassSkyLayer : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSkyLayerTargetType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSkyLayer( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassSkyLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Push layer. */
	void PushLayer( dsRunTime *rt, deSky *sky, int index );
	
	inline dsClass *GetClassSkyLayerTargetType() const{ return pClsSkyLayerTargetType; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkyLink;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsSky;
		dsClass *clsSkyBody;
		dsClass *clsSkyTarget;
		dsClass *clsColor;
		dsClass *clsQuaternion;
		dsClass *clsVector;
		dsClass *clsSkin;
		dsClass *clsSkyLayerTargetType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetSky );
	DEF_NATFUNC( nfGetLayerIndex );
	
	DEF_NATFUNC( nfGetOffset );
	DEF_NATFUNC( nfSetOffset );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetColor );
	DEF_NATFUNC( nfSetColor );
	DEF_NATFUNC( nfGetIntensity );
	DEF_NATFUNC( nfSetIntensity );
	DEF_NATFUNC( nfGetTransparency );
	DEF_NATFUNC( nfSetTransparency );
	DEF_NATFUNC( nfGetSkin );
	DEF_NATFUNC( nfSetSkin );
	
	DEF_NATFUNC( nfGetLightOrientation );
	DEF_NATFUNC( nfSetLightOrientation );
	DEF_NATFUNC( nfGetLightColor );
	DEF_NATFUNC( nfSetLightColor );
	DEF_NATFUNC( nfGetLightIntensity );
	DEF_NATFUNC( nfSetLightIntensity );
	DEF_NATFUNC( nfGetAmbientIntensity );
	DEF_NATFUNC( nfSetAmbientIntensity );
	
	DEF_NATFUNC( nfGetBodyCount );
	DEF_NATFUNC( nfSetBodyCount );
	DEF_NATFUNC( nfGetBodyAt );
	
	DEF_NATFUNC( nfGetMultiplyBySkyLight );
	DEF_NATFUNC( nfSetMultiplyBySkyLight );
	DEF_NATFUNC( nfGetMultiplyBySkyColor );
	DEF_NATFUNC( nfSetMultiplyBySkyColor );
	
	DEF_NATFUNC( nfGetTarget );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
