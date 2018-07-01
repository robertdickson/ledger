#ifndef NETWORK_TCP_CLIENT_HPP
#define NETWORK_TCP_CLIENT_HPP

#include "core/byte_array/const_byte_array.hpp"
#include "core/byte_array/referenced_byte_array.hpp"
#include "core/logger.hpp"
#include "network/message.hpp"
#include "network/tcp/client_implementation.hpp"
#include "network/details/thread_manager.hpp"
#include "core/serializers/byte_array_buffer.hpp"
#include "core/serializers/referenced_byte_array.hpp"

#include "core/mutex.hpp"

#include "network/fetch_asio.hpp"

#include <atomic>
#include <memory>
#include <mutex>

namespace fetch
{
namespace network
{

class TCPClient
{
 public:
  typedef ThreadManager                             thread_manager_type;
  typedef typename ThreadManager::event_handle_type event_handle_type;
  typedef uint64_t                                  handle_type;
  typedef TCPClientImplementation                   implementation_type;
  typedef std::shared_ptr<implementation_type>      pointer_type;

  explicit TCPClient(thread_manager_type &thread_manager)
    : pointer_{std::make_shared< implementation_type >(thread_manager)}
  {
    // Note we register handles here, but do not connect until the base class constructed
    RegisterHandlers();
  }

  // Disable copy and move to avoid races when creating a closure
  // as inherited classes still won't be constructed at this point
  TCPClient(TCPClient const &rhs)            = delete;
  TCPClient(TCPClient &&rhs)                 = delete;
  TCPClient &operator=(TCPClient const &rhs) = delete;
  TCPClient &operator=(TCPClient&& rhs)      = delete;

  virtual ~TCPClient() noexcept {
    LOG_STACK_TRACE_POINT;

    Cleanup();
    pointer_->Close();
    pointer_.reset();
  }

  void Connect(byte_array::ConstByteArray const& host, uint16_t port)
  {
    pointer_->Connect(host, port);
  }

  void Connect(byte_array::ConstByteArray const& host, byte_array::ConstByteArray const& port)
  {
    pointer_->Connect(host, port);
  }

  // For safety, this MUST be called by the base class in its destructor
  // As closures to that class exist in the client implementation
  void Cleanup() noexcept
  {
    pointer_->ClearClosures();
    //pointer_->Close(); // TODO: (`HUT`) : look at with Ed, this appears to induce segfault
  }

  bool Closed() const noexcept
  {
    return pointer_->Closed();
  }

  void Send(message_type const& msg) noexcept
  {
    pointer_->Send(msg);
  }

  virtual void PushMessage(message_type const& value) = 0;
  virtual void ConnectionFailed() = 0;

  handle_type handle() const noexcept { return pointer_->handle(); }

  std::string Address() const noexcept
  {
    return pointer_->Address();
  }

  bool is_alive() const noexcept { return pointer_->is_alive(); }

protected:

  pointer_type  pointer_;

  void RegisterHandlers() noexcept
  {
    pointer_->OnConnectionFailed(
      [this]() {
        this->ConnectionFailed();
      });

    pointer_->OnPushMessage(
      [this](message_type const &m) {
        this->PushMessage(m);
      });
  }
};

}
}

#endif
