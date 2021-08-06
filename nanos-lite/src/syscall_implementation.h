#define concat(x, y) x ## y
#define call_(name) concat(call_, name) (c, a[1], a[2], a[3])
#define CALL_(name) static inline void concat(call_, name) (Context *c, int64_t arg0, int64_t arg1, int64_t arg2)
#define putnstr(buf, n) \
    for(size_t i=0; i<n; ++i) putch(*(buf+i))

#define _a7  c->GPR1
#define _a0  c->GPR2
#define _a1  c->GPR3
#define _a2  c->GPR4
#define _ret c->GPRx

CALL_(exit) {
    halt(arg0);
}

CALL_(yield) {
    yield();
}

CALL_(brk) {
    _ret = 0;
}

CALL_(open) {
    const char* pathname = (char*) arg0;
    int         flags    = (int)   arg1;
    int         mode     = (int)   arg2;
    file_table
    _ret = fd;
}

CALL_(read) {
    int    fd  = (int)    arg0;
    void*  buf = (void*)  arg1;
    size_t len = (size_t) arg2;
}

CALL_(close) {
    _ret = 0;
}

CALL_(lseek) {
    int    fd     = (int)    arg0;
    size_t offset = (size_t) arg1;
    int    whence = (int)    arg2;
}

CALL_(write) {
    int64_t     fd    = (int64_t) arg0; 
    const char* buf   = (char*)   arg1;
    size_t      count = (size_t)  arg2;

    if(fd==1 || fd==2) {
        putnstr(buf, count);
        _ret = count;
    }
    else {
        _ret = -1;
    }
}