#include "utils.h"

namespace utils {
	std::string format(const char* text, ...) {
		char buf[1024];
		va_list va;

		va_start(va, text);
		_vsnprintf_s(buf, 1024, text, va);
		va_end(va);

		return std::string(buf);
	}
}