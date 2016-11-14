/* Interface */
#include <stdint.h>

/* Handle */
typedef struct cv_file *CV_FILE;

/* Callbacks for memory mapped files */
typedef struct cv_filesys *CV_FILESYS;

/* Get handle for file "path", using CV_FILESYS options. Out: file */
CV_FILE cvf_openhandle(const char *path_utf8, CV_FILESYS fsys);

/* Release handle information */
int cvf_closehandle(CV_FILE file);

/* If file does not exist, return 0 */
int cvf_file_location(CV_FILE file);

/* File size, -1 on error */
int64_t cvf_size(CV_FILE file);

/* Is directory */
int cvf_is_directory(CV_FILE file);

/* Can be written / is not read only */
int cvf_is_writable(CV_FILE file);

/* Can be deleted / is not read only */
int cvf_is_deletable(CV_FILE file);

/* File date, =0 on error */
int64_t cvf_last_modified(CV_FILE file);

/* Is zipped */
int cvf_is_gzipped(CV_FILE file);

/* Open file for reading, writing or appending. Use filename from cvf_info. */
int cvf_openfile(CV_FILE file, int access_mode);

/* Close file */
int cvf_closefile(CV_FILE file);

/* Write data to file. Return -1 on error. */
int32_t cvf_write(CV_FILE file, const void *block_ptr, int32_t block_size);

/* Read data from file. Return -1 on error. */
int32_t cvf_read(CV_FILE file, void *block_ptr, int32_t block_size);

/* Callbacks for file system */
struct cv_filesys {
	int (*cv_get_file_disp)(const char *path_utf8, void *user_arg);    
	void *user_arg;
};


/* Implementation */
struct cv_file {
	int64_t size;

	int disp; /* 0 = not exist, 1 = on disk */

#if defined(_WIN32)
	const wchar_t *fname;
#else
	const char *fname;
#endif
};


#if defined(_WIN32)
#else


#endif
