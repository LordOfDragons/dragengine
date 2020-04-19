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

#ifndef _DECLASSLAYERMASK_H_
#define _DECLASSLAYERMASK_H_

#include <libdscript/libdscript.h>

class decLayerMask;
class deScriptingDragonScript;



/**
 * \brief Layer Mask Script Class.
 */
class deClassLayerMask : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassLayerMask( deScriptingDragonScript *ds );
	/** \brief Cleans up the class. */
	virtual ~deClassLayerMask();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	/** \brief Creates the class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Retrieves the layer stored in the given real object. */
	const decLayerMask &GetLayerMask( dsRealObject *myself ) const;
	/** \brief Pushes the given layermask onto the stack. */
	void PushLayerMask( dsRunTime *rt, const decLayerMask &layerMask );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsLyM, *clsVoid, *clsInt, *clsStr, *clsObj, *clsBool;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNew2 );
	DEF_NATFUNC( nfNewWith );
	DEF_NATFUNC( nfNewWith2 );
	DEF_NATFUNC( nfNewWith3 );
	DEF_NATFUNC( nfNewWith4 );
	DEF_NATFUNC( nfNewAll );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfClearMask );
	DEF_NATFUNC( nfSetBit );
	DEF_NATFUNC( nfClearBit );
	DEF_NATFUNC( nfIsBitSet );
	DEF_NATFUNC( nfIsBitCleared );
	DEF_NATFUNC( nfIsEmpty );
	
	DEF_NATFUNC( nfToBitString );
	DEF_NATFUNC( nfSetFromBitString );
	DEF_NATFUNC( nfToHexString );
	DEF_NATFUNC( nfSetFromHexString );
	
	DEF_NATFUNC( nfMatches );
	DEF_NATFUNC( nfMatchesNot );
	
	DEF_NATFUNC( nfOpInverse );
	DEF_NATFUNC( nfOpAnd );
	DEF_NATFUNC( nfOpOr );
	DEF_NATFUNC( nfOpXor );
	DEF_NATFUNC( nfOpAndAssign );
	DEF_NATFUNC( nfOpOrAssign );
	DEF_NATFUNC( nfOpXorAssign );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

#endif
