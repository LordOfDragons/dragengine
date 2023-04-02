/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLEXTENSIONS_H_
#define _DEOGLEXTENSIONS_H_

#include <dragengine/common/string/decStringList.h>

class deoglRenderThread;



/**
 * OpenGL Extensions.
 * Manages everything related to opengl extensions. Determines which
 * extensions exist on the system as well as fetching functions.
 */
class deoglExtensions{
public:
	/** Vendor. */
	enum eVendors{
		/** ATI / AMD. */
		evATI,

		/** nVidia. */
		evNVidia,

		/** Something else. */
		evUnknown
	};

	static const int VendorCount = evUnknown + 1;
	
	/** OpenGL Versions. */
	enum eVersions{
		/**
		 * Version 1.1 . Includes:<ul>
		 * <li>EXT_vertex_array</li>
		 * <li>EXT_polygon_offset</li>
		 * <li>EXT_blend_logic_op</li>
		 * <li>EXT_texture</li>
		 * <li>EXT_copy_texture</li>
		 * <li>EXT_subtexture</li>
		 * <li>EXT_texture_object</li>
		 * </ul>
		 */
		evgl1p1,
		
		/**
		 * Version 1.2 . Includes:<ul>
		 * <li>EXT_texture3D</li>
		 * <li>GL_EXT_blend_color</li>
		 * <li>EXT_bgra</li>
		 * <li>EXT_packed_pixels</li>
		 * <li>EXT_rescale_normal</li>
		 * <li>EXT_separate_specular_color</li>
		 * <li>SGIS_texture_edge_clamp</li>
		 * <li>SGIS_texture_lod</li>
		 * <li>EXT_draw_range_elements</li>
		 * </ul>
		 * Typically grouped by ARB_imaging extension.
		 */
		evgl1p2,
		
		/**
		 * Version 1.3 . Includes:<ul>
		 * <li>ARB_texture_compression</li>
		 * <li>ARB_texture_cube_map</li>
		 * <li>ARB_multisample</li>
		 * <li>ARB_multitexture</li>
		 * <li>ARB_texture_env_add</li>
		 * <li>ARB_texture_env_combine</li>
		 * <li>ARB_texture_env_dot3</li>
		 * <li>ARB_texture_border_clamp</li>
		 * <li>ARB_transpose_matrix</li>
		 * <li>EXT_blend_minmax</li>
		 * </ul>
		 */
		evgl1p3,
		
		/**
		 * Version 1.4 . Includes:<ul>
		 * <li>SGIS_generate_mipmap</li>
		 * <li>NV_blend_square</li>
		 * <li>EXT_blend_color</li>
		 * <li>EXT_blend_subtract</li>
		 * <li>ARB_depth_texture</li>
		 * <li>ARB_shadow</li>
		 * <li>EXT_fog_coord</li>
		 * <li>EXT_multi_draw_arrays</li>
		 * <li>ARB_point_parameters</li>
		 * <li>EXT_secondary_color</li>
		 * <li>EXT_blend_func_separate</li>
		 * <li>EXT_stencil_wrap</li>
		 * <li>ARB_texture_env_crossbar</li>
		 * <li>ARB_texture_mirrored_repeat</li>
		 * <li>ARB_window_pos</li>
		 * </ul>
		 */
		evgl1p4,
		
		/**
		 * Version 1.5 . Includes<ul>
		 * <li>ARB_vertex_buffer_object</li>
		 * <li>ARB_occlusion_query</li>
		 * <li>EXT_shadow_funcs</li>
		 * </ul>
		 */
		evgl1p5,
		
		/**
		 * Version 2.0 . Includes<ul>
		 * <li>ARB_shading_language_100</li>
		 * <li>ARB_shader_objects</li>
		 * <li>ARB_vertex_shader</li>
		 * <li>ARB_fragment_shader</li>
		 * <li>ARB_draw_buffers</li>
		 * <li>ARB_texture_non_power_of_two</li>
		 * <li>ARB_point_sprite</li>
		 * <li>EXT_blend_equation_separate</li>
		 * <li>EXT_stencil_two_side</li>
		 * </ul>
		 */
		evgl2p0,
		
		/**
		 * Version 2.1 . Includes<ul>
		 * <li>ARB_pixel_buffer_object</li>
		 * <li>EXT_texture_sRGB</li>
		 * </ul>
		 */
		evgl2p1,
		
