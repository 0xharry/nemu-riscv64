#ifndef __EXPR_H__
#define __EXPR_H__

#include <common.h>

enum expr_state{
    VALID_RET=1,
    DIV_BY_0,
    MAKE_FAIL,
    PARE_FAIL,
    BAD_EXPR,
    SSCANF_FAIL,
    REG_FAIL,
    FIND_OP_FAIL,
};

word_t expr(char *, int *);

#endif
