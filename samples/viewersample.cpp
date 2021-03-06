#include "viewersample.h"

int ViewerSample::viewer(int argc, char *argv[]) {
    ///Test viewer.pri:
    QApplication app(argc, argv);
    int id;

    cg3::viewer::MainWindow gui;  // finestra principale, contiene la canvas di QGLViewer

    CG3_SUPPRESS_WARNING(id);

    //Test eigenmeshmanager.pri
#ifdef CG3_EIGENMESH_DEFINED
    cg3::viewer::EigenMeshManager em(&gui);
    id = gui.addManager(&em, "EigenMesh");


    //Test booleansmanager.pri
#if defined(CG3_LIBIGL_DEFINED) && defined(CG3_CGAL_DEFINED)
    cg3::viewer::BooleansManager bm(&gui);
    id = gui.addManager(&bm, "Booleans");
#endif
#endif

    //Test dcelmanager.pri
#ifdef CG3_DCEL_DEFINED
    cg3::viewer::DcelManager dm(&gui);
    id = gui.addManager(&dm, "Dcel");
#endif

    gui.setCurrentManager(id); // il window manager sarà quello visualizzato di default
    gui.update();
    gui.show();

    return app.exec();

}
