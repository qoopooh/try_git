#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "myclass.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());
    MyClass myClass;

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/QuickApp/main.qml"));
    viewer->showExpanded();

    QObject::connect(&(*viewer), SIGNAL(qmlSignal(QString)),
        &myClass, SLOT(cppSlot(QString)));

    return app->exec();
}

