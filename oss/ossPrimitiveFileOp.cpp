/*******************************************************************
 *
 *
 * *****************************************************************/
#include "core.hpp"
#include "ossPrimitiveFileOp.hpp"

ossPrimitiveFileOp::ossPrimitiveFileOp()
{
    _fileHandle = OSS_INVALID_HANDLE_FD_VALUE;
    _bIsStdout = false;
}

bool ossPrimitiveFileOp::isValid()
{
    return(OSS_INVALID_HANDLE_FD_VALUE != _fileHandle);
}

void ossPrimitiveFileOp::Close()
{
    if(isValid() && (! _bIsStdout))
    {
        oss_close(_fileHandle);
        _fileHandle = OSS_INVALID_HANDLE_FD_VALUE;
    }
}

int ossPrimitiveFileOp::Open(const char *pFilePath, unsigned int options)
{
    int rc = 0;
    int mode = O_RDWR;

    if(option & OSSPRIMITIVE_FILE_OP_READ_ONLY)
    {
        mode = O_RDONLY;
    }
    else if(options & OSS_PRIMITIVE_FILE_OP_WRITE_ONLY)
    {
        mode = O_WRONLY;
    }


}
