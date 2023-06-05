#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "./types.h"
#include "util.h"

typedef struct Instruction {
  void (*execute)(EmulationState *);
} Instruction;

extern Instruction GB_INSTRUCTIONS[256];
extern Instruction GB_INSTRUCTIONS_PREFIXED[256];

void ld_regd8_d8(EmulationState *emu, u8 *reg);
void ld_regd16_d16(EmulationState *emu, u16 *reg);
void ld_regd8_rega16(EmulationState *emu, u8 *target, u16 *from);
void ld_a16_d8(EmulationState *emu, u16 target, u8 from);
void ld_rega16_d8(EmulationState *emu, u16 *target);
void and_regd8(EmulationState *emu, u8 reg);
void and_d8(EmulationState *emu);
void or_regd8(EmulationState *emu, u8 reg);
void or_d8(EmulationState *emu);
void xor_regd8(EmulationState *emu, u8 reg);
void xor_d8(EmulationState *emu);
void inc_regd8(EmulationState *emu, u8 *reg);
void inc_rega16(EmulationState *emu, u16 from);
void dec_regd8(EmulationState *emu, u8 *reg);
void dec_rega16(EmulationState *emu, u16 from);
void add_regd8(EmulationState *emu, u8 from);
void add_d8(EmulationState *emu);
void add_regd16(EmulationState *emu, u16 *from);
void adc_regd8(EmulationState *emu, u8 from);
void sub_regd8(EmulationState *emu, u8 from);
void sub_d8(EmulationState *emu);
void rotate_left_regd8(EmulationState *emu, u8 *target);
void rotate_right_regd8(EmulationState *emu, u8 *target);
void pop(EmulationState *emu, u16 *reg);
void push(EmulationState *emu, u16 *reg);
void cpl(EmulationState *emu);
void ccf(EmulationState *emu);
void scf(EmulationState *emu);

void nop(EmulationState *emu);
void ei(EmulationState *emu);
void di(EmulationState *emu);

void cp(EmulationState *emu, u8 value);

void jp_a16(EmulationState *emu);
void jp_hl(EmulationState *emu);
void jp_z_a16(EmulationState *emu);
void jp_nz_a16(EmulationState *emu);
void jp_c_a16(EmulationState *emu);
void jp_nc_a16(EmulationState *emu);

void jr_d8(EmulationState *emu);
void jr_z_d8(EmulationState *emu);
void jr_nz_d8(EmulationState *emu);
void jr_c_d8(EmulationState *emu);
void jr_nc_d8(EmulationState *emu);

void call(EmulationState *emu, u16 target);
void call_a16(EmulationState *emu);
void call_z_a16(EmulationState *emu);
void call_nz_a16(EmulationState *emu);
void call_c_a16(EmulationState *emu);
void call_nc_a16(EmulationState *emu);

void rst_00(EmulationState *emu);
void rst_08(EmulationState *emu);
void rst_10(EmulationState *emu);
void rst_18(EmulationState *emu);
void rst_20(EmulationState *emu);
void rst_28(EmulationState *emu);
void rst_30(EmulationState *emu);
void rst_38(EmulationState *emu);

void pop_bc(EmulationState *emu);
void pop_de(EmulationState *emu);
void pop_hl(EmulationState *emu);
void pop_af(EmulationState *emu);

void push_bc(EmulationState *emu);
void push_de(EmulationState *emu);
void push_hl(EmulationState *emu);
void push_af(EmulationState *emu);

void ret(EmulationState *emu);
void reti(EmulationState *emu);
void ret_z(EmulationState *emu);
void ret_nz(EmulationState *emu);
void ret_c(EmulationState *emu);
void ret_nc(EmulationState *emu);

void ldh_a8_a(EmulationState *emu);
void ldh_a_a8(EmulationState *emu);

void ld_ca_a(EmulationState *emu);
void ld_a_ca(EmulationState *emu);

void inc_bc(EmulationState *emu);
void inc_de(EmulationState *emu);
void inc_hl(EmulationState *emu);
void inc_ahl(EmulationState *emu);
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
void dec_ahl(EmulationState *emu);
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

void ld_b_hl(EmulationState *emu);
void ld_d_hl(EmulationState *emu);
void ld_h_hl(EmulationState *emu);
void ld_c_hl(EmulationState *emu);
void ld_e_hl(EmulationState *emu);
void ld_l_hl(EmulationState *emu);
void ld_a_hl(EmulationState *emu);

