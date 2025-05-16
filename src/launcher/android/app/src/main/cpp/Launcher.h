#ifndef DELAUNCHER_LAUNCHER_H
#define DELAUNCHER_LAUNCHER_H

#include <delauncher/delLauncher.h>
#include "FDVFSContainer.h"

class Launcher : public delLauncher{
private:
    FDVFSContainer::Ref pFDContainer;

public:
    explicit Launcher(const sConfig &config);
    ~Launcher() override;

    [[nodiscard]] inline const FDVFSContainer::Ref &GetFDContainer() const{ return pFDContainer; }

    delGameList ReadDelgaGames(const decString &path);

    void Dispose(JNIEnv *env);
};

#endif //DELAUNCHER_LAUNCHER_H
