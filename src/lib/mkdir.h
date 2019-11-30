#include <sys/types.h>
#include <sys/stat.h>

//  mkdir
//-----------------------------------------------------------------------------
//create a new directory with name path. The file permission bits of the new directory are initialised from mode. These file permission bits of the mode argument are modified by the process' file creation mask.
//When bits in mode other than the file permission bits are set, the meaning of these additional bits is implementation-dependent.

//The directory's user ID is set to the process' effective user ID. The directory's group ID is set to the group ID of the parent directory or to the effective group ID of the process.

//The newly created directory will be an empty directory.

//Upon successful completion, mkdir() will mark for update the st_atime, st_ctime and st_mtime fields of the directory. Also, the st_ctime and st_mtime fields of the directory that contains the new entry are marked for update.
//See more at <https://pubs.opengroup.org/onlinepubs/007908799/xsh/mkdir.html>
int nimps_mkdir(const char *path, mode_t mode);
