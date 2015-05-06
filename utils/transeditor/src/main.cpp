#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <sstream>

#include "Configuration.h"
#include "controller/Manager.h"
#include "view/MainWindow.h"
#include "util/Logger.h"

#ifdef _WIN_32
#define FILE_SEPARATOR	("\\")
#else
#define FILE_SEPARATOR	("/")
#endif

//==============================================================================
//
// Initialize logger
//
//==============================================================================


#ifndef _WIN_32
static void initializeLogger() {
	std::stringstream configFilepath;
	configFilepath << getInstallDirPath() << FILE_SEPARATOR << LOG4CXX_CONFIG_PATH;
	initializeLoggerConfig( configFilepath.str() );
}

class LoggerInitializer {
public:
	LoggerInitializer() {
		initializeLogger();
	}
};

LoggerInitializer loggerInitializer;
#endif

int main(int argc, char **argv) {
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
    app.setStyle("Plastique");

    MainWindow mainWindow(0);
    mainWindow.move(QApplication::desktop()->geometry().center() - mainWindow.geometry().center());

    Manager manager(&mainWindow);

    mainWindow.show();
    return app.exec();
}

