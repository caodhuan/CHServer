#include "db_server.h"

using namespace CHServer;

int main() {

	if (DBServer::Instance()->Initilize()) {
		DBServer::Instance()->Run();
	}

	DBServer::Instance()->Finalize();

	return 0;
}