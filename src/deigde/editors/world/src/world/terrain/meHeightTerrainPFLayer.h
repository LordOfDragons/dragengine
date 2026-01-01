/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _MEHEIGHTTERRAINPFLAYER_H_
#define _MEHEIGHTTERRAINPFLAYER_H_

#include "meHeightTerrainPFType.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class meHeightTerrainSector;
class meByteArray;

class deEngine;
class dePropFieldType;
class dePropFieldInstance;

class igdeEnvironment;



/**
 * Prop field type.
 */
class meHeightTerrainPFLayer : public deObject{
private:
	meHeightTerrainSector *pHTSector;
	
	deEngine *pEngine;
	
	decString pPathMask;
	meByteArray *pMask;
	bool pMaskChanged;
	bool pMaskSaved;
	
	meHeightTerrainPFType::List pTypes;
	
	
	
public:
	typedef deTObjectReference<meHeightTerrainPFLayer> Ref;
	typedef decTObjectOrderedSet<meHeightTerrainPFLayer> List;
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create object. */
	explicit meHeightTerrainPFLayer(deEngine *engine);
	
protected:
	/** Clean up object. */
	~meHeightTerrainPFLayer() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** Parent height terrain sector or \em nullptr. */
	inline meHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	
	/** Set parent height terrain sector or \em nullptr. */
	void SetHTSector(meHeightTerrainSector *htsector);
	
	
	
	/** Rebuild instances. */
	void RebuildInstances();
	/*@}*/
	
	
	
	/** \name Mask */
	/*@{*/
	/** Mask image path. */
	inline const decString &GetPathMask() const{ return pPathMask; }
	
	/** Set mask image path. */
	void SetPathMask(const char *path);
	
	/** Mask image has changed. */
	inline bool GetMaskChanged() const{ return pMaskChanged; }
	
	/** Set if mask image has changed. */
	void SetMaskChanged(bool changed);
	
	/** Mask image has been saved. */
	inline bool GetMaskSaved() const{ return pMaskSaved; }
	
	/** Set if mask image has been saved. */
	void SetMaskSaved(bool saved);
	
	
	
	/** Mask bytes. */
	inline meByteArray *GetMask() const{ return pMask; }
	
	/** Notify mask changed. */
	void NotifyMaskChanged();
	
	/** Load mask from stored path. */
	void LoadMaskFromImage();
	
	/** Height terrain sector size or resolution changed. */
	void SectorSizeOrResChanged();
	/*@}*/
	
	
	
	/** \name Types */
	/*@{*/
	/** Types. */
	inline const meHeightTerrainPFType::List &GetTypes() const{ return pTypes; }
	
	/** Add type. */
	void AddType(meHeightTerrainPFType *type);
	
	/** Remove type. */
	void RemoveType(meHeightTerrainPFType *type);
	
	/** Remove all types. */
	void RemoveAllTypes();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pResizeMask();
};

#endif
