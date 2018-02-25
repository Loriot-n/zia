#include "socket/Stream.hpp"


namespace zia {

	Stream::Stream() : _socket(0) {}

	Stream::~Stream() { ::close(_socket); }

}