		/**
		 * Version 3.0 . Includes<ul>
		 * <li>EXT_gpu_shader4</li>
		 * <li>EXT_framebuffer_object</li>
		 * <li>EXT_framebuffer_blit</li>
		 * <li>ARB_texture_float</li>
		 * <li>EXT_texture_integer</li>
		 * <li>EXT_texture_compression_rgtc</li>
		 * </ul>
		 */
		evgl3p0,
		
		/**
		 * Version 3.1 . Includes<ul>
		 * <li>ARB_draw_instanced</li>
		 * <li>EXT_copy_buffer</li>
		 * <li>ARB_texture_buffer_object</li>
		 * <li>ARB_texture_rectangle</li>
		 * <li>ARB_uniform_buffer_object</li>
		 * <li>NV_primitive_restart</li>
		 * </ul>
		 */
		evgl3p1,
		
		/**
		 * Version 3.2 . Includes<ul>
		 * <li>ARB_geometry_shader4</li>
		 * <li>ARB_sync</li>
		 * <li>ARB_vertex_array_bgra</li>
		 * <li>ARB_draw_elements_base_vertex</li>
		 * <li>ARB_fragment_coord_conventions</li>
		 * <li>ARB_provoking_vertex</li>
		 * <li>ARB_seamless_cube_map</li>
		 * <li>ARB_texture_multisample</li>
		 * <li>ARB_depth_clamp</li>
		 * </ul>
		 */
		evgl3p2,
		
		/**
		 * Version 3.3 . Includes<ul>
		 * <li>ARB_blend_func_extended</li>
		 * <li>ARB_explicit_attrib_location</li>
		 * <li>ARB_occlusion_query2</li>
		 * <li>ARB_sampler_objects</li>
		 * <li>ARB_texture_swizzle</li>
		 * <li>ARB_timer_query</li>
		 * <li>ARB_instanced_arrays</li>
		 * <li>ARB_texture_rgb10_a2ui</li>
		 * <li>ARB_vertex_type_2_10_10_10_rev</li>
		 * <li>GL_ARB_draw_elements_base_vertex</li>
		 * <li>GL_ARB_timer_query</li>
		 * </ul>
		 */
		evgl3p3,
		
		/**
		 * Version 4.0 . Includes<ul>
		 * <li>ARB_texture_cube_map_array</li>
		 * <li>ARB_tessellation_shader</li>
		 * <li>ARB_transform_feedback2</li>
		 * <li>ARB_transform_feedback3</li>
		 * <li>ARB_gpu_shader5</li>
		 * </ul>
		 */
		evgl4p0,
		
		/**
		 * Version 4.1 . Includes<ul>
		 * <li>ARB_viewport_array</li>
		 */
		evgl4p1,
		
		/**
		 * Version 4.2 . Includes<ul>
		 * <li>ARB_transform_feedback_instanced</li>
		 */
		evgl4p2,
		
		/**
		 * Version 4.3 .
		 * <li>ARB_shader_storage_buffer_object</li>
		 * <li>ARB_program_interface_query</li>
		 * <li>ARB_draw_indirect</li>
		 * <li>ARB_multi_draw_indirect</li>
		 */
		evgl4p3,
		
		/** Version 4.4 . */
		evgl4p4,
		
		/**
		 * Version 4.5 . Includes
		 * <ul>
		 * <li>GL_ARB_clip_control</li>
		 * </ul>
		 */
		evgl4p5,
		
		/**
		 * Version 4.6 .
		 * <ul>
		 * <li>GL_ARB_shader_image_load_store</li>
		 * <li>ARB_indirect_parameters</li>
		 * <li>ARB_shader_draw_parameters</li>
		 * </ul>
		 */
		evgl4p6,
		
		/** Unknown version higher than 4.6 . */
		evglUnknown,
		
		/** Number of entries. */
		EVGL_COUNT
	};
	
	/** OpenGL ES versions. */
	enum eESVersions {
		/** OpenGL ES 2.x or earlier */
		evglesUnsupported,
		
		/** OpenGL ES 3.0 */
		evgles3p0,
		
		/** OpenGL ES 3.1 */
		evgles3p1,
		
		/** OpenGL ES 3.2 */
		evgles3p2,
		
		/** Unknown. */
		evglesUnknown
	};
	
