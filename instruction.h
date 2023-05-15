#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "./types.h"
#include "util.h"

typedef struct Instruction {
  u8 encoding;
  void (*execute)(EmulationState *);
} Instruction;

#define GB_INSTRUCTIONS_LENGTH 152

extern Instruction GB_INSTRUCTIONS[GB_INSTRUCTIONS_LENGTH];

void ld_regd8_d8(EmulationState *emu, u8 *reg);
void ld_regd16_d16(EmulationState *emu, u16 *reg);
void ld_regd8_rega16(EmulationState *emu, u8 *target, u16 *from);
void ld_rega16_regd8(EmulationState *emu, u16 *target, u8 *from);
void ld_rega16_d8(EmulationState *emu, u16 *target);
void and_regd8(EmulationState *emu, u8 *reg);
void and_d8(EmulationState *emu);
void or_regd8(EmulationState *emu, u8 *reg);
void or_d8(EmulationState *emu);
void xor_regd8(EmulationState *emu, u8 *reg);
void xor_d8(EmulationState *emu);
void inc_regd8(EmulationState *emu, u8 *reg);
void dec_regd8(EmulationState *emu, u8 *reg);
void add_regd8(EmulationState *emu, u8 *from);
void sub_regd8(EmulationState *emu, u8 *from);

void nop(EmulationState *emu);
void di(EmulationState *emu);
void ei(EmulationState *emu);

void jp_a16(EmulationState *emu);

void jr_d8(EmulationState *emu);
void jr_z_d8(EmulationState *emu);
void jr_nz_d8(EmulationState *emu);
void jp_c_a16(EmulationState *emu);
void jr_nc_d8(EmulationState *emu);

void call_a16(EmulationState *emu);
void ret(EmulationState *emu);

void ldh_a8_a(EmulationState *emu);
void ldh_a_a8(EmulationState *emu);

void ld_ca_a(EmulationState *emu);
void ld_a_ca(EmulationState *emu);

void inc_bc(EmulationState *emu);
void inc_de(EmulationState *emu);
void inc_hl(EmulationState *emu);
void inc_sp(EmulationState *emu);

void inc_b(EmulationState *emu);
void inc_d(EmulationState *emu);
void inc_h(EmulationState *emu);

void inc_c(EmulationState *emu);
void inc_e(EmulationState *emu);
void inc_l(EmulationState *emu);
void inc_a(EmulationState *emu);

void dec_bc(EmulationState *emu);
void dec_de(EmulationState *emu);
void dec_hl(EmulationState *emu);
void dec_sp(EmulationState *emu);

void dec_b(EmulationState *emu);
void dec_d(EmulationState *emu);
void dec_h(EmulationState *emu);

void dec_c(EmulationState *emu);
void dec_e(EmulationState *emu);
void dec_l(EmulationState *emu);
void dec_a(EmulationState *emu);

void ld_b_b(EmulationState *emu);
void ld_d_b(EmulationState *emu);
void ld_h_b(EmulationState *emu);

void ld_b_c(EmulationState *emu);
void ld_d_c(EmulationState *emu);
void ld_h_c(EmulationState *emu);

void ld_b_d(EmulationState *emu);
void ld_d_d(EmulationState *emu);
void ld_h_d(EmulationState *emu);

void ld_b_e(EmulationState *emu);
void ld_d_e(EmulationState *emu);
void ld_h_e(EmulationState *emu);

void ld_b_h(EmulationState *emu);
void ld_d_h(EmulationState *emu);
void ld_h_h(EmulationState *emu);

void ld_b_l(EmulationState *emu);
void ld_d_l(EmulationState *emu);
void ld_h_l(EmulationState *emu);

void ld_b_a(EmulationState *emu);
void ld_d_a(EmulationState *emu);
void ld_h_a(EmulationState *emu);

void ld_c_b(EmulationState *emu);
void ld_e_b(EmulationState *emu);
void ld_l_b(EmulationState *emu);
void ld_a_b(EmulationState *emu);

void ld_c_c(EmulationState *emu);
void ld_e_c(EmulationState *emu);
void ld_l_c(EmulationState *emu);
void ld_a_c(EmulationState *emu);

void ld_c_d(EmulationState *emu);
void ld_e_d(EmulationState *emu);
void ld_l_d(EmulationState *emu);
void ld_a_d(EmulationState *emu);

void ld_c_e(EmulationState *emu);
void ld_e_e(EmulationState *emu);
void ld_l_e(EmulationState *emu);
void ld_a_e(EmulationState *emu);

void ld_c_h(EmulationState *emu);
void ld_e_h(EmulationState *emu);
void ld_l_h(EmulationState *emu);
void ld_a_h(EmulationState *emu);

void ld_c_l(EmulationState *emu);
void ld_e_l(EmulationState *emu);
void ld_l_l(EmulationState *emu);
void ld_a_l(EmulationState *emu);

void ld_c_a(EmulationState *emu);
void ld_e_a(EmulationState *emu);
void ld_l_a(EmulationState *emu);
void ld_a_a(EmulationState *emu);

void ld_a16_a(EmulationState *emu);

void ld_a_a16(EmulationState *emu);

void ld_b_d8(EmulationState *emu);
void ld_d_d8(EmulationState *emu);
void ld_h_d8(EmulationState *emu);
void ld_hl_d8(EmulationState *emu);

void ld_c_d8(EmulationState *emu);
void ld_e_d8(EmulationState *emu);
void ld_l_d8(EmulationState *emu);
void ld_a_d8(EmulationState *emu);

void ld_bc_d16(EmulationState *emu);
void ld_de_d16(EmulationState *emu);
void ld_hl_d16(EmulationState *emu);
void ld_sp_d16(EmulationState *emu);

void ld_a_bc(EmulationState *emu);
void ld_a_de(EmulationState *emu);
void ld_a_hli(EmulationState *emu);
void ld_a_hld(EmulationState *emu);

void ld_bc_a(EmulationState *emu);
void ld_de_a(EmulationState *emu);
void ld_hli_a(EmulationState *emu);
void ld_hld_a(EmulationState *emu);

void add_b(EmulationState *emu);
void add_c(EmulationState *emu);
void add_d(EmulationState *emu);
void add_e(EmulationState *emu);
void add_h(EmulationState *emu);
void add_l(EmulationState *emu);
void add_a(EmulationState *emu);
;
void sub_b(EmulationState *emu);
void sub_c(EmulationState *emu);
void sub_d(EmulationState *emu);
void sub_e(EmulationState *emu);
void sub_h(EmulationState *emu);
void sub_l(EmulationState *emu);
void sub_a(EmulationState *emu);
;
void and_b(EmulationState *emu);
void and_c(EmulationState *emu);
void and_d(EmulationState *emu);
void and_e(EmulationState *emu);
void and_h(EmulationState *emu);
void and_l(EmulationState *emu);
void and_a(EmulationState *emu);

void or_b(EmulationState *emu);
void or_c(EmulationState *emu);
void or_d(EmulationState *emu);
void or_e(EmulationState *emu);
void or_h(EmulationState *emu);
void or_l(EmulationState *emu);
void or_a(EmulationState *emu);

void xor_b(EmulationState *emu);
void xor_c(EmulationState *emu);
void xor_d(EmulationState *emu);
void xor_e(EmulationState *emu);
void xor_h(EmulationState *emu);
void xor_l(EmulationState *emu);
void xor_a(EmulationState *emu);

void cp_d8(EmulationState *emu);

#endif // INSTRUCTION_H
