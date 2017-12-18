#ifndef VIEWERTEST_H
#define VIEWERTEST_H

///Test cg3 viewer module
#ifdef CG3_VIEWER_DEFINED
#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/viewer/interfaces/pickable_object.h>
#include <cg3/viewer/mainwindow.h>
#endif

///Test cg3 dcel module
#ifdef CG3_DCEL_DEFINED
#include <cg3/meshes/dcel/dcel.h>
//dcel manager
#ifdef CG3_VIEWER_DEFINED
#include <cg3/viewer/managers/dcel_manager/dcel_manager.h>
#endif
#endif

//Test eigenmesh module
#ifdef CG3_EIGENMESH_DEFINED
#include <cg3/meshes/eigenmesh/eigenmesh.h>
///Test trimeshviewer.pri: uncomment to test trimesh module
#ifdef CG3_VIEWER_DEFINED
#include <cg3/viewer/managers/eigenmesh_manager/eigenmesh_manager.h>
#ifdef CG3_CGAL_DEFINED
#ifdef CG3_LIBIGL_DEFINED
#include <cg3/viewer/managers/booleans_manager/booleans_manager.h>
#endif
#endif
#endif
#endif

namespace ViewerTest {
int viewer(int argc, char *argv[]);
}

#endif // VIEWERTEST_H
