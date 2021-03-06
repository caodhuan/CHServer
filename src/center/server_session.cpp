#include "server_session.h"
#include "commonprotobuf.h"
#include "log.h"
#include "socket_base.h"

namespace CHServer {

	ServerSession::ServerSession(SocketBase* socket)
		: Session(socket) {
		RegisterProcedure(REGISTER_SERVER, std::bind(&ServerSession::OnRegisterServer, this, std::placeholders::_1, std::placeholders::_2) );

		RegisterProcedure(UPDATE_SERVER_INFO, std::bind(&ServerSession::OnUpdateServerInfo, this, std::placeholders::_1, std::placeholders::_2) );

		RegisterProcedure(QUERY_SERVER_INFO, std::bind(&ServerSession::OnQueryServerInfo, this, std::placeholders::_1, std::placeholders::_2) );
	}

	ServerSession::~ServerSession() {

	}

	void ServerSession::OnSessionConnected() {

	}

	void ServerSession::OnSessionDisconnect() {
		CHWARNINGLOG("OnSessionDisconnect");
	}

	bool ServerSession::OnRegisterServer(const char* data, uint16_t len) {
		CHWARNINGLOG("OnRegisterServer");

		static CMD_REGISTER_SERVER req;
		if (!req.ParseFromArray(data, len)) {
			return false;
		}

		return true;
	}

	bool ServerSession::OnUpdateServerInfo(const char* data, uint16_t len) {
		CHWARNINGLOG("OnUpdateServerInfo");

		static CMD_UPDATE_SERVER_INFO req;
		if (!req.ParseFromArray(data, len)) {
			return false;
		}

		return true;
	}

	bool ServerSession::OnQueryServerInfo(const char* data, uint16_t len) {
		CHWARNINGLOG("OnQueryServerInfo");

		static CMD_QUERY_SERVER_INFO req;
		if (!req.ParseFromArray(data, len)) {
			return false;
		}

		return true;
	}

}