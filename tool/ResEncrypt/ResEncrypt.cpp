#include "ResEncrypt.h"
#include "cocos2d.h"
#include "../xxtea/xxtea.h"

ResEncrypt* ResEncrypt::pResEncrypt_ = nullptr;
ResEncrypt* ResEncrypt::getInstance()
{
    if(!pResEncrypt_)
    {
        pResEncrypt_ = new ResEncrypt();
    }
    return pResEncrypt_;
}

ResEncrypt::ResEncrypt()
:xxteaEnabled_(false)
,xxteaKey_(nullptr)
,xxteaKeyLen_(0)
,xxteaSign_(nullptr)
,xxteaSignLen_(0)
{
}

void ResEncrypt::setXXTeaKeyAndSign(const char* xxteaKey, int xxteaKeyLen, const char* xxteaSign, int xxteaSignLen)
{
    cleanupXXTeaKeyAndSign();
    if( xxteaKey && xxteaKeyLen && xxteaSign && xxteaSignLen)
    {
        xxteaKey_ = (char*)malloc(xxteaKeyLen);
        memcpy(xxteaKey_, xxteaKey, xxteaKeyLen);
        xxteaKeyLen_ = xxteaKeyLen;
        xxteaSign_ = (char*)malloc(xxteaSignLen);
        memcpy(xxteaSign_, xxteaSign, xxteaSignLen);
        xxteaSignLen_ = xxteaSignLen;
        xxteaEnabled_ = true;
    } else
    {
        xxteaEnabled_ = false;
    }
}

void ResEncrypt::cleanupXXTeaKeyAndSign()
{
    if(xxteaKey_)
    {
        free(xxteaKey_);
        xxteaKey_ = nullptr;
        xxteaKeyLen_ = 0;
    }
    if(xxteaSign_)
    {
        free(xxteaSign_);
        xxteaSign_ = nullptr;
        xxteaSignLen_ = 0;
    }
}

unsigned char* ResEncrypt::getFileData(const char* fileName, const char* mode, unsigned long* pSize)
{
    ssize_t size;
    unsigned char* buf = cocos2d::FileUtils::getInstance()->getFileData(fileName, mode, &size);
    if(nullptr == buf)
    {
        return nullptr;
    }
    unsigned char* buffer = nullptr;
    ResEncrypt* pResEncrypt = ResEncrypt::getInstance();
    bool isXXTEA = pResEncrypt && pResEncrypt->xxteaEnabled_;
    for(unsigned int i = 0; isXXTEA && i < pResEncrypt->xxteaSignLen_ && i < size; ++ i )
    {
        isXXTEA = buf[i] == pResEncrypt->xxteaSign_[i];
    }
    if(isXXTEA)
    {
        xxtea_long len = 0;
        buffer = xxtea_decrypt(	buf+pResEncrypt->xxteaSignLen_,
                               (xxtea_long)size - (xxtea_long)pResEncrypt->xxteaSignLen_,
                               (unsigned char*)pResEncrypt->xxteaKey_,
                               (xxtea_long)pResEncrypt->xxteaKeyLen_, &len);
        delete [] buf;
        buf = nullptr;
        size = len;
    } else
    {
        buffer = buf;
    }
    if(pSize)
    {
        *pSize = size;
    }
    return buffer;
}

unsigned char *ResEncrypt::decryptData(unsigned char* buf, unsigned long size, unsigned long* pSize)
{
    CCAssert(buf != nullptr, "decryptData buf cannot nullptr");
    unsigned char* buffer = nullptr;
    ResEncrypt* pResEncrypt = ResEncrypt::getInstance();
    bool isXXTEA = pResEncrypt && pResEncrypt->xxteaEnabled_;
    for(unsigned int i = 0; isXXTEA && i < pResEncrypt->xxteaSignLen_ && i < size; ++ i )
    {
        isXXTEA = buf[i] == pResEncrypt->xxteaSign_[i];
    }
    if(isXXTEA)
    {
        xxtea_long len = 0;
        buffer = xxtea_decrypt(	buf+pResEncrypt->xxteaSignLen_,
                               (xxtea_long)size - (xxtea_long)pResEncrypt->xxteaSignLen_,
                               (unsigned char*)pResEncrypt->xxteaKey_,
                               (xxtea_long)pResEncrypt->xxteaKeyLen_, &len);
        free(buf);
        buf = nullptr;
        size = len;
    } else
    {
        buffer = buf;
    }
    if(pSize)
    {
        *pSize = size;
    }
    return buffer;
}

unsigned char* ResEncrypt::encryptData(unsigned char* buf, unsigned long size, unsigned long* pSize)
{
    CCAssert(buf != nullptr, "encryptData buf cannot nullptr");
    unsigned char* buffer = nullptr;
    unsigned char* ret = nullptr;
    ResEncrypt* pResEncrypt = ResEncrypt::getInstance();
    bool isXXTEA = pResEncrypt && pResEncrypt->xxteaEnabled_;
    for(unsigned int i = 0; isXXTEA && i < pResEncrypt->xxteaSignLen_ && i < size; ++ i )
    {
        isXXTEA = buf[i] == pResEncrypt->xxteaSign_[i];
    }
    if(!isXXTEA)
    {
        xxtea_long len = 0;
        buffer = xxtea_encrypt(	buf,
                               (xxtea_long)size,
                               (unsigned char*)pResEncrypt->xxteaKey_,
                               (xxtea_long)pResEncrypt->xxteaKeyLen_, &len);
        free(buf);
        buf = nullptr;
        size = len;
        ret = (unsigned char*)malloc(size+pResEncrypt->xxteaSignLen_+1);
        memcpy(ret, pResEncrypt->xxteaSign_, pResEncrypt->xxteaSignLen_);
        memcpy(ret+pResEncrypt->xxteaSignLen_, buffer, size);
        ret[len+pResEncrypt->xxteaSignLen_] = '\0';
    } else
    {
        ret = buf;
    }
    if(pSize)
    {
        *pSize = size+pResEncrypt->xxteaSignLen_;
    }
    return ret;
}
