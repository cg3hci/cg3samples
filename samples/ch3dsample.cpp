#include "ch3dsample.h"
#include <cg3/utilities/timer.h>

void CH3DSamples::execute() {
    #ifdef _WIN32
    std::string p0(CG3_STRINGIFY(CG3_SAMPLES_HOME));
    p0.erase(p0.begin());
    p0.erase(p0.begin() + p0.size()-1);
    std::string p1("/data/airplane.obj");
    std::string fp = p0 + p1;
    std::replace(fp.begin(), fp.end(), '/', '\\');
    cg3::Dcel mesh(fp);
    #else
    cg3::Dcel mesh(CG3_STRINGIFY(CG3_SAMPLES_HOME) "/data/airplane.obj");
    #endif

    cg3::Timer t("Convex Hull");
    cg3::Dcel ch = cg3::convexHull(mesh);
    t.stopAndPrint();
    #ifdef _WIN32
    std::string p2("/data/convex_hull_airplane.obj");
    fp = p0 + p2;
    std::replace(fp.begin(), fp.end(), '/', '\\');
    ch.saveOnObjFile(fp);
    #else
    ch.saveOnObjFile(CG3_STRINGIFY(CG3_SAMPLES_HOME) "/data/convex_hull_airplane.obj");
    #endif
}
