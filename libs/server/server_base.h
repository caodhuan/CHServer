#pragma once

#include <set>

#include "singleton.h"

namespace CHServer {
	class EventDispatcher;
	class Session;
	class SocketTCP;

	// 一个服务器进程抽象
	class ServerBase : public SingletonInheritable<ServerBase> {
	public:
		ServerBase();
		virtual ~ServerBase();
	
	public:
		virtual bool BeforeInitilize() = 0;
		bool Initilize();
		virtual bool AfterInitilize() = 0;

		virtual void BeforeFinalize() = 0;
		void Finalize();
		virtual void AfterFinalize() = 0;

		void Run();

	public:
		void RemoveSession(Session* session);

	private:
		EventDispatcher* m_dispatcher;

		std::set<Session*> m_sessions;

		SocketTCP* m_Server;
	};
}
