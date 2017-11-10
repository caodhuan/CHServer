#include "socket_base.h"
#include "event_dispatcher.h"

#include<cstring>

namespace CHServer {


	SocketBase::SocketBase(EventDispatcher* dispatcher)
		: m_receiveStartIndex(0)
		, m_receiveIndex(0)
		, m_sendBufferHead(&m_sendBuffer[0])
		, m_sendBuffIndex(0)
		, m_sendingBufferHead(&m_sendBuffer[1])
		, m_dispatcher(dispatcher) {
		for (int i = 0; i < SocketBase::MAX; i++) {
			m_callback[i] = NULL;
		}
	}

	SocketBase::~SocketBase() {

	}

	void SocketBase::SetCallback(SocketCallback connected, SocketCallback received) {
		m_callback[CONNECTED] = connected;
		m_callback[RECEIVED] = received;
	}

	bool SocketBase::IsClose() {
		return GetHandle() == NULL;
	}

	void SocketBase::Close() {

	}

	void SocketBase::Send(char* data, int32_t len) {
		AppendSendData(data, len);

		if (m_writer.data) {
			// �Ѿ��ڷ����ˣ���һ���ٷ�
			return;
		}
		uv_buf_t bufs;
		static const int count = 1;
		std::swap(m_sendBufferHead, m_sendingBufferHead);

		bufs = uv_buf_init((char*)m_sendingBufferHead, m_sendBuffIndex);

		m_sendBuffIndex = 0;

		uv_write(&m_writer, (uv_stream_t*)GetHandle(), &bufs, count, SocketBase::OnSent);

	}

	int32_t SocketBase::GetBuffLength() {
		if (m_receiveIndex >= m_receiveStartIndex) {
			return m_receiveIndex - m_receiveStartIndex;
		}

		return (int32_t)m_receiveBuffer.size() - m_receiveStartIndex + m_receiveIndex;
	}

	int32_t SocketBase::ReadBuff(char*& data) {
		if (m_receiveIndex == m_receiveStartIndex) {
			return 0;
		}

		data = &m_receiveBuffer[m_receiveStartIndex];

		if (m_receiveStartIndex < m_receiveIndex) {
			return m_receiveIndex - m_receiveStartIndex;
		} else {
			return (int32_t)m_receiveBuffer.size() - m_receiveStartIndex;
		}
	}

	void SocketBase::RemoveBuff(int32_t len) {
		if (m_receiveStartIndex == m_receiveIndex) {
			return;
		}

		if (m_receiveStartIndex + len > m_receiveBuffer.size()) {
			fprintf(stderr, "������ֱ���Ƴ�����\n");
			return;
		}

		m_receiveStartIndex += len;
		if (m_receiveStartIndex == m_receiveBuffer.size()) {
			m_receiveStartIndex = 0;
		}
	}

	void SocketBase::AppendSendData(char* data, int32_t len) {
		std::vector<char> buffer = *m_sendBufferHead;
		if (buffer.size() - m_sendBuffIndex < len) {
			buffer.resize(len - (buffer.size() - m_sendBuffIndex)); //  CTODO:�����ڴ����
		}
		memcpy(&buffer[m_sendBuffIndex], data, len);
		m_sendBuffIndex += len;
	}

	void SocketBase::OnNewConnection(uv_stream_t* handle, int status) {
		SocketBase* socket = (SocketBase*)handle->data;
		socket->m_callback[RECEIVED]();
	}

	void SocketBase::Allocator(uv_handle_t* handle, size_t suggestedSize, uv_buf_t* buf) {
		SocketBase* socket = (SocketBase*)handle->data;
		uint32_t remainSize = (uint32_t)socket->m_receiveBuffer.size() - socket->m_receiveIndex;
		if (remainSize == 0) {
			if (socket->m_receiveStartIndex >= suggestedSize) {
				buf->base = &socket->m_receiveBuffer[0];
				return;
			} else {
				socket->m_receiveBuffer.resize(suggestedSize);
				buf->base = &socket->m_receiveBuffer[socket->m_receiveIndex];
				return;
			}

		}
		buf->base = &socket->m_receiveBuffer[socket->m_receiveIndex];
	}

	void SocketBase::OnReceived(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
		SocketBase* socket = (SocketBase*)handle->data;
		socket->m_receiveIndex += nread;
		if (socket->m_receiveIndex > (int32_t)socket->m_receiveBuffer.size())
		{
			socket->m_receiveIndex = (int32_t)socket->m_receiveBuffer.size();
		}
		socket->m_callback[RECEIVED]();
	}

	void SocketBase::OnSent(uv_write_t* handle, int status) {
		SocketBase* socket = (SocketBase*)handle->data;

	}

	void SocketBase::OnConnected(uv_connect_t* handle, int status) {
		SocketBase* socket = (SocketBase*)handle->data;
		socket->m_callback[CONNECTED]();

		uv_read_start(handle->handle, SocketBase::Allocator, SocketBase::OnReceived);
	}

	void SocketBase::OnClose(uv_handle_t* handle) {
		SocketBase* socket = (SocketBase*)handle->data;
		socket->Close();
	}
}