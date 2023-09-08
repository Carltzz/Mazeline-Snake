#include "File.h"
#include "MazelineEngine.h"

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace Mazeline;

bool Mazeline::FileExists(const char* path) {
	/*
	const char *c_str_path = path.c_str();
	DWORD attributes = GetFileAttributes(c_str_path);
	return (attributes != INVALID_FILE_ATTRIBUTES &&
		!(attributes & FILE_ATTRIBUTE_DIRECTORY));
	*/
#ifdef _WIN32
	DWORD attributes = GetFileAttributes(path);
	return (attributes != INVALID_FILE_ATTRIBUTES) &&
		!(attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
	MazelineFatalError("Platform not supported.");
	return false;
#endif
}

File::File(const char* path) {
	m_isOpen = false;
	if ((m_pathLength = (strlen(path) + 1)) > M_MAX_PATH_LEN)
		return;
	strncpy_s(m_path, path, M_MAX_FILE_LEN);
}

bool File::GetDirectory(char* dst, int length) {
	int index = M_MAX_PATH_LEN - 1;
	for (; index >= 0; --index) {
		if (m_path[index] == '/'
#ifdef _WIN32 
			|| m_path[index] == '\\'
#endif
		) {
			strncpy_s(dst, length, m_path, index);
			return true;
		}
	}

	if (length > 0) {
		dst[0] = '\0';
	}

	return false;
}

bool File::GetPath(char* dst, int length) {
	strncpy_s(dst, length, m_path, length);
	return true;
}

bool File::GetName(char* dst, int length) {
	int nameStart = M_MAX_PATH_LEN - 1;
	int nameEnd = nameStart;

	// First locate the last file path separator
	for (; nameStart >= 0; --nameStart) {
		if (m_path[nameStart] == '/'
#ifdef _WIN32
			|| m_path[nameStart] == '\\'
#endif
			) {
			break;
		}
	}

	// Second attempt to locate the final period after the final file separator
	for (; nameEnd >= nameStart; --nameEnd) {
		if (m_path[nameEnd] == '.')
			break;
	}

	// If a period cannot be found, the file has no extension
	if (nameStart == nameEnd) {
		for (; nameEnd < M_MAX_FILE_LEN; nameEnd++)
			if (m_path[nameEnd == '\0'])
				break;
	}

	int len = nameEnd - nameStart - 1;

	if (len > 0) {
		len = length < len ? length : len;
		strncpy_s(dst, length, m_path + nameStart + 1, len);
		return true;
	}
	return false;
}

bool File::GetExtension(char* dst, int length) {
	int extStart = M_MAX_FILE_LEN - 1;
	int extEnd = -1;

	for (; extStart >= 0; --extStart) {
		if (m_path[extStart] == '.') {
			for (extEnd = extStart; extEnd < M_MAX_FILE_LEN; ++extEnd) {
				if (m_path[extEnd] == '\0') {
					int len = extEnd - extStart;
					strncpy_s(dst, length, m_path + extStart, len);
					return true;
				}
			}
		}
	}

	if (length > 0) {
		dst[0] = '\0';
	}

	return false;
}

FILE* File::GetStream() {
	if (!m_isOpen)
		return 0;
	return m_pfile;
}

FileOpenError File::Open(FileOpenMode mode) {
	char* readMode;
	switch (mode) {
	case 1: 
		readMode = "r";
		if (!FileExists(const_cast<char*>(m_path)))
			return FOR_FILE_NOT_FOUND;
		break;
	case 9: readMode = "r+";
		if (!FileExists(const_cast<char*>(m_path)))
			return FOR_FILE_NOT_FOUND; 
		break;
	case 17: readMode = "rb";
		if (!FileExists(const_cast<char*>(m_path)))
			return FOR_FILE_NOT_FOUND; 
		break;
	case 25: readMode = "rb+";
		if (!FileExists(const_cast<char*>(m_path)))
			return FOR_FILE_NOT_FOUND; 
		break;
	case 2: readMode = "w"; break;
	case 4: readMode = "a"; break;
	case 18: readMode = "wb"; break;
	case 20: readMode = "ab"; break;
	case 10: readMode = "w+"; break;
	case 12: readMode = "a+"; break;
	case 26: readMode = "wb+"; break;
	case 28: readMode = "ab+"; break;
	}

	fopen_s(&m_pfile, m_path, const_cast<char*>(readMode));
	if (!m_pfile)
		return FOR_UNKNOWN;
	m_isOpen = true;
	return FOR_NO_ERROR;
}

long File::GetLength() {
	if (!m_isOpen || !m_pfile)
		return -1;
	long pos = ftell(m_pfile);
	long length = 0;
	fseek(m_pfile, 0, SEEK_END);
	length = ftell(m_pfile);
	fseek(m_pfile, pos, SEEK_SET);
	return length;
}

bool File::ReadAllText(char* dst, int length) {
	long len = GetLength();
	if (len == -1)
		return false;
	fseek(m_pfile, 0, SEEK_SET);
	len = len > length ? length : len;
	fread(dst, 1, len, m_pfile);
	return true;
}

bool File::IsOpen() {
	return m_pfile && m_isOpen;
}

void File::Flush() {
	if (m_isOpen && m_pfile)
		fflush(m_pfile);
}

bool File::Exists() {
	return FileExists(m_path);
}

void File::Close() {
	if (m_isOpen && m_pfile) {
		fflush(m_pfile);
		fclose(m_pfile);
	}
	m_pfile = 0;
}

File::~File() {
	if (IsOpen())
		Close();
}