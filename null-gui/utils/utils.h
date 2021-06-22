#pragma once
#define NOMINMAX
#include <windows.h>
#include <string>
#include <functional>
#include <assert.h>

namespace utils {
	std::string format(const std::string text, ...);

	FILE* file_open(const char* filename, const char* mode);
	static bool file_close(FILE* file) { return fclose(file) == 0; }
	static unsigned __int64 file_get_size(FILE* file) { long off = 0, sz = 0; return ((off = ftell(file)) != -1 && !fseek(file, 0, SEEK_END) && (sz = ftell(file)) != -1 && !fseek(file, off, SEEK_SET)) ? (unsigned __int64)sz : (unsigned __int64)-1; }
	static unsigned __int64 file_read(void* data, unsigned __int64 size, unsigned __int64 count, FILE* file) { return fread(data, (size_t)size, (size_t)count, file); }
	void* file_load_to_memory(const char* filename, const char* mode, size_t* out_file_size = NULL, int padding_bytes = 0);
}