//
//  file_reader.c
//  c_compiler
//
//  Created by xichen on 2012-3-19.
//  Copyright (c) 2012 xichen. All rights reserved.
//

#include <stdio.h>
#include "file_reader.h"
#include <stdlib.h>
#include <string.h>
#include "common.h"

FileBuf *createFileBuf(const char *fileName, size_t bufLen)
{
    FILE *fd = fopen(fileName, "rb");
    long fileSize;
    FileBuf *fileBuf;
    char *buf;
    if(fd == NULL)
        return NULL;
    
    fseek(fd, 0, SEEK_SET);
    fileBuf = (FileBuf *)malloc(sizeof(FileBuf));
    if(fileBuf == NULL)
        goto shouldCloseFile;
    
    fileSize = getFileSize(fd);
    if(fileSize < 0)
        goto shouldCloseFile;
    
    if(bufLen == 0)
        bufLen = DEFAULT_BUF_LEN;
    
    buf = (char *)malloc(bufLen);
    if(!buf)
        goto shouldFreeBuf;
    fileBuf->buf = fileBuf->cur = buf;
    fileBuf->bufLen = bufLen;
    fileBuf->end = buf + bufLen;
    fileBuf->fileSize = fileSize;
    fileBuf->fd = fd;
    fileBuf->initLineNo = 0;
    return fileBuf;
    
shouldFreeBuf:
    free(buf);
shouldCloseFile:
    fclose(fd);
    return NULL;
}

bool    readFile(FileBuf *fileBuf, int *err)
{
    return readFileToBufOffsetInternal(fileBuf, 0, err);
}

bool    readFileToBufOffset(FileBuf *fileBuf, size_t offset, int *err)
{
    return readFileToBufOffsetInternal(fileBuf, offset, err);
}

// read a line to the buf
LineInfo    *readLine(FileBuf *fileBuf, int *err)
{
    char    *newlinePos;
    LineInfo *lineInfo;
    newlinePos = getNextNewlinePos(fileBuf->cur, fileBuf->end - fileBuf->cur);
    if(!newlinePos)
    {
        return NULL;
    }
    lineInfo = (LineInfo *)malloc(sizeof(LineInfo));
    if(!lineInfo)
    {
        *err = ERR_NO_MEM;
        return NULL;
    }
    lineInfo->str = fileBuf->cur;
    lineInfo->len = newlinePos - fileBuf->cur;
    lineInfo->line = fileBuf->initLineNo + 1;
    ++(fileBuf->initLineNo);
    fileBuf->cur = newlinePos + 1;
    *err = ERR_OK;
    
    return lineInfo;
}


void    freeFileBuf(FileBuf *fileBuf)
{
    if(fileBuf)
    {
        free(fileBuf->buf);
        fclose(fileBuf->fd);
    }
}

long  getFileSize(FILE *fd)
{
    long ret;
    long cur = ftell(fd);
    
    fseek(fd, 0, SEEK_END);
    ret = ftell(fd);
    
    fseek(fd, cur, SEEK_SET);
    return ret;
}

static  bool    readFileToBufOffsetInternal(FileBuf *fileBuf, size_t offset, int *err)
{
    size_t ret = fread(fileBuf->buf + offset, 1, fileBuf->bufLen - offset, fileBuf->fd);
    if(ret < 1 && ferror(fileBuf->fd))
        goto readError;
    if(fileBuf->bufLen > ret)
    {
        fileBuf->buf[ret] = '\0';
        fileBuf->end = fileBuf->buf + ret;
    }
    else
    {
        fileBuf->buf[fileBuf->bufLen - 1] = '\0';
    }
    *err = ERR_OK;
    return true;
    
readError:
    *err = ERR_READ_FILE_ERR;
end:
    return false;
}

char    *getNextNewlinePos(char *buf, int len)
{
    int index = 0;
    while(*buf != '\n' && index < len)
    {
        ++index;
        ++buf;
    }
    if(index == len)
        return NULL;
    return buf;
}

char    *copyStr(char *str, int len)
{
    char *ret = (char *)malloc(len + 1);
    if(!ret)
        return NULL;
    
    strncpy(ret, str, len);
    ret[len] = '\0';
    return ret;
}

bool    isAnnotation(LineInfo *lineInfo)
{
    char *str = lineInfo->str;
    if(lineInfo->len < strlen(ANNOTATION_SIMPLE))
        return false;
    cc_skip_space_n((const char **)&str, lineInfo->len);
    if(lineInfo->len - (str - lineInfo->str) < strlen(ANNOTATION_SIMPLE))
        return false;
    return strncmp(lineInfo->str, ANNOTATION_SIMPLE, strlen(ANNOTATION_SIMPLE)) == 0;
}


bool    isAnnotationCStyleBegin(LineInfo *lineInfo)
{
    char *backStr = lineInfo->str;
    if(lineInfo->len < strlen(ANNOTATION_C_STYLE))
        return false;
    cc_skip_space_n((const char **)&lineInfo->str, lineInfo->len);
    if(lineInfo->len - (lineInfo->str - backStr) < strlen(ANNOTATION_C_STYLE))
        return false;
    return strncmp(lineInfo->str, ANNOTATION_C_STYLE, strlen(ANNOTATION_C_STYLE)) == 0;
}


void    testFileReader()
{
#if 0
    int err;
    FileBuf *fileBuf = createFileBuf("/Volumes/MACNEED/c_compiler/hello.c", 0);
    if(!fileBuf)
    {
        perror("createFileBuf error\n");
        return;
    }
    
    if(readFile(fileBuf, &err) && err == ERR_OK)
    {
        // printf("fileBuf's buf is \n%s\n", fileBuf->buf);
        LineInfo *line = readLine(fileBuf, &err);
        while(line)
        {
            if(err == ERR_OK)
            {
                char *temp = copyStr(line->str, line->len);
                if(temp)
                {
                    printf("%d %s %s\n", line->line, TO_BOOL_STR(isAnnotation(line)), temp);
                    free(temp);
                }
                free(line);
            }
            else
            {
                perror("readLine error\n");
                free(line);
                freeFileBuf(fileBuf);
                return;
            }
            line = readLine(fileBuf, &err);
        }
    }
    else
    {
        perror("readFile error\n");
        freeFileBuf(fileBuf);
        return;
    }
    
#endif
}


