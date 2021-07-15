/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEVKPIPELINECONFIGURATION_H_
#define _DEVKPIPELINECONFIGURATION_H_

#include "devkSpecialization.h"
#include "../devkBasics.h"

class devkDescriptorSetLayout;
class devkShaderModule;


/**
 * Vulkan pipeline.
 */
class devkPipelineConfiguration{
public:
	/** Type. */
	enum eType{
		etGraphics,
		etCompute,
		etRaytracing
	};
	
	
	
private:
	eType pType;
	devkDescriptorSetLayout *pDescriptorSetLayout;
	
	devkShaderModule *pShaderVertex;
	devkShaderModule *pShaderTessellationControl;
	devkShaderModule *pShaderTessellationEvaluation;
	devkShaderModule *pShaderGeometry;
	devkShaderModule *pShaderFragment;
	
	devkShaderModule *pShaderCompute;
	
	devkShaderModule *pShaderRayGen;
	devkShaderModule *pShaderAnyHit;
	devkShaderModule *pShaderClosestHit;
	devkShaderModule *pShaderMiss;
	devkShaderModule *pShaderIntersection;
	devkShaderModule *pShaderCallable;
	
	devkShaderModule *pShaderTask;
	devkShaderModule *pShaderMesh;
	
	devkSpecialization::Ref pSpecialization;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline configuration. */
	devkPipelineConfiguration();
	
	/** Create copy of pipeline configuration. */
	devkPipelineConfiguration( const devkPipelineConfiguration &configuration );
	
	/** Clean up pipeline configuration. */
	~devkPipelineConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Type. */
	inline eType GetType() const{ return pType; }
	
	/** Set type. */
	void SetType( eType type );
	
	/** Descriptor set layout. */
	inline devkDescriptorSetLayout *GetDescriptorSetLayout() const{ return pDescriptorSetLayout; }
	
	/** Set descriptor set layout. */
	void SetDescriptorSetLayout( devkDescriptorSetLayout *layout );
	
	
	
	/** Vertex shader module or nullptr. */
	inline devkShaderModule *GetShaderVertex() const{ return pShaderVertex; }
	
	/** Set vertex shader module or nullptr. */
	void SetShaderVertex( devkShaderModule *shader );
	
	/** Tessellation control shader module or nullptr. */
	inline devkShaderModule *GetShaderTessellationControl() const{ return pShaderTessellationControl; }
	
	/** Set tessellation control shader module or nullptr. */
	void SetShaderTessellationControl( devkShaderModule *shader );
	
	/** Tessellation evaluation shader module or nullptr. */
	inline devkShaderModule *GetShaderTessellationEvaluation() const{ return pShaderTessellationEvaluation; }
	
	/** Set tessellation evaluation shader module or nullptr. */
	void SetShaderTessellationEvaluation( devkShaderModule *shader );
	
	/** Geometry shader module or nullptr. */
	inline devkShaderModule *GetShaderGeometry() const{ return pShaderGeometry; }
	
	/** Set geometry shader module or nullptr. */
	void SetShaderGeometry( devkShaderModule *shader );
	
	/** Fragment shader module or nullptr. */
	inline devkShaderModule *GetShaderFragment() const{ return pShaderFragment; }
	
	/** Set fragment shader module or nullptr. */
	void SetShaderFragment( devkShaderModule *shader );
	
	
	
	/** Compute shader module or nullptr. */
	inline devkShaderModule *GetShaderCompute() const{ return pShaderCompute; }
	
	/** Set compute shader module or nullptr. */
	void SetShaderCompute( devkShaderModule *shader );
	
	
	
	/** Ray generation shader module or nullptr. */
	inline devkShaderModule *GetShaderRayGen() const{ return pShaderRayGen; }
	
	/** Set ray generation shader module or nullptr. */
	void SetShaderRayGen( devkShaderModule *shader );
	
	/** AnyHit shader module or nullptr. */
	inline devkShaderModule *GetShaderAnyHit() const{ return pShaderAnyHit; }
	
	/** Set any hit shader module or nullptr. */
	void SetShaderAnyHit( devkShaderModule *shader );
	
	/** ClosestHit shader module or nullptr. */
	inline devkShaderModule *GetShaderClosestHit() const{ return pShaderClosestHit; }
	
	/** Set closest hit shader module or nullptr. */
	void SetShaderClosestHit( devkShaderModule *shader );
	
	/** Miss shader module or nullptr. */
	inline devkShaderModule *GetShaderMiss() const{ return pShaderMiss; }
	
	/** Set miss shader module or nullptr. */
	void SetShaderMiss( devkShaderModule *shader );
	
	/** Intersection shader module or nullptr. */
	inline devkShaderModule *GetShaderIntersection() const{ return pShaderIntersection; }
	
	/** Set intersection shader module or nullptr. */
	void SetShaderIntersection( devkShaderModule *shader );
	
	/** Callable shader module or nullptr. */
	inline devkShaderModule *GetShaderCallable() const{ return pShaderCallable; }
	
	/** Set callable shader module or nullptr. */
	void SetShaderCallable( devkShaderModule *shader );
	
	
	
	/** Task shader module or nullptr. */
	inline devkShaderModule *GetShaderTask() const{ return pShaderTask; }
	
	/** Set task shader module or nullptr. */
	void SetShaderTask( devkShaderModule *shader );
	
	/** Mesh shader module or nullptr. */
	inline devkShaderModule *GetShaderMesh() const{ return pShaderMesh; }
	
	/** Set mesh shader module or nullptr. */
	void SetShaderMesh( devkShaderModule *shader );
	
	
	
	/** Specialization. */
	inline devkSpecialization *GetSpecialization() const{ return pSpecialization; }
	
	/** Set specialization. */
	void SetSpecialization( devkSpecialization *specialization );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==( const devkPipelineConfiguration &configuration ) const;
	
	/** Copy configuration. */
	devkPipelineConfiguration &operator=( const devkPipelineConfiguration &configuration );
	/*@}*/
};

#endif

