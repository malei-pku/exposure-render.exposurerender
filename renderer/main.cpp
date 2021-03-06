
#include "core\renderthread.h"
#include "network\compositorsocket.h"
#include "gui\rendererwindow.h"
#include "geometry\lds.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <tchar.h>

#include <QtGui>
#include <QTcpSocket>

int main(int argc, char **argv)
{
	qDebug() << "Starting up renderer";
	
	QApplication Application(argc, argv);

	Application.setApplicationName("Exposure Render Compositor");
	Application.setOrganizationName("Delft University of Technology, department of Computer Graphics and Visualization");
	Application.setStyle("cleanlooks");

	if (!QDir(QApplication::applicationDirPath() + "//resources").exists())
	{
		qDebug() << "Resource directory does not exist, creating it";
		
		QDir().mkdir("resources");
	}

	QSettings Settings("renderer.ini", QSettings::IniFormat);

	QRenderer Renderer(&Application);

	QCompositorSocket CompositorSocket(&Renderer, &Application);
	
	const int Wait		= Settings.value("network/wait", 2000).toInt();
	QString HostName	= Settings.value("network/host", "localhost").toString();
	const quint16 Port	= Settings.value("network/port", 6000).toInt();

	qDebug() << "Connecting to" << HostName << "on port" << Port;

	CompositorSocket.connectToHost(HostName, Port);
	
	if (!CompositorSocket.waitForConnected(Wait))
	{
		qDebug() << "Unable to connect to host";

		qDebug() << "Last resort: trying to connect to localhost";

		HostName = "localhost";

		CompositorSocket.connectToHost(HostName, Port);

		if (!CompositorSocket.waitForConnected(Wait))
			qDebug() << "Not connected to host";
	}

	if (CompositorSocket.isOpen())
		qDebug() << "Connected to" << HostName << "on port" << Port;

	const bool& GuiEnabled = Settings.value("gui/enabled", true).toBool();

	qDebug() << "Gui is" << (GuiEnabled ? "enabled" : "disabled");

	QRendererWindow RendererWindow(&Renderer);

	if (GuiEnabled)
	{
		RendererWindow.show();
		RendererWindow.resize(640, 480);
	}

	return Application.exec();
}
