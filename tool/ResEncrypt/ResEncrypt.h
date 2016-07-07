#ifndef __firework_ResEncrypt__
#define __firework_ResEncrypt__

// add by xyh, from network

class ResEncrypt
{
public:
    static ResEncrypt* getInstance();
    
public:
    unsigned char* decryptData(unsigned char* buf, unsigned long size, unsigned long *pSize);
    unsigned char* getFileData(const char* fileName, const char* mode, unsigned long *pSize);
    unsigned char* encryptData(unsigned char* buf, unsigned long size, unsigned long *pSize);
    void setXXTeaKeyAndSign(const char* xxteaKey, int xxteaKeyLen, const char* xxteaSign, int xxteaSignLen);
    void cleanupXXTeaKeyAndSign();
private:
    static ResEncrypt* pResEncrypt_;
    
    bool xxteaEnabled_;
    char* xxteaKey_;
    int xxteaKeyLen_;
    char* xxteaSign_;
    int xxteaSignLen_;
private:
    ResEncrypt();
    ResEncrypt(const ResEncrypt&);
    ResEncrypt& operator = (const ResEncrypt&);
};

#endif