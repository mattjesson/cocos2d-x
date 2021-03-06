#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include "TG3.h"

namespace CocosDenshion {

/**
@class FileUtils
@brief Offer tools of file system
*/
class FileUtils
{
public:
    /**
    @brief Whether the file is existed or not
    @param pFilePath The absolute path of file.
    @return If existed return true,or return false
    */
    static bool isFileExisted(const char* pFilePath);

    /**
    @brief  Set the ResourcePath,we will find resource in this path
    @param pszResourcePath  The absolute resource path
    */
    static void setResourcePath(const char *pszResourcePath);

    /**
    @brief Set the absolute path of the .zip file which contains all resource files
    @param pszZipPath The absolute path of the .zip file
    */
    static void setResourceZipFile(const char* pszZipPath);

    /**
    @brief Get resource file data
    @param[in]  pszFileName The resource file name which contain the path
    @param[in]  pszMode The read mode of the file
    @param[out] pSize If get the file data succeed the it will be the data size,or it will be 0
    @return if success,the pointer of data will be returned,or NULL is returned
    @warning If you get the file data succeed,you must delete it after used.
    */
    static unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
};

} // end of namespace CocosDenshion

#endif
