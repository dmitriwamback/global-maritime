#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QPushButton>

#include "src/widgets/GlobeViewer.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QMainWindow window;

    window.setWindowTitle("Hello World");
    window.resize(1200, 800);

    auto* globeViewerWidget = new GlobeViewer();
    auto* centralWidget = new QWidget();
    auto* layout = new QHBoxLayout(centralWidget);
    auto* testButton = new QPushButton("hello");

    globeViewerWidget->resize(100, 100);
    layout->addWidget(globeViewerWidget);
    layout->addWidget(testButton);

    window.setCentralWidget(centralWidget);

    window.show();

    return QApplication::exec();
}