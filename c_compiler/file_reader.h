//
//  file_reader.h
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#ifndef C_COMPILER_FILEREADER_H
#define C_COMPILER_FILEREADER_H

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KB_1                1024
#define KB_4                (4 * KB_1)
#define DEFAULT_BUF_LEN     (2 * KB_4)
    
#define ANNOTATION_SIMPLE       "//"
#define ANNOTATION_C_STYLE      "/*"
    
    typedef enum
    {
        ERR_OK = 0,
        ERR_READ_FILE_ERR = -1,
        ERR_NO_MEM = -2
    }Err;
    
    typedef struct 
    {
        int     line;
        char    *str;
        int     len;
    }LineInfo;
    
    
    typedef struct
    {
        FILE    *fd;
        char    *buf;
        size_t  bufLen;
        long    fileSize;
        
        char    *cur;
        char    *end;
        int     initLineNo;
    }FileBuf;
    
    
    FileBuf *createFileBuf(const char *fileName, size_t bufLen);
    // by default, it just reads UTF-8 format file
    bool    readFile(FileBuf *fileBuf, int *err);
    bool    readFileToBufOffset(FileBuf *fileBuf, size_t offset, int *err);
    
    // read a line to the buf
    LineInfo    *readLine(FileBuf *fileBuf, int *err);
    
    void    freeFileBuf(FileBuf *fileBuf);
    
    
    long    getFileSize(FILE *fd);
    
    static  bool    readFileToBufOffsetInternal(FileBuf *fileBuf, size_t offset, int *err);
    
    char    *getNextNewlinePos(char *buf, int len);
    char    *copyStr(char *str, int len);
    
    bool    isAnnotation(LineInfo *lineInfo);
    bool    isAnnotationCStyleBegin(LineInfo *lineInfo);
    
    void    testFileReader();
    
#ifdef __cplusplus
}
#endif

#endif
