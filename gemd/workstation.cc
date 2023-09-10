#include "workstation.h"

Workstation::Workstation(QLocalSocket *client, Screen *screen, QObject *parent)
			:QObject{parent}
			,_client(client)
			,_screen(screen)
	{

	}
