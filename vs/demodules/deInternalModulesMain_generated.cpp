/* Generated File. Do not edit */
#include <dragengine/systems/deModuleSystem.h>

extern deInternalModule *deadRegisterInternalModule(deModuleSystem*);
extern deInternalModule *dearRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deaiRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deanimRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deanRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deoalRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deCRRegisterInternalModule(deModuleSystem*);
extern deInternalModule *decrbRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deFontRegisterInternalModule(deModuleSystem*);
extern deInternalModule *degnRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deoglRegisterInternalModule(deModuleSystem*);
extern deInternalModule *depngRegisterInternalModule(deModuleSystem*);
extern deInternalModule *depng3DRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deTgaRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deJpegRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deIesRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deWebpRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deWebp3DRegisterInternalModule(deModuleSystem*);
extern deInternalModule *dewiRegisterInternalModule(deModuleSystem*);
extern deInternalModule *delpRegisterInternalModule(deModuleSystem*);
extern deInternalModule *demdlRegisterInternalModule(deModuleSystem*);
extern deInternalModule *denbRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deoccmRegisterInternalModule(deModuleSystem*);
extern deInternalModule *depbRegisterInternalModule(deModuleSystem*);
extern deInternalModule *dermRegisterInternalModule(deModuleSystem*);
extern deInternalModule *desmRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deoggRegisterInternalModule(deModuleSystem*);
extern deInternalModule *desynRegisterInternalModule(deModuleSystem*);
extern deInternalModule *dethRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deapngRegisterInternalModule(deModuleSystem*);
extern deInternalModule *dewebmRegisterInternalModule(deModuleSystem*);
extern deInternalModule *fbxModelRegisterInternalModule(deModuleSystem*);
extern deInternalModule *fbxRigRegisterInternalModule(deModuleSystem*);
extern deInternalModule *fbxAnimRegisterInternalModule(deModuleSystem*);
extern deInternalModule *fbxSkinRegisterInternalModule(deModuleSystem*);
extern deInternalModule *denvrRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deoxrRegisterInternalModule(deModuleSystem*);
extern deInternalModule *deModioRegisterInternalModule(deModuleSystem*);

static const deModuleSystem::FPRegisterInternalModule vInternalModuleFunctionsPriority[]{
	deadRegisterInternalModule,
	nullptr};

static const deModuleSystem::FPRegisterInternalModule vInternalModuleFunctions[]{
	dearRegisterInternalModule,
	deaiRegisterInternalModule,
	deanimRegisterInternalModule,
	deanRegisterInternalModule,
	deoalRegisterInternalModule,
	deCRRegisterInternalModule,
	decrbRegisterInternalModule,
	deFontRegisterInternalModule,
	degnRegisterInternalModule,
	deoglRegisterInternalModule,
	depngRegisterInternalModule,
	depng3DRegisterInternalModule,
	deTgaRegisterInternalModule,
	deJpegRegisterInternalModule,
	deIesRegisterInternalModule,
	deWebpRegisterInternalModule,
	deWebp3DRegisterInternalModule,
	dewiRegisterInternalModule,
	delpRegisterInternalModule,
	demdlRegisterInternalModule,
	denbRegisterInternalModule,
	deoccmRegisterInternalModule,
	depbRegisterInternalModule,
	dermRegisterInternalModule,
	desmRegisterInternalModule,
	deoggRegisterInternalModule,
	desynRegisterInternalModule,
	dethRegisterInternalModule,
	deapngRegisterInternalModule,
	dewebmRegisterInternalModule,
	fbxModelRegisterInternalModule,
	fbxRigRegisterInternalModule,
	fbxAnimRegisterInternalModule,
	fbxSkinRegisterInternalModule,
	denvrRegisterInternalModule,
	deoxrRegisterInternalModule,
	deModioRegisterInternalModule,
	nullptr};

extern "C" {
MOD_ENTRY_POINT_ATTR const deModuleSystem::FPRegisterInternalModule* DEInternalModuleFunctionsPriority(){
	return (deModuleSystem::FPRegisterInternalModule*)&vInternalModuleFunctionsPriority;
}
MOD_ENTRY_POINT_ATTR const deModuleSystem::FPRegisterInternalModule* DEInternalModuleFunctions(){
	return (deModuleSystem::FPRegisterInternalModule*)&vInternalModuleFunctions;
}
}