	/** Extensions. */
	enum eExtensions{
		ext_ARB_copy_buffer,
		ext_ARB_copy_image,
		ext_ARB_debug_output,
		ext_ARB_draw_buffers,
		ext_ARB_draw_buffers_blend,
		ext_ARB_draw_elements_base_vertex,
		ext_ARB_draw_instanced,
		ext_ARB_framebuffer_object,
		ext_ARB_geometry_shader4,
		ext_ARB_gpu_shader5,
		ext_ARB_get_program_binary,
		ext_ARB_multisample,
		ext_ARB_multitexture,
		ext_ARB_occlusion_query,
		ext_ARB_provoking_vertex,
		ext_ARB_sampler_objects,
		ext_ARB_seamless_cube_map,
		ext_ARB_separate_shader_objects,
		ext_ARB_shader_objects,
		ext_ARB_tessellation_shader,
		ext_ARB_texture_buffer_object,
		ext_ARB_texture_compression,
		ext_ARB_texture_cube_map_array,
		ext_ARB_texture_multisample,
		ext_ARB_timer_query,
		ext_ARB_transform_feedback2,
		ext_ARB_transform_feedback3,
		ext_ARB_transform_feedback_instanced,
		ext_ARB_uniform_buffer_object,
		ext_ARB_vertex_array_object,
		ext_ARB_vertex_buffer_object,
		ext_ARB_vertex_program,
		ext_ARB_vertex_shader,
		ext_ARB_viewport_array,
		ext_ARB_clip_control,
		ext_ARB_shader_storage_buffer_object,
		ext_ARB_program_interface_query,
		ext_ARB_shader_image_load_store,
		ext_ARB_compute_shader,
		ext_ARB_draw_indirect,
		ext_ARB_multi_draw_indirect,
		ext_ARB_indirect_parameters,
		ext_ARB_bindless_texture,
		ext_ARB_fragment_layer_viewport,
		ext_ARB_shader_draw_parameters,
		ext_ARB_shader_viewport_layer_array,
		ext_ARB_depth_clamp,
		ext_ARB_shading_language_420pack,
		ext_ARB_shader_atomic_counters,
		ext_ARB_shader_atomic_counter_ops,
		ext_ARB_gpu_shader_fp64,
		ext_ARB_direct_state_access,
		ext_ARB_clear_buffer_object,
		ext_ARB_buffer_storage,
		
		ext_EXT_bindable_uniform,
		ext_EXT_blend_equation_separate,
		ext_EXT_blend_func_separate,
		ext_EXT_blend_minmax,
		ext_EXT_copy_texture,
		ext_EXT_draw_range_elements,
		ext_EXT_gpu_shader4,
		ext_EXT_multi_draw_arrays,
		ext_EXT_packed_depth_stencil,
		ext_EXT_polygon_offset,
		ext_EXT_subtexture,
		ext_EXT_texture3D,
		ext_EXT_texture_array,
		ext_EXT_texture_integer,
		ext_EXT_texture_object,
		ext_EXT_transform_feedback,
		
		ext_AMD_debug_output,
		ext_AMD_performance_monitor,
		ext_AMD_seamless_cubemap_per_texture,
		ext_AMD_vertex_shader_layer,
		
		ext_ATI_meminfo,
		ext_ATI_separate_stencil,
		
		ext_NV_copy_image,
		ext_NV_packed_depth_stencil,
		ext_NV_texture_barrier,
		ext_NV_transform_feedback,
		ext_NV_transform_feedback2,
		ext_NV_transform_feedback3,
		
		ext_KHR_debug,
		
		ext_GLX_EXT_swap_control,
		ext_GLX_EXT_swap_control_tear,
		
		ext_WGL_EXT_swap_control,
		ext_WGL_EXT_swap_control_tear,
		
		/** Dummy entry containing the number of extensions. */
		EXT_COUNT
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	static bool pInitialized;
	
	decString pStrVendor;
	decString pStrGLVersion;
	decStringList pStrListExtensions;
	
	eVendors pVendor;
	int pGLVersionMajor;
	int pGLVersionMinor;
	eVersions pGLVersion;
	eESVersions pGLESVersion;
	
	bool pHasExtension[ EXT_COUNT ];
	bool pDisableExtension[ EXT_COUNT ];
	
	bool pHasRequiredFunctions;
	
