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

#ifndef _MEHEIGHTTERRAINPROPFIELD_H_
#define _MEHEIGHTTERRAINPROPFIELD_H_

#include "../heightterrain/meHTVInstance.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/propfield/dePropField.h>

class meHeightTerrainSector;
class meUpdateHeightTerrainPropField;

class deEngine;
class dePropFieldType;
class dePropFieldInstance;

class igdeEnvironment;



/**
 * \brief Height terrain prop field.
 */
class meHeightTerrainPropField : public deObject{
public:
	typedef deTObjectReference<meHeightTerrainPropField> Ref;
	typedef decTObjectOrderedSet<meHeightTerrainPropField> List;
	
	
private:
	meHeightTerrainSector *pHTSector;
	
	decVector2 pMinExtend;
	decVector2 pMaxExtend;
	
	deEngine *pEngine;
	dePropField::Ref pEngPF;
	
	meHTVInstance::List pVInstances;
	bool pDirtyVInstances;
	
	bool pDirty;
	bool pKeepClean;
	
	meUpdateHeightTerrainPropField *pListener;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	explicit meHeightTerrainPropField(deEngine *engine);
	
protected:
	/** \brief Clean up object. */
	~meHeightTerrainPropField() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent height terrain sector or \em nullptr. */
	inline meHeightTerrainSector *GetHTSector() const{ return pHTSector; }
	
	/** \brief Set parent height terrain sector or \em nullptr. */
	void SetHTSector(meHeightTerrainSector *htsector);
	
	
	
	/** \brief Minimum extend. */
	inline const decVector2 &GetMinimumExtend() const{ return pMinExtend; }
	
	/** \brief Maximum extend. */
	inline const decVector2 &GetMaximumExtend() const{ return pMaxExtend; }
	
	/** \brief Set extends. */
	void SetExtend(const decVector2 &minimum, const decVector2 &maximum);
	
	
	
	/** \brief Engine prop field. */
	inline const dePropField::Ref &GetEnginePropField() const{ return pEngPF; }
	
	
	
	/** \brief Init delegates. */
	void InitDelegates(igdeEnvironment *environment);
	
	/** \brief Invalidate prop field. */
	void Invalidate();
	
	/** \brief Validate prop field. */
	void Validate();
	
	
	
	/** \brief Update prop field. */
	void Update();
	
	/** \brief Clear prop field. */
	void Clear();
	
	
	
	/** \brief Rebuild engine prop field types. */
	void RebuildVegetationPropFieldTypes();
	
	
	
	/** \brief Prop field has to be kept clean. */
	inline bool GetKeepClean() const{ return pKeepClean; }
	
	/** Vegetation instances. */
	inline const meHTVInstance::List &GetVInstances() const{ return pVInstances; }
	
	/** \brief Add vegetation instance. */
	const meHTVInstance::Ref &AddVInstance();
	
	/** \brief Remove all vegetation instances. */
	void RemoveAllVInstances();
	
	/** \brief Update vegetation instances if dirty. */
	void UpdateVInstances();
	
	/** \brief Mark vegetation instances dirty. */
	void MarkVInstancesDirty();
	
	/** \brief Mark vegetation instances valid. */
	void MarkVInstancesValid();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
