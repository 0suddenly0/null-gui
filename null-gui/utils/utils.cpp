#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include <assert.h>

namespace utils {
	std::string format(const char* text, ...) {
		char buf[1024];
		va_list va;

		va_start(va, text);
		_vsnprintf_s(buf, 1024, text, va);
		va_end(va);

		return std::string(buf);
	}

    FILE* file_open(const char* filename, const char* mode) {
        const int filename_wsize = ::MultiByteToWideChar(CP_UTF8, 0, filename, -1, NULL, 0);
        const int mode_wsize = ::MultiByteToWideChar(CP_UTF8, 0, mode, -1, NULL, 0);
        std::vector<unsigned short> buf;
        buf.resize(filename_wsize + mode_wsize);
        ::MultiByteToWideChar(CP_UTF8, 0, filename, -1, (wchar_t*)&buf[0], filename_wsize);
        ::MultiByteToWideChar(CP_UTF8, 0, mode, -1, (wchar_t*)&buf[filename_wsize], mode_wsize);
        return ::_wfopen((const wchar_t*)&buf[0], (const wchar_t*)&buf[filename_wsize]);
    }

    void* file_load_to_memory(const char* filename, const char* mode, size_t* out_file_size, int padding_bytes) {
        assert(filename && mode);
        if (out_file_size)
            *out_file_size = 0;

        FILE* f;
        if ((f = file_open(filename, mode)) == NULL)
            return NULL;

        size_t file_size = (size_t)file_get_size(f);
        if (file_size == (size_t)-1) {
            file_close(f);
            return NULL;
        }

        void* file_data = malloc(file_size + padding_bytes);
        if (file_data == NULL) {
            file_close(f);
            return NULL;
        }

        if (/*file_read(file_data, 1, file_size, f)*/fread(file_data, (size_t)1, (size_t)file_size, f) != file_size) {
            file_close(f);
            free(file_data);
            return NULL;
        }

        if (padding_bytes > 0)
            memset((void*)(((char*)file_data) + file_size), 0, (size_t)padding_bytes);

        file_close(f);
        if (out_file_size)
            *out_file_size = file_size;

        return file_data;
    }
}