	bool pHasArrayCubeMap;
	bool pHasSeamlessCubeMap;
	bool pHasCopyImage;
	bool pSupportsGeometryShader;
	bool pSupportsGSInstancing;
	bool pSupportsComputeShader;
	bool pSupportsVSLayer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new object. */
	deoglExtensions( deoglRenderThread &renderThread );
	/** Cleans up the object. */
	~deoglExtensions();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/**
	 * Initializes the extensions. Determines existing extensions and
	 * fetches functions.
	 */
	void Initialize();
	/** Prints a summary of the findings. */
	void PrintSummary();
	
	/** Returns true if all required extensions are present. */
	bool VerifyPresence() const;
	
	/** Retrieves the vendor string. */
	inline const decString &GetStringVendor() const{ return pStrVendor; }

	/** Retrieves the opengl version string. */
	inline const decString &GetStringGLVersion() const{ return pStrGLVersion; }

	/** Retrieves the list of extension strings supported by the hardware. */
	inline const decStringList &GetStringListExtensions() const{ return pStrListExtensions; }
	
	/** Retrieves the vendor. */
	inline eVendors GetVendor() const{ return pVendor; }
	
	/** OpenGL major version. */
	inline int GetGLVersionMajor() const{ return pGLVersionMajor; }
	
	/** OpenGL minor version. */
	inline int GetGLVersionMinor() const{ return pGLVersionMinor; }
	
	/** OpenGL version. */
	inline eVersions GetGLVersion() const{ return pGLVersion; }
	
	/** OpenGL ES version. */
	inline eESVersions GetGLESVersion() const{ return pGLESVersion; }
	
	/** Determines if extensions exist. */
	inline bool GetHasExtension( eExtensions extension ) const{ return pHasExtension[ extension ]; }
	/** Retrieves the name of an extension. */
	const char *GetExtensionName( eExtensions extension ) const;
	
	/** Disable extension. */
	void DisableExtension( eExtensions extension );
	
	/** Determines if seamless cube maps are supported. */
	inline bool GetHasSeamlessCubeMap() const{ return pHasSeamlessCubeMap; }
	/** Determines if array cube maps are supported. */
	inline bool GetHasArrayCubeMap() const{ return pHasArrayCubeMap; }
	/** Determines if copy image is supported. */
	inline bool GetHasCopyImage() const{ return pHasCopyImage; }
	
	/** Geometry shader is supported. Required so always true. */
	// inline bool SupportsGeometryShader() const{ return pSupportsGeometryShader; }
	
	/** Geometry shader instancing is supported. */
	inline bool SupportsGSInstancing() const{ return pSupportsGSInstancing; }
	
	/** Compute shader is supported. Required so always true. */
	// inline bool SupportsComputeShader() const{ return pSupportsComputeShader; }
	
	/** Layer ID output is supported in vertex shaders. */
	inline bool SupportsVSLayer() const{ return pSupportsVSLayer; }
	/*@}*/
	
private:
	void pScanVendor();
	void pScanVersion();
	void pScanExtensions();
	void pDisableExtensions();
	
	void pFetchRequiredFunctions();
	void pFetchOptionalFunctions();
	void pFixBuggedFunctions();
	void pOptionalDisableExtensions();
	
	void pGetRequiredFunction( void **funcPointer, const char *funcName );
	void pGetRequiredFunction( void **funcPointer, const char *funcNameBase, const char *funcNameExtension );
	void pGetRequiredFunction( void **funcPointer, const char *funcNameBase,
		const char *funcNameExtension1, const char *funcNameExtension2 );
	void pGetRequiredFunctionExt( void **funcPointer, const char *funcName );
	void pGetRequiredFunctionArbExt( void **funcPointer, const char *funcName );
	
	void pGetOptionalFunction( void **funcPointer, const char *funcName, int extensionIndex );
	void pGetOptionalFunction( void **funcPointer, const char *funcNameBase,
		const char *funcNameExtension, int extensionIndex );
	void pGetOptionalFunction( void **funcPointer, const char *funcNameBase,
		const char *funcNameExtension1, const char *funcNameExtension2, int extensionIndex );
	void pGetOptionalFunctionExt( void **funcPointer, const char *funcName, int extensionIndex );
	void pGetOptionalFunctionArbExt( void **funcPointer, const char *funcName, int extensionIndex );
	
	bool pVerifyExtensionPresent( eExtensions extension ) const;
	bool pVerifyExtensionPresent( eExtensions extension1, eExtensions extension2 ) const;
};

#endif
