/* 
 * FBX Modules
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

#ifndef _FBXSCENE_H_
#define _FBXSCENE_H_


class decBaseFileReader;
class decBaseFileWriter;
class deBaseModule;
class fbxNode;


/**
 * \brief FBX Scene.
 */
class fbxScene{
public:
	/** \brief Axis. */
	enum eAxis{
		eaXPos,
		eaXNeg,
		eaYPos,
		eaYNeg,
		eaZPos,
		eaZNeg
	};
	
	
	
private:
	int pVersion;
	fbxNode *pNode;
	
	// global settings
	eAxis pUpAxis;
	eAxis pFrontAxis;
	eAxis pCoordAxis;
	float pUnitScaleFactor;
	float pScaleFactor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create scene. */
	fbxScene();
	
	/** \brief Load scene. */
	fbxScene( decBaseFileReader &reader );
	
	/** \brief Clean up scene. */
	~fbxScene();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Version in the form 7300 = 7.3 . */
	inline int GetVersion() const{ return pVersion; }
	
	/** \brief Set version in the form 7300 = 7.3 . */
	void SetVersion( int version );
	
	
	
	/** \brief Root node. */
	inline fbxNode *GetNode() const{ return pNode; }
	
	/** \brief Scale factor. */
	inline float GetScaleFactor() const{ return pScaleFactor; }
	
	/** \brief Unit scale factor. */
	inline float GetUnitScaleFactor() const{ return pUnitScaleFactor; }
	
	
	
	/** \brief Prepare after reading. */
	void Prepare( deBaseModule &module );
	
	/** \brief Save to file. */
	void Save( decBaseFileWriter &writer );
	
	/** \brief Debug print scene structure. */
	void DebugPrintStructure( deBaseModule &logger, bool verbose = false ) const;
	/*@}*/
	
	
	
private:
	eAxis pGetAxis( int axisType, int axisSign ) const;
};

#endif