void ld_bc_a(EmulationState *emu);
void ld_de_a(EmulationState *emu);
void ld_hli_a(EmulationState *emu);
void ld_hld_a(EmulationState *emu);

void ld_hl_b(EmulationState *emu);
void ld_hl_c(EmulationState *emu);
void ld_hl_d(EmulationState *emu);
void ld_hl_e(EmulationState *emu);
void ld_hl_h(EmulationState *emu);
void ld_hl_l(EmulationState *emu);
void ld_hl_a(EmulationState *emu);

void add_b(EmulationState *emu);
void add_c(EmulationState *emu);
void add_d(EmulationState *emu);
void add_e(EmulationState *emu);
void add_h(EmulationState *emu);
void add_l(EmulationState *emu);
void add_ahl(EmulationState *emu);
void add_a(EmulationState *emu);

void add_bc(EmulationState *emu);
void add_de(EmulationState *emu);
void add_hl(EmulationState *emu);
void add_sp(EmulationState *emu);

void adc_b(EmulationState *emu);
void adc_c(EmulationState *emu);
void adc_d(EmulationState *emu);
void adc_e(EmulationState *emu);
void adc_h(EmulationState *emu);
void adc_l(EmulationState *emu);
void adc_ahl(EmulationState *emu);
void adc_a(EmulationState *emu);

void sub_b(EmulationState *emu);
void sub_c(EmulationState *emu);
void sub_d(EmulationState *emu);
void sub_e(EmulationState *emu);
void sub_h(EmulationState *emu);
void sub_l(EmulationState *emu);
void sub_ahl(EmulationState *emu);
void sub_a(EmulationState *emu);

void and_b(EmulationState *emu);
void and_c(EmulationState *emu);
void and_d(EmulationState *emu);
void and_e(EmulationState *emu);
void and_h(EmulationState *emu);
void and_l(EmulationState *emu);
void and_ahl(EmulationState *emu);
void and_a(EmulationState *emu);

void or_b(EmulationState *emu);
void or_c(EmulationState *emu);
void or_d(EmulationState *emu);
void or_e(EmulationState *emu);
void or_h(EmulationState *emu);
void or_l(EmulationState *emu);
void or_ahl(EmulationState *emu);
void or_a(EmulationState *emu);

void xor_b(EmulationState *emu);
void xor_c(EmulationState *emu);
void xor_d(EmulationState *emu);
void xor_e(EmulationState *emu);
void xor_h(EmulationState *emu);
void xor_l(EmulationState *emu);
void xor_ahl(EmulationState *emu);
void xor_a(EmulationState *emu);

void rlca(EmulationState *emu);
void rrca(EmulationState *emu);

void cp_d8(EmulationState *emu);
void cp_b(EmulationState *emu);
void cp_c(EmulationState *emu);
void cp_d(EmulationState *emu);
void cp_e(EmulationState *emu);
void cp_h(EmulationState *emu);
void cp_l(EmulationState *emu);
void cp_ahl(EmulationState *emu);
void cp_a(EmulationState *emu);

void prefix(EmulationState *emu);

void sla_regd8(EmulationState *emu, u8 *reg);
void sla_rega16(EmulationState *emu, u16 addr);
void sra_regd8(EmulationState *emu, u8 *reg);
void sra_rega16(EmulationState *emu, u16 addr);
void swap_regd8(EmulationState *emu, u8 *reg);
void swap_rega16(EmulationState *emu, u16 addr);
void srl_regd8(EmulationState *emu, u8 *reg);
void srl_rega16(EmulationState *emu, u16 addr);
void bit_regd8(EmulationState *emu, u8 *reg, u8 bit);
void bit_rega16(EmulationState *emu, u16 addr, u8 bit);
void res_regd8(EmulationState *emu, u8 *reg, u8 bit);
void res_rega16(EmulationState *emu, u16 addr, u8 bit);
void set_regd8(EmulationState *emu, u8 *reg, u8 bit);
void set_rega16(EmulationState *emu, u16 addr, u8 bit);

void sla_b(EmulationState *emu);
void sla_c(EmulationState *emu);
void sla_d(EmulationState *emu);
void sla_e(EmulationState *emu);
void sla_h(EmulationState *emu);
void sla_l(EmulationState *emu);
void sla_hl(EmulationState *emu);
void sla_a(EmulationState *emu);

