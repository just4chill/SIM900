

#include "stdio.h"
#include "stdlib.h"
#include "errno.h"

FILE *stdin;
FILE *stdout;
FILE *stderr;

int _sbrk(int incr) {
    return -1;
}

int _close(int file) {
    return -1;
}

int _execve(char *name, char **argv, char **env) {
    return -1;
}

void _exit(int __status) {
  while(1);
}

int _fork() {
    return -1;
}

int _fstat(int file, struct stat *st) {
    return 0;
}

int _getpid() {
    return 1;
}

int _kill(int pid, int sig) {
    return(-1);
}

int isatty(int fildes) {
    return 1;
}
int _isatty(int fildes) {
    return 1;
}

int _link(char *old, char *new) {
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _open(const char *name, int flags, int mode) {
    return -1;
}

int _read(int file, char *ptr, int len) {
    return 0;
}

int _stat(char *file, struct stat *st) {
    return 0;
}

int _times(struct tms *buf) {
    return -1;
}

int _unlink(char *name) {
    return -1;
}

int _wait(int *status) {
    return -1;
}

int _write(int file, char *ptr, int len) {
    return -1;
}

/*
 * _fopen
 * Open a file. Minimal implementation:
 */
FILE *_fopen(const char *name, const char *_type) 
{
    return NULL;
}

/*
 * _fclose
 * Close a file. Minimal implementation:
 */
int _fclose(FILE *fp)
{
    return 0;
}

/*
 * _malloc
 * Allocate memory. Minimal implementation:
 */
void *_malloc(size_t nbytes) 
{
    return NULL;
}

/*
 * _free
 * Release memory. Minimal implementation:
 */
void _free(void *aptr) 
{
}
