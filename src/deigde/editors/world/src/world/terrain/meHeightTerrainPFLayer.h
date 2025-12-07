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

#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class meHeightTerrainSector;
class meHeightTerrainPFType;
class meByteArray;

class deEngine;
class dePropFieldType;
class dePropFieldInstance;

class igdeEnvironment;



/**
 * \brief Prop field type.
 */
class meHeightTerrainPFLayer : public deObject{
private:
	meHeightTerrainSector *pHTSector;
	
	deEngine *pEngine;
	
	decString pPathMask;
	meByteArray *pMask;
	bool pMaskChanged;
	bool pMaskSaved;
	
	meHeightTerrainPFType **pTypes;
	int pTypeCount;
	int pTypeSize;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHeightTerrainPFLayer> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meHeightTerrainPFLayer(deEngine *engine);
	
	/** \brief Clean up object. */
	virtual ~meHeightTerrainPFLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent height terrain sector or \em NULL. */
	inline meHeightTerrainSector *GetHTSector() const{return pHTSector;}
	
	/** \brief Set parent height terrain sector or \em NULL. */
	void SetHTSector(meHeightTerrainSector *htsector);
	
	
	
	/** \brief Rebuild instances. */
	void RebuildInstances();
	/*@}*/
	
	
	
	/** \name Mask */
	/*@{*/
	/** \brief Mask image path. */
	inline const decString &GetPathMask() const{return pPathMask;}
	
	/** \brief Set mask image path. */
	void SetPathMask(const char *path);
	
	/** \brief Mask image has changed. */
	inline bool GetMaskChanged() const{return pMaskChanged;}
	
	/** \brief Set if mask image has changed. */
	void SetMaskChanged(bool changed);
	
	/** \brief Mask image has been saved. */
	inline bool GetMaskSaved() const{return pMaskSaved;}
	
	/** \brief Set if mask image has been saved. */
	void SetMaskSaved(bool saved);
	
	
	
	/** \brief Mask bytes. */
	inline meByteArray *GetMask() const{return pMask;}
	
	/** \brief Notify mask changed. */
	void NotifyMaskChanged();
	
	/** \brief Load mask from stored path. */
	void LoadMaskFromImage();
	
	/** \brief Height terrain sector size or resolution changed. */
	void SectorSizeOrResChanged();
	/*@}*/
	
	
	
	/** \name Types */
	/*@{*/
	/** \brief Number of types. */
	inline int GetTypeCount() const{return pTypeCount;}
	
	/** \brief Type at index. */
	meHeightTerrainPFType *GetTypeAt(int index) const;
	
	/** \brief Index of type or -1 if absent. */
	int IndexOfType(meHeightTerrainPFType *type) const;
	
	/** \brief Type exists. */
	bool HasType(meHeightTerrainPFType *type) const;
	
	/** \brief Add type. */
	void AddType(meHeightTerrainPFType *type);
	
	/** \brief Remove type. */
	void RemoveType(meHeightTerrainPFType *type);
	
	/** \brief Remove all types. */
	void RemoveAllTypes();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pResizeMask();
};

#endif