void sra_b(EmulationState *emu);
void sra_c(EmulationState *emu);
void sra_d(EmulationState *emu);
void sra_e(EmulationState *emu);
void sra_h(EmulationState *emu);
void sra_l(EmulationState *emu);
void sra_hl(EmulationState *emu);
void sra_a(EmulationState *emu);

void swap_b(EmulationState *emu);
void swap_c(EmulationState *emu);
void swap_d(EmulationState *emu);
void swap_e(EmulationState *emu);
void swap_h(EmulationState *emu);
void swap_l(EmulationState *emu);
void swap_hl(EmulationState *emu);
void swap_a(EmulationState *emu);

void srl_b(EmulationState *emu);
void srl_c(EmulationState *emu);
void srl_d(EmulationState *emu);
void srl_e(EmulationState *emu);
void srl_h(EmulationState *emu);
void srl_l(EmulationState *emu);
void srl_hl(EmulationState *emu);
void srl_a(EmulationState *emu);

void bit_b0(EmulationState *emu);
void bit_c0(EmulationState *emu);
void bit_d0(EmulationState *emu);
void bit_e0(EmulationState *emu);
void bit_h0(EmulationState *emu);
void bit_l0(EmulationState *emu);
void bit_hl0(EmulationState *emu);
void bit_a0(EmulationState *emu);

void bit_b1(EmulationState *emu);
void bit_c1(EmulationState *emu);
void bit_d1(EmulationState *emu);
void bit_e1(EmulationState *emu);
void bit_h1(EmulationState *emu);
void bit_l1(EmulationState *emu);
void bit_hl1(EmulationState *emu);
void bit_a1(EmulationState *emu);

void bit_b2(EmulationState *emu);
void bit_c2(EmulationState *emu);
void bit_d2(EmulationState *emu);
void bit_e2(EmulationState *emu);
void bit_h2(EmulationState *emu);
void bit_l2(EmulationState *emu);
void bit_hl2(EmulationState *emu);
void bit_a2(EmulationState *emu);

void bit_b3(EmulationState *emu);
void bit_c3(EmulationState *emu);
void bit_d3(EmulationState *emu);
void bit_e3(EmulationState *emu);
void bit_h3(EmulationState *emu);
void bit_l3(EmulationState *emu);
void bit_hl3(EmulationState *emu);
void bit_a3(EmulationState *emu);

void bit_b4(EmulationState *emu);
void bit_c4(EmulationState *emu);
void bit_d4(EmulationState *emu);
void bit_e4(EmulationState *emu);
void bit_h4(EmulationState *emu);
void bit_l4(EmulationState *emu);
void bit_hl4(EmulationState *emu);
void bit_a4(EmulationState *emu);

void bit_b5(EmulationState *emu);
void bit_c5(EmulationState *emu);
void bit_d5(EmulationState *emu);
void bit_e5(EmulationState *emu);
void bit_h5(EmulationState *emu);
void bit_l5(EmulationState *emu);
void bit_hl5(EmulationState *emu);
void bit_a5(EmulationState *emu);

void bit_b6(EmulationState *emu);
void bit_c6(EmulationState *emu);
void bit_d6(EmulationState *emu);
void bit_e6(EmulationState *emu);
void bit_h6(EmulationState *emu);
void bit_l6(EmulationState *emu);
void bit_hl6(EmulationState *emu);
void bit_a6(EmulationState *emu);

void bit_b7(EmulationState *emu);
void bit_c7(EmulationState *emu);
void bit_d7(EmulationState *emu);
void bit_e7(EmulationState *emu);
void bit_h7(EmulationState *emu);
void bit_l7(EmulationState *emu);
void bit_hl7(EmulationState *emu);
void bit_a7(EmulationState *emu);

void res_b0(EmulationState *emu);
void res_c0(EmulationState *emu);
void res_d0(EmulationState *emu);
void res_e0(EmulationState *emu);
void res_h0(EmulationState *emu);
void res_l0(EmulationState *emu);
void res_hl0(EmulationState *emu);
void res_a0(EmulationState *emu);

void res_b1(EmulationState *emu);
void res_c1(EmulationState *emu);
void res_d1(EmulationState *emu);
void res_e1(EmulationState *emu);
void res_h1(EmulationState *emu);
void res_l1(EmulationState *emu);
void res_hl1(EmulationState *emu);
void res_a1(EmulationState *emu);

