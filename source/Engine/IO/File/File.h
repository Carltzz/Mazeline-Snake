#ifndef M_FILE_H
#define M_FILE_H

#include <cstdio>

#ifndef M_FILE_OPEN_ERRORS
#define M_FILE_OPEN_ERRORS 1
#define FOR_NO_ERROR 0
#define FOR_FILE_NOT_FOUND 1
#define FOR_ACCESS_DENIED 2
#define FOR_UNKNOWN 0xff
#endif

#ifdef _WIN32
#include <Windows.h>
#define M_MAX_PATH_LEN MAX_PATH
#define M_MAX_FILE_LEN FILENAME_MAX
#else

#endif

typedef int FileOpenError;

namespace Mazeline {

	enum FileOpenMode {
		Read = 0x01,
		Write = 0x02,
		Append = 0x04,
		Update = 0x08,
		Binary = 0x10
	};

	bool FileExists(const char* path);

	// A wrapper class for file operations
	class File {
	public:
		// Initialises an instance of a file
		File(const char* path);

		// Gets the absolute directory of the file
		bool GetDirectory(char* dst, int length);

		// Gets the full path of the file
		bool GetPath(char* dst, int length);

		// Gets the name of the file
		bool GetName(char* dst, int length);

		// Gets the extension of the file
		bool GetExtension(char* dst, int length);

		// Gets the object controlling the stream
		FILE* GetStream();

		// Gets the length of the open file
		long GetLength();

		// Reads all the text in the final
		bool ReadAllText(char* dst, int length);

		// Returns whether the file is open
		bool IsOpen();

		// Ensures all buffered data is written to the output stream
		void Flush();

		// Attempts to open the file (default mode is a+ mode)
		FileOpenError Open(FileOpenMode mode = (FileOpenMode)12); // 12 = a+

		// Returns whether the file exists or not
		bool Exists();

		// Attempts to close the file
		void Close();

		~File();
	private:
		bool m_isOpen;
		char m_path[M_MAX_PATH_LEN];
		FILE* m_pfile;
		int m_pathLength;
	};
}

#endif