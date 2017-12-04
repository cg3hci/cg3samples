/**
    @author Stefano Nuvoli
*/

///Test cg3 core module
#ifdef CG3_CORE_DEFINED
#include <cg3/data_structures/arrays/arrays.h>
#include <cg3/geometry/bounding_box.h>
#include <cg3/utilities/comparators.h>
#include <cg3/utilities/timer.h>
#endif

///Test cg3 viewer module
#ifdef CG3_VIEWER_DEFINED
#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/viewer/interfaces/pickable_object.h>
#include <cg3/viewer/mainwindow.h>
#include <cg3/viewer/managers/window_manager/window_manager.h>
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


#include <cg3/cg3lib.h>

#ifdef BSTTEST
#include "tests/bsttest.h"
#endif

#ifdef RTTEST
#include "tests/rttest.h"
#endif

#ifdef AABBTEST
#include "tests/aabbtest.h"
#endif

#ifdef CHTEST
#include "tests/chtest.h"
#endif

#ifdef BSTSAMPLE
#include "samples/bstsample.h"
#endif

#ifdef RTSAMPLE
#include "samples/rtsample.h"
#endif

#ifdef AABBSAMPLE
#include "samples/aabbsample.h"
#endif

#ifdef CHSAMPLE
#include "samples/chsample.h"
#endif

#ifdef BIPARTITEGRAPHSAMPLE
#include "samples/bipartitegraphsample.h"
#endif

#ifdef CH3DSAMPLE
#include "samples/ch3dsamples.h"
#endif

#include <iostream>

int main(int argc, char *argv[]) {
    CG3_SUPPRESS_WARNING(argc);
    CG3_SUPPRESS_WARNING(argv);

#ifdef BSTTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " BST test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    BSTTests::testCorrectness();
    BSTTests::testRandom();
    BSTTests::testMixed();
    BSTTests::testProgressive();

    std::cout << std::endl << std::endl;
#endif

#ifdef RTTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " RT test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    RTTests::testCorrectness();
    RTTests::testRandom();
    RTTests::testMixed();
    RTTests::testProgressive();

    std::cout << std::endl << std::endl;
#endif


#ifdef AABBTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " AABBTree test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    AABBTest::testCorrectness();
    AABBTest::testRandom();
    AABBTest::testMixed();
    AABBTest::testProgressive();

    std::cout << std::endl << std::endl;
#endif

#ifdef CHTEST
    std::cout << std::endl << std::endl <<
                 "##########################################################################" <<
                 " Convex hull test " <<
                 "##########################################################################" <<
                 std::endl << std::endl;

    CHTests::testCorrectness();
    CHTests::testRandom();
    CHTests::testMixed();
    CHTests::testProgressive();

    std::cout << std::endl << std::endl;
#endif

#ifdef BSTSAMPLE
    std::cout << "------ Binary search tree sample ------" << std::endl << std::endl;
    BSTSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef RTSAMPLE
    std::cout << "------ Range tree sample ------" << std::endl << std::endl;
    RTSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef AABBSAMPLE
    std::cout << "------ AABBTree sample ------" << std::endl << std::endl;
    AABBSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef CHSAMPLE
    std::cout << "------ Convex hull sample ------" << std::endl << std::endl;
    CHSamples::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef BIPARTITEGRAPHSAMPLE
    std::cout << "------ Bipartite Graph sample ------" << std::endl << std::endl;
    BipartiteGraphSample::execute();
    std::cout <<  std::endl << std::endl;
#endif

#ifdef CH3DSAMPLE
    std::cout << "------ Convex Hull 3D sample ------" << std::endl << std::endl;
    CH3DSamples::execute();
    std::cout <<  std::endl << std::endl;
#endif

    ///Test viewer.pri:
#ifdef VIEWERTEST
#ifdef CG3_VIEWER_DEFINED
    QApplication app(argc, argv);

    MainWindow gui;  // finestra principale, contiene la canvas di QGLViewer


    WindowManager wm(&gui); // Creo un window manager e lo aggiungo alla mainwindow
    int id = gui.addManager(&wm, "Window");

    //Test eigenmeshmanager.pri
#ifdef CG3_EIGENMESH_DEFINED
    EigenMeshManager em(&gui);
    id = gui.addManager(&em, "EigenMesh");


    //Test booleansmanager.pri
#if defined(CG3_LIBIGL_DEFINED) && defined(CG3_CGAL_DEFINED)
    BooleansManager bm(&gui);
    id = gui.addManager(&bm, "Booleans");
#endif
#endif

    //Test dcelmanager.pri
#ifdef CG3_DCEL_DEFINED
    DcelManager dm(&gui);
    id = gui.addManager(&dm, "Dcel");
#endif

    gui.setCurrentIndexToolBox(id); // il window manager sarà quello visualizzato di default
    gui.updateGlCanvas();
    gui.show();

    return app.exec();
#else
    return 0;
#endif
#endif
}
