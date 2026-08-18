//
// Created by Dmitri on 2026-08-15.
//

#ifndef GLOBAL_MARITIME_GLOBEVIEWER_H
#define GLOBAL_MARITIME_GLOBEVIEWER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "../core/objects/Globe.h"
#include "../core/objects/Camera.h"


class GlobeViewer : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    GlobeViewer();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
private:
    Globe globe;
    Shader globeShader;
    Camera camera;
    float aspectRatio = 1;
};


#endif //GLOBAL_MARITIME_GLOBEVIEWER_H
