#include "TelldusMain.h"
#include "ConnectionListener.h"
#include "EventHandler.h"
#include "ClientCommunicationHandler.h"

#include <stdio.h>
#include <list>

class TelldusMain::PrivateData {
public:
	EventHandler eventHandler;
	Event *stopEvent;
};

TelldusMain::TelldusMain(void)
{
	d = new PrivateData;
	d->stopEvent = d->eventHandler.addEvent();
}

TelldusMain::~TelldusMain(void)
{
	delete d;
}

void TelldusMain::start(void) {
	Event *clientEvent = d->eventHandler.addEvent();
	
	ConnectionListener clientListener(L"TelldusClient");
	//TODO: eventlistener

	clientListener.listen(clientEvent);
	//TODO: listen on eventListener

	std::list<ClientCommunicationHandler *> clientCommunicationHandlerList;

	while(!d->stopEvent->isSignaled()) {
		if (!d->eventHandler.waitForAny()) {
			continue;
		}
		if (clientEvent->isSignaled()) {
			clientEvent->clearSignal();
			//New client connection
			
			
			TelldusCore::Socket *s = clientListener.retrieveClientSocket();
			if(s){
				Event *handlerEvent = d->eventHandler.addEvent();
				ClientCommunicationHandler *clientCommunication = new ClientCommunicationHandler(s, handlerEvent);
				clientCommunication->start();
				clientCommunicationHandlerList.push_back(clientCommunication);
			}
			
			clientListener.listen(clientEvent);
		}


		for ( std::list<ClientCommunicationHandler *>::iterator it = clientCommunicationHandlerList.begin(); it != clientCommunicationHandlerList.end(); ){
			if ((*it)->isDone()){
				delete *it;
				it = clientCommunicationHandlerList.erase(it);
			}
			else{
				++it;
			}
		}

#ifdef _WINDOWS
		Sleep(1000);
#else
		sleep(1);
#endif
	}
}

void TelldusMain::stop(void){
	d->eventHandler.signal(d->stopEvent);
}