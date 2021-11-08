#include <QApplication>

#include "Tags.h"
#include "Module.h"
#include "MainWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto context = createContext();
    auto mainWindow = context.resolve<MainWindowTag>();

    mainWindow->show();

    return app.exec();
}
