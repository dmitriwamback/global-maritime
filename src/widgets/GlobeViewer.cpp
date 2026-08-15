//
// Created by Dmitri on 2026-08-15.
//

#include "GlobeViewer.h"

GlobeViewer::GlobeViewer() {

}

inline void GlobeViewer::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

inline void GlobeViewer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void GlobeViewer::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
}
