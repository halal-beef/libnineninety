/*
 * Copyright (c) 2005-2018 Rich Felker
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define ISSPACE(c) \
    ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r' || (c) == '\f' || (c) == '\v')
#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')
#define ISUPPER(c) ((c) >= 'A' && (c) <= 'Z')
#define ISLOWER(c) ((c) >= 'a' && (c) <= 'z')
#define ISALPHA(c) (ISUPPER(c) || ISLOWER(c))
#define ISALNUM(c) (ISALPHA(c) || ISDIGIT(c))

int atoi(const char *str);
int memcmp(const void* s1, const void* s2, size_t n);
void* memchr(const void* s, int c, size_t n);
void* memset(void* dst, int c, size_t n);
int strcmp(const char* s1, const char* s2);
char* strchr(const char* s, int c);
char *strrchr(const char *s, int c);
char *strtok(char *str, const char *delim);
size_t strlen(const char* str);
size_t strnlen(const char *s, size_t n);
int strncmp(const char* s1, const char* s2, size_t n);
char* strstr(const char* h, const char* n);
unsigned long strtoul(const char* nptr, char** endptr, register int base);
long strtol(const char* str, char** endptr, int base);
unsigned short strtou16(const char* str);
char *strcpy (char *s1, const char *s2);
char *strncpy (char *d, char *s, long n);

void *memcpy (void *__restrict, const void *__restrict, size_t);
void *memmove (void *, const void *, size_t);
