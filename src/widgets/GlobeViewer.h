//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_GLOBEVIEWER_H
#define GLOBAL_MARITIME_GLOBEVIEWER_H

#include <QOpenGLWidget>

class GlobeViewer : public QOpenGLWidget {
public:
    GlobeViewer();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
};


#endif //GLOBAL_MARITIME_GLOBEVIEWER_H
