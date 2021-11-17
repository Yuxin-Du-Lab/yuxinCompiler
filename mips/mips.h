//
// Created by yuxin on 2021/11/15.
//

#ifndef COMPILER_MIPS_H
#define COMPILER_MIPS_H

#define word2bytes 4
#define standardStrName "str_"

#define reg_zero    "$0"
#define reg_at      "$at"
#define reg_v0      "$v0"
#define reg_v1      "$v1"
#define reg_a0      "$a0"
#define reg_a1      "$a1"
#define reg_a2      "$a2"
#define reg_a3      "$a3"
#define reg_t0      "$t0"
#define reg_t1      "$t1"
#define reg_t2      "$t2"
#define reg_t3      "$t3"
#define reg_t4      "$t4"
#define reg_t5      "$t5"
#define reg_t6      "$t6"
#define reg_t7      "$t7"
#define reg_s0      "$s0"
#define reg_s1      "$s1"
#define reg_s2      "$s2"
#define reg_s3      "$s3"
#define reg_s4      "$s4"
#define reg_s5      "$s5"
#define reg_s6      "$s6"
#define reg_s7      "$s7"
#define reg_t8      "$t8"
#define reg_t9      "$t9"
#define reg_k0      "$k0"
#define reg_k1      "$k1"
#define reg_gp      "$gp"
#define reg_sp      "$sp"
#define reg_fp      "$fp"
#define reg_ra      "$ra"

void makeMips();
#endif //COMPILER_MIPS_H