void res_b2(EmulationState *emu);
void res_c2(EmulationState *emu);
void res_d2(EmulationState *emu);
void res_e2(EmulationState *emu);
void res_h2(EmulationState *emu);
void res_l2(EmulationState *emu);
void res_hl2(EmulationState *emu);
void res_a2(EmulationState *emu);

void res_b3(EmulationState *emu);
void res_c3(EmulationState *emu);
void res_d3(EmulationState *emu);
void res_e3(EmulationState *emu);
void res_h3(EmulationState *emu);
void res_l3(EmulationState *emu);
void res_hl3(EmulationState *emu);
void res_a3(EmulationState *emu);

void res_b4(EmulationState *emu);
void res_c4(EmulationState *emu);
void res_d4(EmulationState *emu);
void res_e4(EmulationState *emu);
void res_h4(EmulationState *emu);
void res_l4(EmulationState *emu);
void res_hl4(EmulationState *emu);
void res_a4(EmulationState *emu);

void res_b5(EmulationState *emu);
void res_c5(EmulationState *emu);
void res_d5(EmulationState *emu);
void res_e5(EmulationState *emu);
void res_h5(EmulationState *emu);
void res_l5(EmulationState *emu);
void res_hl5(EmulationState *emu);
void res_a5(EmulationState *emu);

void res_b6(EmulationState *emu);
void res_c6(EmulationState *emu);
void res_d6(EmulationState *emu);
void res_e6(EmulationState *emu);
void res_h6(EmulationState *emu);
void res_l6(EmulationState *emu);
void res_hl6(EmulationState *emu);
void res_a6(EmulationState *emu);

void res_b7(EmulationState *emu);
void res_c7(EmulationState *emu);
void res_d7(EmulationState *emu);
void res_e7(EmulationState *emu);
void res_h7(EmulationState *emu);
void res_l7(EmulationState *emu);
void res_hl7(EmulationState *emu);
void res_a7(EmulationState *emu);

void set_b0(EmulationState *emu);
void set_c0(EmulationState *emu);
void set_d0(EmulationState *emu);
void set_e0(EmulationState *emu);
void set_h0(EmulationState *emu);
void set_l0(EmulationState *emu);
void set_hl0(EmulationState *emu);
void set_a0(EmulationState *emu);

void set_b1(EmulationState *emu);
void set_c1(EmulationState *emu);
void set_d1(EmulationState *emu);
void set_e1(EmulationState *emu);
void set_h1(EmulationState *emu);
void set_l1(EmulationState *emu);
void set_hl1(EmulationState *emu);
void set_a1(EmulationState *emu);

void set_b2(EmulationState *emu);
void set_c2(EmulationState *emu);
void set_d2(EmulationState *emu);
void set_e2(EmulationState *emu);
void set_h2(EmulationState *emu);
void set_l2(EmulationState *emu);
void set_hl2(EmulationState *emu);
void set_a2(EmulationState *emu);

void set_b3(EmulationState *emu);
void set_c3(EmulationState *emu);
void set_d3(EmulationState *emu);
void set_e3(EmulationState *emu);
void set_h3(EmulationState *emu);
void set_l3(EmulationState *emu);
void set_hl3(EmulationState *emu);
void set_a3(EmulationState *emu);

void set_b4(EmulationState *emu);
void set_c4(EmulationState *emu);
void set_d4(EmulationState *emu);
void set_e4(EmulationState *emu);
void set_h4(EmulationState *emu);
void set_l4(EmulationState *emu);
void set_hl4(EmulationState *emu);
void set_a4(EmulationState *emu);

void set_b5(EmulationState *emu);
void set_c5(EmulationState *emu);
void set_d5(EmulationState *emu);
void set_e5(EmulationState *emu);
void set_h5(EmulationState *emu);
void set_l5(EmulationState *emu);
void set_hl5(EmulationState *emu);
void set_a5(EmulationState *emu);

void set_b6(EmulationState *emu);
void set_c6(EmulationState *emu);
void set_d6(EmulationState *emu);
void set_e6(EmulationState *emu);
void set_h6(EmulationState *emu);
void set_l6(EmulationState *emu);
void set_hl6(EmulationState *emu);
void set_a6(EmulationState *emu);

void set_b7(EmulationState *emu);
void set_c7(EmulationState *emu);
void set_d7(EmulationState *emu);
void set_e7(EmulationState *emu);
void set_h7(EmulationState *emu);
void set_l7(EmulationState *emu);
void set_hl7(EmulationState *emu);
void set_a7(EmulationState *emu);

#endif // INSTRUCTION_H
