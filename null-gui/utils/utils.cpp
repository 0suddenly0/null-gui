#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>

#include "utils.h"

namespace utils {
    std::string format(const std::string text, ...) {
        int final_n, n = ((int)text.size()) * 2;
        std::unique_ptr<char[]> formatted;
        va_list ap;
        while(true) {
            formatted.reset(new char[n]);
            strcpy(&formatted[0], text.c_str());
            va_start(ap, text);
            final_n = vsnprintf(&formatted[0], n, text.c_str(), ap);
            va_end(ap);
            if (final_n < 0 || final_n >= n) n += abs(final_n - n + 1);
            else break;
        }
        return std::string(formatted.get());
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