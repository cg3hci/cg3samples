#include "ch3dsamples.h"

void CH3DSamples::execute() {
    cg3::Dcel mesh(CG3_STRINGIFY(CG3_SAMPLES_HOME) "/data/airplane.obj");

    cg3::convexHull(mesh).saveOnObjFile(CG3_STRINGIFY(CG3_SAMPLES_HOME) "/data/convex_hull_airplane.obj");
}
