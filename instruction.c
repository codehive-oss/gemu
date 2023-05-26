#include "./instruction.h"
#include "types.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

// Loads 8-bit data to any 8-bit register
void ld_regd8_d8(EmulationState *emu, u8 *reg) {
  u8 target = emu->rom[*emu->pc];
  *reg      = target;
  *emu->pc += 1;
}

// Loads 16-bit data to any 16-bit register
void ld_regd16_d16(EmulationState *emu, u16 *reg) {
  u16 target = *(u16 *)&emu->rom[*emu->pc];
  *reg       = target;
  *emu->pc += 2;
}

// Loads 8-bit data from any the memory address pointed by any 16-bit register
void ld_regd8_rega16(EmulationState *emu, u8 *target, u16 *from) {
  *target = emu->mem[*from];
}

// Loads 8-bit register data to any the memory address pointed by any 16-bit
// register
void ld_rega16_regd8(EmulationState *emu, u16 *target, u8 *from) {
  emu->mem[*target] = *from;
}

// Loads 8-bit data to any the memory address pointed by any 16-bit register
void ld_rega16_d8(EmulationState *emu, u16 *target) {
  u8 from           = emu->rom[*emu->pc];
  emu->mem[*target] = from;
  *emu->pc += 1;
}

// AND with 8-bit register
void and_regd8(EmulationState *emu, u8 reg) {
  *emu->a &= reg;
  set_flags(emu, *emu->a == 0, 0, 1, 0);
}

// AND with 8-bit data
void and_d8(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  *emu->pc += 1;
  and_regd8(emu, target);
}

// OR with 8-bit register
void or_regd8(EmulationState *emu, u8 reg) {
  *emu->a |= reg;
  set_flags(emu, *emu->a == 0, 0, 0, 0);
}

// OR with 8-bit data
void or_d8(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  *emu->pc += 1;
  or_regd8(emu, target);
}

// XOR with 8-bit register
void xor_regd8(EmulationState *emu, u8 reg) {
  *emu->a ^= reg;
  set_flags(emu, *emu->a == 0, 0, 0, 0);
}

// XOR with 8-bit data
void xor_d8(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  *emu->pc += 1;
  xor_regd8(emu, target);
}

// Increment 1 from 8-bit register
void inc_regd8(EmulationState *emu, u8 *reg) {
  *reg += 1;
  set_flags(emu, *reg == 0, 0, (*reg & 0x0F) == 0x00, -1);
}

// Increment 1 from 8-bit register
void inc_rega16(EmulationState *emu, u16 from) {
  inc_regd8(emu, &emu->mem[from]);
}

// Decrement 1 from 8-bit register
void dec_regd8(EmulationState *emu, u8 *reg) {
  *reg -= 1;
  set_flags(emu, *reg == 0, 1, (*reg & 0x0F) == 0x0F, -1);
}

// Decrement 1 from 16-bit address data
void dec_rega16(EmulationState *emu, u16 from) {
  dec_regd8(emu, &emu->mem[from]);
}

// Add 8-bit data to A register
void add_regd8(EmulationState *emu, u8 from) {
  bool h = (*emu->a & 0x0F) + (from & 0x0F) > 0x0F;
  bool c = *emu->a + from > 0xFF;

  *emu->a += from;
  set_flags(emu, *emu->a == 0, 0, h, c);
}

void add_d8(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  *emu->pc += 1;
  add_regd8(emu, target);
}

// Add 16-bit data to HL register
void add_regd16(EmulationState *emu, u16 *from) {
  bool h = (*emu->hl & 0x00FF) + (*from & 0x00FF) > 0x00FF;
  bool c = *emu->hl + *from > 0xFFFF;

  *emu->hl += *from;
  set_flags(emu, -1, 0, h, c);
}

void sub_regd8(EmulationState *emu, u8 from) {
  bool h = (*emu->a & 0x0F) - (from & 0x0F) < 0;
  bool c = *emu->a - from < 0;

  *emu->a -= from;
  set_flags(emu, *emu->a == 0, 1, h, c);
}

void sub_d8(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  *emu->pc += 1;
  sub_regd8(emu, target);
}

void pop(EmulationState *emu, u16 *reg) {
  *reg = *(u16 *)&emu->mem[*emu->sp];
  *emu->sp += 2;
}

void push(EmulationState *emu, u16 *reg) {
  *emu->sp -= 2;
  *(u16 *)&emu->mem[*emu->sp] = *reg;
}

void cpl(EmulationState *emu) {
  *emu->a ^= 0xFF;
  set_flags(emu, -1, 1, 1, -1);
}

void nop(EmulationState *emu) {}

// https://gbdev.io/pandocs/Interrupts.html
// TODO: Delay instruction by one
void ei(EmulationState *emu) { emu->ime = true; }
void di(EmulationState *emu) { emu->ime = false; }

void jp_a16(EmulationState *emu) {
  u16 target = *(u16 *)&emu->rom[*emu->pc];
  *emu->pc   = target;
}

void jp_hl(EmulationState *emu) {
  *emu->pc = *emu->hl;
}

void jp_z_a16(EmulationState *emu) {
  if (*emu->f & Z_MASK) { // Z is set
    jp_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void jp_nz_a16(EmulationState *emu) {
  if (!(*emu->f & Z_MASK)) { // Z is not set
    jp_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void jp_c_a16(EmulationState *emu) {
  if (*emu->f & C_MASK) { // C is set
    jp_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void jp_nc_a16(EmulationState *emu) {
  if (!(*emu->f & C_MASK)) { // C is not set
    jp_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void jr_d8(EmulationState *emu) {
  i8 target = *(i8 *)&emu->rom[*emu->pc];
  *emu->pc += 1;
  *emu->pc += target;
}

void jr_z_d8(EmulationState *emu) {
  if (*emu->f & Z_MASK) { // Z is set
    jr_d8(emu);
  } else {
    *emu->pc += 1;
  }
}

void jr_nz_d8(EmulationState *emu) {
  if (!(*emu->f & Z_MASK)) { // Z is not set
    jr_d8(emu);
  } else {
    *emu->pc += 1;
  }
}

void jr_c_d8(EmulationState *emu) {
  if (*emu->f & C_MASK) { // C is set
    jr_d8(emu);
  } else {
    *emu->pc += 1;
  }
}

void jr_nc_d8(EmulationState *emu) {
  if (!(*emu->f & C_MASK)) { // C not is set
    jr_d8(emu);
  } else {
    *emu->pc += 1;
  }
}

void call(EmulationState *emu, u16 target) {
  push(emu, emu->pc);
  *emu->pc = target;
}

void call_a16(EmulationState *emu) {
  u16 target = *(u16 *)&emu->rom[*emu->pc];
  *emu->pc += 2;

  call(emu, target);
}

void call_z_a16(EmulationState *emu) {
  if (*emu->f & Z_MASK) { // Z is set
    call_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void call_nz_a16(EmulationState *emu) {
  if (!(*emu->f & Z_MASK)) { // C not is set
    call_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void call_c_a16(EmulationState *emu) {
  if (*emu->f & C_MASK) { // Z is set
    call_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void call_nc_a16(EmulationState *emu) {
  if (!(*emu->f & C_MASK)) { // C not is set
    call_a16(emu);
  } else {
    *emu->pc += 2;
  }
}

void rst_00(EmulationState *emu) { call(emu, 0x0000); }
void rst_08(EmulationState *emu) { call(emu, 0x0008); }
void rst_10(EmulationState *emu) { call(emu, 0x0010); }
void rst_18(EmulationState *emu) { call(emu, 0x0018); }
void rst_20(EmulationState *emu) { call(emu, 0x0020); }
void rst_28(EmulationState *emu) { call(emu, 0x0028); }
void rst_30(EmulationState *emu) { call(emu, 0x0030); }
void rst_38(EmulationState *emu) { call(emu, 0x0038); }

void pop_bc(EmulationState *emu) { pop(emu, emu->bc); }
void pop_de(EmulationState *emu) { pop(emu, emu->de); }
void pop_hl(EmulationState *emu) { pop(emu, emu->hl); }
void pop_af(EmulationState *emu) { pop(emu, emu->af); }

void push_bc(EmulationState *emu) { push(emu, emu->bc); }
void push_de(EmulationState *emu) { push(emu, emu->de); }
void push_hl(EmulationState *emu) { push(emu, emu->hl); }
void push_af(EmulationState *emu) { push(emu, emu->af); }

void ret(EmulationState *emu) { pop(emu, emu->pc); }
void reti(EmulationState *emu) {
  ei(emu);
  pop(emu, emu->pc);
}

void ret_z(EmulationState *emu) {
  if (*emu->f & Z_MASK) { // Z is set
    ret(emu);
  }
}

void ret_nz(EmulationState *emu) {
  if (!(*emu->f & Z_MASK)) { // C not is set
    ret(emu);
  }
}

void ret_c(EmulationState *emu) {
  if (*emu->f & C_MASK) { // Z is set
    ret(emu);
  }
}

void ret_nc(EmulationState *emu) {
  if (!(*emu->f & C_MASK)) { // C not is set
    ret(emu);
  }
}

void ldh_a8_a(EmulationState *emu) {
  u8 target       = emu->rom[*emu->pc];
  emu->io[target] = *emu->a;
  *emu->pc += 1;
}

void ldh_a_a8(EmulationState *emu) {
  u8 target = emu->rom[*emu->pc];
  *emu->a   = emu->io[target];
  *emu->pc += 1;
}

void ld_ca_a(EmulationState *emu) { emu->io[*emu->c] = *emu->a; }
void ld_a_ca(EmulationState *emu) { *emu->a = emu->io[*emu->c]; }

void inc_bc(EmulationState *emu) { *emu->bc += 1; }
void inc_de(EmulationState *emu) { *emu->de += 1; }
void inc_hl(EmulationState *emu) { *emu->hl += 1; }
void inc_sp(EmulationState *emu) { *emu->sp += 1; }

void inc_b(EmulationState *emu) { inc_regd8(emu, emu->b); }
void inc_d(EmulationState *emu) { inc_regd8(emu, emu->d); }
void inc_h(EmulationState *emu) { inc_regd8(emu, emu->h); }
void inc_ahl(EmulationState *emu) { inc_rega16(emu, *emu->hl); }

void inc_c(EmulationState *emu) { inc_regd8(emu, emu->c); }
void inc_e(EmulationState *emu) { inc_regd8(emu, emu->e); }
void inc_l(EmulationState *emu) { inc_regd8(emu, emu->l); }
void inc_a(EmulationState *emu) { inc_regd8(emu, emu->a); }

void dec_bc(EmulationState *emu) { *emu->bc -= 1; }
void dec_de(EmulationState *emu) { *emu->de -= 1; }
void dec_hl(EmulationState *emu) { *emu->hl -= 1; }
void dec_sp(EmulationState *emu) { *emu->sp -= 1; }

void dec_b(EmulationState *emu) { dec_regd8(emu, emu->b); }
void dec_d(EmulationState *emu) { dec_regd8(emu, emu->d); }
void dec_h(EmulationState *emu) { dec_regd8(emu, emu->h); }
void dec_ahl(EmulationState *emu) { dec_rega16(emu, *emu->hl); }

void dec_c(EmulationState *emu) { dec_regd8(emu, emu->c); }
void dec_e(EmulationState *emu) { dec_regd8(emu, emu->e); }
void dec_l(EmulationState *emu) { dec_regd8(emu, emu->l); }
void dec_a(EmulationState *emu) { dec_regd8(emu, emu->a); }

void ld_b_b(EmulationState *emu) { *emu->b = *emu->b; }
void ld_d_b(EmulationState *emu) { *emu->d = *emu->b; }
void ld_h_b(EmulationState *emu) { *emu->h = *emu->b; }

void ld_b_c(EmulationState *emu) { *emu->b = *emu->c; }
void ld_d_c(EmulationState *emu) { *emu->d = *emu->c; }
void ld_h_c(EmulationState *emu) { *emu->h = *emu->c; }

void ld_b_d(EmulationState *emu) { *emu->b = *emu->d; }
void ld_d_d(EmulationState *emu) { *emu->d = *emu->d; }
void ld_h_d(EmulationState *emu) { *emu->h = *emu->d; }

void ld_b_e(EmulationState *emu) { *emu->b = *emu->e; }
void ld_d_e(EmulationState *emu) { *emu->d = *emu->e; }
void ld_h_e(EmulationState *emu) { *emu->h = *emu->e; }

void ld_b_h(EmulationState *emu) { *emu->b = *emu->h; }
void ld_d_h(EmulationState *emu) { *emu->d = *emu->h; }
void ld_h_h(EmulationState *emu) { *emu->h = *emu->h; }

void ld_b_l(EmulationState *emu) { *emu->b = *emu->l; }
void ld_d_l(EmulationState *emu) { *emu->d = *emu->l; }
void ld_h_l(EmulationState *emu) { *emu->h = *emu->l; }

void ld_b_a(EmulationState *emu) { *emu->b = *emu->a; }
void ld_d_a(EmulationState *emu) { *emu->d = *emu->a; }
void ld_h_a(EmulationState *emu) { *emu->h = *emu->a; }

void ld_c_b(EmulationState *emu) { *emu->c = *emu->b; }
void ld_e_b(EmulationState *emu) { *emu->e = *emu->b; }
void ld_l_b(EmulationState *emu) { *emu->l = *emu->b; }
void ld_a_b(EmulationState *emu) { *emu->a = *emu->b; }

void ld_c_c(EmulationState *emu) { *emu->c = *emu->c; }
void ld_e_c(EmulationState *emu) { *emu->e = *emu->c; }
void ld_l_c(EmulationState *emu) { *emu->l = *emu->c; }
void ld_a_c(EmulationState *emu) { *emu->a = *emu->c; }

void ld_c_d(EmulationState *emu) { *emu->c = *emu->d; }
void ld_e_d(EmulationState *emu) { *emu->e = *emu->d; }
void ld_l_d(EmulationState *emu) { *emu->l = *emu->d; }
void ld_a_d(EmulationState *emu) { *emu->a = *emu->d; }

void ld_c_e(EmulationState *emu) { *emu->c = *emu->e; }
void ld_e_e(EmulationState *emu) { *emu->e = *emu->e; }
void ld_l_e(EmulationState *emu) { *emu->l = *emu->e; }
void ld_a_e(EmulationState *emu) { *emu->a = *emu->e; }

void ld_c_h(EmulationState *emu) { *emu->c = *emu->h; }
void ld_e_h(EmulationState *emu) { *emu->e = *emu->h; }
void ld_l_h(EmulationState *emu) { *emu->l = *emu->h; }
void ld_a_h(EmulationState *emu) { *emu->a = *emu->h; }

void ld_c_l(EmulationState *emu) { *emu->c = *emu->l; }
void ld_e_l(EmulationState *emu) { *emu->e = *emu->l; }
void ld_l_l(EmulationState *emu) { *emu->l = *emu->l; }
void ld_a_l(EmulationState *emu) { *emu->a = *emu->l; }

void ld_c_a(EmulationState *emu) { *emu->c = *emu->a; }
void ld_e_a(EmulationState *emu) { *emu->e = *emu->a; }
void ld_l_a(EmulationState *emu) { *emu->l = *emu->a; }
void ld_a_a(EmulationState *emu) { *emu->a = *emu->a; }

void ld_a16_a(EmulationState *emu) {
  u16 target_addr       = *(u16 *)&emu->rom[*emu->pc];
  emu->mem[target_addr] = *emu->a;
  *emu->pc += 2;
}

void ld_a_a16(EmulationState *emu) {
  u16 target_addr = *(u16 *)&emu->rom[*emu->pc];
  *emu->a         = emu->mem[target_addr];
  *emu->pc += 2;
}

void ld_b_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->b); }
void ld_d_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->d); }
void ld_h_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->h); }
void ld_hl_d8(EmulationState *emu) { ld_rega16_d8(emu, emu->hl); }

void ld_c_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->c); }
void ld_e_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->e); }
void ld_l_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->l); }
void ld_a_d8(EmulationState *emu) { ld_regd8_d8(emu, emu->a); }

void ld_bc_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->bc); }
void ld_de_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->de); }
void ld_hl_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->hl); }
void ld_sp_d16(EmulationState *emu) { ld_regd16_d16(emu, emu->sp); }

void ld_a_bc(EmulationState *emu) { ld_regd8_rega16(emu, emu->a, emu->bc); }
void ld_a_de(EmulationState *emu) { ld_regd8_rega16(emu, emu->a, emu->de); }
void ld_a_hli(EmulationState *emu) {
  ld_regd8_rega16(emu, emu->a, emu->hl);
  inc_hl(emu);
}
void ld_a_hld(EmulationState *emu) {
  ld_regd8_rega16(emu, emu->a, emu->hl);
  dec_hl(emu);
}

void ld_b_hl(EmulationState *emu) { ld_regd8_rega16(emu, emu->b, emu->hl); }
void ld_d_hl(EmulationState *emu) { ld_regd8_rega16(emu, emu->d, emu->hl); }
void ld_h_hl(EmulationState *emu) { ld_regd8_rega16(emu, emu->h, emu->hl); }
void ld_c_hl(EmulationState *emu) { ld_regd8_rega16(emu, emu->c, emu->hl); }
void ld_e_hl(EmulationState *emu) { ld_regd8_rega16(emu, emu->e, emu->hl); }
void ld_l_hl(EmulationState *emu) { ld_regd8_rega16(emu, emu->l, emu->hl); }
void ld_a_hl(EmulationState *emu) { ld_regd8_rega16(emu, emu->a, emu->hl); }

void ld_bc_a(EmulationState *emu) { ld_rega16_regd8(emu, emu->bc, emu->a); }
void ld_de_a(EmulationState *emu) { ld_rega16_regd8(emu, emu->de, emu->a); }
void ld_hli_a(EmulationState *emu) {
  ld_rega16_regd8(emu, emu->hl, emu->a);
  inc_hl(emu);
}
void ld_hld_a(EmulationState *emu) {
  ld_rega16_regd8(emu, emu->hl, emu->a);
  dec_hl(emu);
}

void ld_hl_b(EmulationState *emu) { ld_rega16_regd8(emu, emu->hl, emu->b); }
void ld_hl_c(EmulationState *emu) { ld_rega16_regd8(emu, emu->hl, emu->c); }
void ld_hl_d(EmulationState *emu) { ld_rega16_regd8(emu, emu->hl, emu->d); }
void ld_hl_e(EmulationState *emu) { ld_rega16_regd8(emu, emu->hl, emu->e); }
void ld_hl_h(EmulationState *emu) { ld_rega16_regd8(emu, emu->hl, emu->h); }
void ld_hl_l(EmulationState *emu) { ld_rega16_regd8(emu, emu->hl, emu->l); }
void ld_hl_a(EmulationState *emu) { ld_rega16_regd8(emu, emu->hl, emu->a); }

void add_b(EmulationState *emu) { add_regd8(emu, *emu->b); }
void add_c(EmulationState *emu) { add_regd8(emu, *emu->c); }
void add_d(EmulationState *emu) { add_regd8(emu, *emu->d); }
void add_e(EmulationState *emu) { add_regd8(emu, *emu->e); }
void add_h(EmulationState *emu) { add_regd8(emu, *emu->h); }
void add_l(EmulationState *emu) { add_regd8(emu, *emu->l); }
void add_a(EmulationState *emu) { add_regd8(emu, *emu->a); }

void add_bc(EmulationState *emu) { add_regd16(emu, emu->bc); }
void add_de(EmulationState *emu) { add_regd16(emu, emu->de); }
void add_hl(EmulationState *emu) { add_regd16(emu, emu->hl); }
void add_sp(EmulationState *emu) { add_regd16(emu, emu->sp); }

void sub_b(EmulationState *emu) { sub_regd8(emu, *emu->b); }
void sub_c(EmulationState *emu) { sub_regd8(emu, *emu->c); }
void sub_d(EmulationState *emu) { sub_regd8(emu, *emu->d); }
void sub_e(EmulationState *emu) { sub_regd8(emu, *emu->e); }
void sub_h(EmulationState *emu) { sub_regd8(emu, *emu->h); }
void sub_l(EmulationState *emu) { sub_regd8(emu, *emu->l); }
void sub_a(EmulationState *emu) { sub_regd8(emu, *emu->a); }

void and_b(EmulationState *emu) { and_regd8(emu, *emu->b); }
void and_c(EmulationState *emu) { and_regd8(emu, *emu->c); }
void and_d(EmulationState *emu) { and_regd8(emu, *emu->d); }
void and_e(EmulationState *emu) { and_regd8(emu, *emu->e); }
void and_h(EmulationState *emu) { and_regd8(emu, *emu->h); }
void and_l(EmulationState *emu) { and_regd8(emu, *emu->l); }
void and_a(EmulationState *emu) { and_regd8(emu, *emu->a); }

void or_b(EmulationState *emu) { or_regd8(emu, *emu->b); }
void or_c(EmulationState *emu) { or_regd8(emu, *emu->c); }
void or_d(EmulationState *emu) { or_regd8(emu, *emu->d); }
void or_e(EmulationState *emu) { or_regd8(emu, *emu->e); }
void or_h(EmulationState *emu) { or_regd8(emu, *emu->h); }
void or_l(EmulationState *emu) { or_regd8(emu, *emu->l); }
void or_a(EmulationState *emu) { or_regd8(emu, *emu->a); }

void xor_b(EmulationState *emu) { xor_regd8(emu, *emu->b); }
void xor_c(EmulationState *emu) { xor_regd8(emu, *emu->c); }
void xor_d(EmulationState *emu) { xor_regd8(emu, *emu->d); }
void xor_e(EmulationState *emu) { xor_regd8(emu, *emu->e); }
void xor_h(EmulationState *emu) { xor_regd8(emu, *emu->h); }
void xor_l(EmulationState *emu) { xor_regd8(emu, *emu->l); }
void xor_a(EmulationState *emu) { xor_regd8(emu, *emu->a); }

void cp_d8(EmulationState *emu) {
  u8 value = emu->rom[*emu->pc];
  if (*emu->a == value) {
    set_flags(emu, 1, 1, 0, 0);
  } else if (*emu->a > value) {
    set_flags(emu, 0, 1, 0, 0);
  } else {
    set_flags(emu, 0, 1, 1, 1);
  }
  *emu->pc += 1;
}

void prefix(EmulationState *emu) {
  u8 inst = emu->rom[*emu->pc];
  *emu->pc += 1;

  Instruction instruction = GB_INSTRUCTIONS_PREFIXED[inst];
  printf("Prefixed Inst: %02X\n", inst);

  instruction.execute(emu);
}

Instruction GB_INSTRUCTIONS[256] = {
    [0x00] = {.execute = &nop},
    [0xF3] = {.execute = &di},
    [0xFB] = {.execute = &ei},
    [0xCB] = {.execute = &prefix},

    [0x01] = {.execute = &ld_bc_d16},
    [0x11] = {.execute = &ld_de_d16},
    [0x21] = {.execute = &ld_hl_d16},
    [0x31] = {.execute = &ld_sp_d16},

    [0x40] = {.execute = &ld_b_b},
    [0x50] = {.execute = &ld_d_b},
    [0x60] = {.execute = &ld_h_b},

    [0x41] = {.execute = &ld_b_c},
    [0x51] = {.execute = &ld_d_c},
    [0x61] = {.execute = &ld_h_c},

    [0x42] = {.execute = &ld_b_d},
    [0x52] = {.execute = &ld_d_d},
    [0x62] = {.execute = &ld_h_d},

    [0x43] = {.execute = &ld_b_e},
    [0x53] = {.execute = &ld_d_e},
    [0x63] = {.execute = &ld_h_e},

    [0x44] = {.execute = &ld_b_h},
    [0x54] = {.execute = &ld_d_h},
    [0x64] = {.execute = &ld_h_h},

    [0x45] = {.execute = &ld_b_l},
    [0x55] = {.execute = &ld_d_l},
    [0x65] = {.execute = &ld_h_l},

    [0x46] = {.execute = &ld_b_hl},
    [0x56] = {.execute = &ld_d_hl},
    [0x66] = {.execute = &ld_h_hl},

    [0x47] = {.execute = &ld_b_a},
    [0x57] = {.execute = &ld_d_a},
    [0x67] = {.execute = &ld_h_a},

    [0x48] = {.execute = &ld_c_b},
    [0x58] = {.execute = &ld_e_b},
    [0x68] = {.execute = &ld_l_b},
    [0x78] = {.execute = &ld_a_b},

    [0x49] = {.execute = &ld_c_c},
    [0x59] = {.execute = &ld_e_c},
    [0x69] = {.execute = &ld_l_c},
    [0x79] = {.execute = &ld_a_c},

    [0x4A] = {.execute = &ld_c_d},
    [0x5A] = {.execute = &ld_e_d},
    [0x6A] = {.execute = &ld_l_d},
    [0x7A] = {.execute = &ld_a_d},

    [0x4B] = {.execute = &ld_c_e},
    [0x5B] = {.execute = &ld_e_e},
    [0x6B] = {.execute = &ld_l_e},
    [0x7B] = {.execute = &ld_a_e},

    [0x4C] = {.execute = &ld_c_h},
    [0x5C] = {.execute = &ld_e_h},
    [0x6C] = {.execute = &ld_l_h},
    [0x7C] = {.execute = &ld_a_h},

    [0x4D] = {.execute = &ld_c_l},
    [0x5D] = {.execute = &ld_e_l},
    [0x6D] = {.execute = &ld_l_l},
    [0x7D] = {.execute = &ld_a_l},

    [0x4E] = {.execute = &ld_c_hl},
    [0x5E] = {.execute = &ld_e_hl},
    [0x6E] = {.execute = &ld_l_hl},
    [0x7E] = {.execute = &ld_a_hl},

    [0x4F] = {.execute = &ld_c_a},
    [0x5F] = {.execute = &ld_e_a},
    [0x6F] = {.execute = &ld_l_a},
    [0x7F] = {.execute = &ld_a_a},

    [0x02] = {.execute = &ld_bc_a},
    [0x12] = {.execute = &ld_de_a},
    [0x22] = {.execute = &ld_hli_a},
    [0x32] = {.execute = &ld_hld_a},

    [0xE2] = {.execute = &ld_ca_a},
    [0xF2] = {.execute = &ld_a_ca},

    [0x70] = {.execute = &ld_hl_b},
    [0x71] = {.execute = &ld_hl_c},
    [0x72] = {.execute = &ld_hl_d},
    [0x73] = {.execute = &ld_hl_e},
    [0x74] = {.execute = &ld_hl_h},
    [0x75] = {.execute = &ld_hl_l},
    [0x77] = {.execute = &ld_hl_a},

    [0x03] = {.execute = &inc_bc},
    [0x13] = {.execute = &inc_de},
    [0x23] = {.execute = &inc_hl},
    [0x33] = {.execute = &inc_sp},

    [0x04] = {.execute = &inc_b},
    [0x14] = {.execute = &inc_d},
    [0x24] = {.execute = &inc_h},
    [0x34] = {.execute = &inc_ahl},

    [0x0C] = {.execute = &inc_c},
    [0x1C] = {.execute = &inc_e},
    [0x2C] = {.execute = &inc_l},
    [0x3C] = {.execute = &inc_a},

    [0x05] = {.execute = &dec_b},
    [0x15] = {.execute = &dec_d},
    [0x25] = {.execute = &dec_h},
    [0x35] = {.execute = &dec_ahl},

    [0x0D] = {.execute = &dec_c},
    [0x1D] = {.execute = &dec_e},
    [0x2D] = {.execute = &dec_l},
    [0x3D] = {.execute = &dec_a},

    [0x0B] = {.execute = &dec_bc},
    [0x1B] = {.execute = &dec_de},
    [0x2B] = {.execute = &dec_hl},
    [0x3B] = {.execute = &dec_sp},

    [0x0A] = {.execute = &ld_a_bc},
    [0x1A] = {.execute = &ld_a_de},
    [0x2A] = {.execute = &ld_a_hli},
    [0x3A] = {.execute = &ld_a_hld},

    [0x06] = {.execute = &ld_b_d8},
    [0x16] = {.execute = &ld_d_d8},
    [0x26] = {.execute = &ld_h_d8},
    [0x36] = {.execute = &ld_hl_d8},

    [0x0E] = {.execute = &ld_c_d8},
    [0x1E] = {.execute = &ld_e_d8},
    [0x2E] = {.execute = &ld_l_d8},
    [0x3E] = {.execute = &ld_a_d8},

    [0xEA] = {.execute = &ld_a16_a},
    [0xFA] = {.execute = &ld_a_a16},

    [0xE0] = {.execute = &ldh_a8_a},
    [0xF0] = {.execute = &ldh_a_a8},

    [0x80] = {.execute = &add_b},
    [0x81] = {.execute = &add_c},
    [0x82] = {.execute = &add_d},
    [0x83] = {.execute = &add_e},
    [0x84] = {.execute = &add_h},
    [0x85] = {.execute = &add_l},
    [0x87] = {.execute = &add_a},

    [0x09] = {.execute = &add_bc},
    [0x19] = {.execute = &add_de},
    [0x29] = {.execute = &add_hl},
    [0x39] = {.execute = &add_sp},

    [0x90] = {.execute = &sub_b},
    [0x91] = {.execute = &sub_c},
    [0x92] = {.execute = &sub_d},
    [0x93] = {.execute = &sub_e},
    [0x94] = {.execute = &sub_h},
    [0x95] = {.execute = &sub_l},
    [0x97] = {.execute = &sub_a},

    [0xA0] = {.execute = &and_b},
    [0xA1] = {.execute = &and_c},
    [0xA2] = {.execute = &and_d},
    [0xA3] = {.execute = &and_e},
    [0xA4] = {.execute = &and_h},
    [0xA5] = {.execute = &and_l},
    [0xA7] = {.execute = &and_a},

    [0xA8] = {.execute = &xor_b},
    [0xA9] = {.execute = &xor_c},
    [0xAA] = {.execute = &xor_d},
    [0xAB] = {.execute = &xor_e},
    [0xAC] = {.execute = &xor_h},
    [0xAD] = {.execute = &xor_l},
    [0xAF] = {.execute = &xor_a},

    [0xB0] = {.execute = &or_b},
    [0xB1] = {.execute = &or_c},
    [0xB2] = {.execute = &or_d},
    [0xB3] = {.execute = &or_e},
    [0xB4] = {.execute = &or_h},
    [0xB5] = {.execute = &or_l},
    [0xB7] = {.execute = &or_a},

    [0xC6] = {.execute = &add_d8},
    [0xD6] = {.execute = &sub_d8},
    [0xE6] = {.execute = &and_d8},
    [0xF6] = {.execute = &or_d8},

    [0xEE] = {.execute = &xor_d8},

    [0x2F] = {.execute = &cpl},

    [0xFE] = {.execute = &cp_d8},

    [0xC3] = {.execute = &jp_a16},
    [0xE9] = {.execute = &jp_hl},
    [0xC2] = {.execute = &jp_nz_a16},
    [0xD2] = {.execute = &jp_nc_a16},
    [0xCA] = {.execute = &jp_z_a16},
    [0xDA] = {.execute = &jp_c_a16},

    [0xCD] = {.execute = &call_a16},
    [0xC4] = {.execute = &call_nz_a16},
    [0xD4] = {.execute = &call_nc_a16},
    [0xCC] = {.execute = &call_z_a16},
    [0xDC] = {.execute = &call_c_a16},

    [0xC7] = {.execute = &rst_00},
    [0xD7] = {.execute = &rst_10},
    [0xE7] = {.execute = &rst_20},
    [0xF7] = {.execute = &rst_30},
    [0xCF] = {.execute = &rst_08},
    [0xDF] = {.execute = &rst_18},
    [0xEF] = {.execute = &rst_28},
    [0xFF] = {.execute = &rst_38},

    [0xC1] = {.execute = &pop_bc},
    [0xD1] = {.execute = &pop_de},
    [0xE1] = {.execute = &pop_hl},
    [0xF1] = {.execute = &pop_af},

    [0xC5] = {.execute = &push_bc},
    [0xD5] = {.execute = &push_de},
    [0xE5] = {.execute = &push_hl},
    [0xF5] = {.execute = &push_af},

    [0xC9] = {.execute = &ret},
    [0xD9] = {.execute = &reti},
    [0xC0] = {.execute = &ret_nz},
    [0xD0] = {.execute = &ret_nc},
    [0xC8] = {.execute = &ret_z},
    [0xD8] = {.execute = &ret_c},

    [0x20] = {.execute = &jr_nz_d8},
    [0x30] = {.execute = &jr_nc_d8},

    [0x18] = {.execute = &jr_d8},
    [0x28] = {.execute = &jr_z_d8},
    [0x38] = {.execute = &jr_c_d8},
};

void sla_regd8(EmulationState *emu, u8 *reg) {
  u8 c = *reg & 0b10000000;
  *reg <<= 1;
  set_flags(emu, *reg == 0, 0, 0, c);
}

void sla_rega16(EmulationState *emu, u16 addr) {
  sla_regd8(emu, &emu->mem[addr]);
}

void sra_regd8(EmulationState *emu, u8 *reg) {
  u8 c    = *reg & 0b00000001;
  u8 left = *reg & 0b10000000;
  *reg >>= 1;
  *reg |= left;
  set_flags(emu, *reg == 0, 0, 0, c);
}

void sra_rega16(EmulationState *emu, u16 addr) {
  sra_regd8(emu, &emu->mem[addr]);
}

void swap_regd8(EmulationState *emu, u8 *reg) {
  *reg = (*reg << 4) | (*reg >> 4);
  set_flags(emu, *reg == 0, 0, 0, 0);
}

void swap_rega16(EmulationState *emu, u16 addr) {
  swap_regd8(emu, &emu->mem[addr]);
}

void srl_regd8(EmulationState *emu, u8 *reg) {
  u8 c = *reg & 0b00000001;
  *reg >>= 1;
  set_flags(emu, *reg == 0, 0, 0, c);
}

void srl_rega16(EmulationState *emu, u16 addr) {
  srl_regd8(emu, &emu->mem[addr]);
}

void bit_regd8(EmulationState *emu, u8 *reg, u8 bit) {
  set_flags(emu, (*reg & (1 << bit)) == 0, 0, 1, -1);
}

void bit_rega16(EmulationState *emu, u16 addr, u8 bit) {
  bit_regd8(emu, &emu->mem[addr], bit);
}

void res_regd8(EmulationState *emu, u8 *reg, u8 bit) {
  *reg &= ~(1 << bit);
}

void res_rega16(EmulationState *emu, u16 addr, u8 bit) {
  res_regd8(emu, &emu->mem[addr], bit);
}

void set_regd8(EmulationState *emu, u8 *reg, u8 bit) {
  *reg |= (1 << bit);
}

void set_rega16(EmulationState *emu, u16 addr, u8 bit) {
  set_regd8(emu, &emu->mem[addr], bit);
}

void sla_b(EmulationState *emu) { sla_regd8(emu, emu->b); }
void sla_c(EmulationState *emu) { sla_regd8(emu, emu->c); }
void sla_d(EmulationState *emu) { sla_regd8(emu, emu->d); }
void sla_e(EmulationState *emu) { sla_regd8(emu, emu->e); }
void sla_h(EmulationState *emu) { sla_regd8(emu, emu->h); }
void sla_l(EmulationState *emu) { sla_regd8(emu, emu->h); }
void sla_hl(EmulationState *emu) { sla_rega16(emu, *emu->hl); }
void sla_a(EmulationState *emu) { sla_regd8(emu, emu->a); }

void sra_b(EmulationState *emu) { sra_regd8(emu, emu->b); }
void sra_c(EmulationState *emu) { sra_regd8(emu, emu->c); }
void sra_d(EmulationState *emu) { sra_regd8(emu, emu->d); }
void sra_e(EmulationState *emu) { sra_regd8(emu, emu->e); }
void sra_h(EmulationState *emu) { sra_regd8(emu, emu->h); }
void sra_l(EmulationState *emu) { sra_regd8(emu, emu->h); }
void sra_hl(EmulationState *emu) { sra_rega16(emu, *emu->hl); }
void sra_a(EmulationState *emu) { sra_regd8(emu, emu->a); }

void swap_b(EmulationState *emu) { swap_regd8(emu, emu->b); }
void swap_c(EmulationState *emu) { swap_regd8(emu, emu->c); }
void swap_d(EmulationState *emu) { swap_regd8(emu, emu->d); }
void swap_e(EmulationState *emu) { swap_regd8(emu, emu->e); }
void swap_h(EmulationState *emu) { swap_regd8(emu, emu->h); }
void swap_l(EmulationState *emu) { swap_regd8(emu, emu->l); }
void swap_hl(EmulationState *emu) { swap_rega16(emu, *emu->hl); }
void swap_a(EmulationState *emu) { swap_regd8(emu, emu->a); }

void srl_b(EmulationState *emu) { srl_regd8(emu, emu->b); }
void srl_c(EmulationState *emu) { srl_regd8(emu, emu->c); }
void srl_d(EmulationState *emu) { srl_regd8(emu, emu->d); }
void srl_e(EmulationState *emu) { srl_regd8(emu, emu->e); }
void srl_h(EmulationState *emu) { srl_regd8(emu, emu->h); }
void srl_l(EmulationState *emu) { srl_regd8(emu, emu->h); }
void srl_hl(EmulationState *emu) { srl_rega16(emu, *emu->hl); }
void srl_a(EmulationState *emu) { srl_regd8(emu, emu->a); }

void bit_b0(EmulationState *emu) { bit_regd8(emu, emu->b, 0); }
void bit_c0(EmulationState *emu) { bit_regd8(emu, emu->c, 0); }
void bit_d0(EmulationState *emu) { bit_regd8(emu, emu->d, 0); }
void bit_e0(EmulationState *emu) { bit_regd8(emu, emu->e, 0); }
void bit_h0(EmulationState *emu) { bit_regd8(emu, emu->h, 0); }
void bit_l0(EmulationState *emu) { bit_regd8(emu, emu->l, 0); }
void bit_hl0(EmulationState *emu) { bit_rega16(emu, *emu->hl, 0); }
void bit_a0(EmulationState *emu) { bit_regd8(emu, emu->a, 0); }

void bit_b1(EmulationState *emu) { bit_regd8(emu, emu->b, 1); }
void bit_c1(EmulationState *emu) { bit_regd8(emu, emu->c, 1); }
void bit_d1(EmulationState *emu) { bit_regd8(emu, emu->d, 1); }
void bit_e1(EmulationState *emu) { bit_regd8(emu, emu->e, 1); }
void bit_h1(EmulationState *emu) { bit_regd8(emu, emu->h, 1); }
void bit_l1(EmulationState *emu) { bit_regd8(emu, emu->l, 1); }
void bit_hl1(EmulationState *emu) { bit_rega16(emu, *emu->hl, 1); }
void bit_a1(EmulationState *emu) { bit_regd8(emu, emu->a, 1); }

void bit_b2(EmulationState *emu) { bit_regd8(emu, emu->b, 2); }
void bit_c2(EmulationState *emu) { bit_regd8(emu, emu->c, 2); }
void bit_d2(EmulationState *emu) { bit_regd8(emu, emu->d, 2); }
void bit_e2(EmulationState *emu) { bit_regd8(emu, emu->e, 2); }
void bit_h2(EmulationState *emu) { bit_regd8(emu, emu->h, 2); }
void bit_l2(EmulationState *emu) { bit_regd8(emu, emu->l, 2); }
void bit_hl2(EmulationState *emu) { bit_rega16(emu, *emu->hl, 2); }
void bit_a2(EmulationState *emu) { bit_regd8(emu, emu->a, 2); }

void bit_b3(EmulationState *emu) { bit_regd8(emu, emu->b, 3); }
void bit_c3(EmulationState *emu) { bit_regd8(emu, emu->c, 3); }
void bit_d3(EmulationState *emu) { bit_regd8(emu, emu->d, 3); }
void bit_e3(EmulationState *emu) { bit_regd8(emu, emu->e, 3); }
void bit_h3(EmulationState *emu) { bit_regd8(emu, emu->h, 3); }
void bit_l3(EmulationState *emu) { bit_regd8(emu, emu->l, 3); }
void bit_hl3(EmulationState *emu) { bit_rega16(emu, *emu->hl, 3); }
void bit_a3(EmulationState *emu) { bit_regd8(emu, emu->a, 3); }

void bit_b4(EmulationState *emu) { bit_regd8(emu, emu->b, 4); }
void bit_c4(EmulationState *emu) { bit_regd8(emu, emu->c, 4); }
void bit_d4(EmulationState *emu) { bit_regd8(emu, emu->d, 4); }
void bit_e4(EmulationState *emu) { bit_regd8(emu, emu->e, 4); }
void bit_h4(EmulationState *emu) { bit_regd8(emu, emu->h, 4); }
void bit_l4(EmulationState *emu) { bit_regd8(emu, emu->l, 4); }
void bit_hl4(EmulationState *emu) { bit_rega16(emu, *emu->hl, 4); }
void bit_a4(EmulationState *emu) { bit_regd8(emu, emu->a, 4); }

void bit_b5(EmulationState *emu) { bit_regd8(emu, emu->b, 5); }
void bit_c5(EmulationState *emu) { bit_regd8(emu, emu->c, 5); }
void bit_d5(EmulationState *emu) { bit_regd8(emu, emu->d, 5); }
void bit_e5(EmulationState *emu) { bit_regd8(emu, emu->e, 5); }
void bit_h5(EmulationState *emu) { bit_regd8(emu, emu->h, 5); }
void bit_l5(EmulationState *emu) { bit_regd8(emu, emu->l, 5); }
void bit_hl5(EmulationState *emu) { bit_rega16(emu, *emu->hl, 5); }
void bit_a5(EmulationState *emu) { bit_regd8(emu, emu->a, 5); }

void bit_b6(EmulationState *emu) { bit_regd8(emu, emu->b, 6); }
void bit_c6(EmulationState *emu) { bit_regd8(emu, emu->c, 6); }
void bit_d6(EmulationState *emu) { bit_regd8(emu, emu->d, 6); }
void bit_e6(EmulationState *emu) { bit_regd8(emu, emu->e, 6); }
void bit_h6(EmulationState *emu) { bit_regd8(emu, emu->h, 6); }
void bit_l6(EmulationState *emu) { bit_regd8(emu, emu->l, 6); }
void bit_hl6(EmulationState *emu) { bit_rega16(emu, *emu->hl, 6); }
void bit_a6(EmulationState *emu) { bit_regd8(emu, emu->a, 6); }

void bit_b7(EmulationState *emu) { bit_regd8(emu, emu->b, 7); }
void bit_c7(EmulationState *emu) { bit_regd8(emu, emu->c, 7); }
void bit_d7(EmulationState *emu) { bit_regd8(emu, emu->d, 7); }
void bit_e7(EmulationState *emu) { bit_regd8(emu, emu->e, 7); }
void bit_h7(EmulationState *emu) { bit_regd8(emu, emu->h, 7); }
void bit_l7(EmulationState *emu) { bit_regd8(emu, emu->l, 7); }
void bit_hl7(EmulationState *emu) { bit_rega16(emu, *emu->hl, 7); }
void bit_a7(EmulationState *emu) { bit_regd8(emu, emu->a, 7); }

void res_b0(EmulationState *emu) { res_regd8(emu, emu->b, 0); }
void res_c0(EmulationState *emu) { res_regd8(emu, emu->c, 0); }
void res_d0(EmulationState *emu) { res_regd8(emu, emu->d, 0); }
void res_e0(EmulationState *emu) { res_regd8(emu, emu->e, 0); }
void res_h0(EmulationState *emu) { res_regd8(emu, emu->h, 0); }
void res_l0(EmulationState *emu) { res_regd8(emu, emu->l, 0); }
void res_hl0(EmulationState *emu) { res_rega16(emu, *emu->hl, 0); }
void res_a0(EmulationState *emu) { res_regd8(emu, emu->a, 0); }

void res_b1(EmulationState *emu) { res_regd8(emu, emu->b, 1); }
void res_c1(EmulationState *emu) { res_regd8(emu, emu->c, 1); }
void res_d1(EmulationState *emu) { res_regd8(emu, emu->d, 1); }
void res_e1(EmulationState *emu) { res_regd8(emu, emu->e, 1); }
void res_h1(EmulationState *emu) { res_regd8(emu, emu->h, 1); }
void res_l1(EmulationState *emu) { res_regd8(emu, emu->l, 1); }
void res_hl1(EmulationState *emu) { res_rega16(emu, *emu->hl, 1); }
void res_a1(EmulationState *emu) { res_regd8(emu, emu->a, 1); }

void res_b2(EmulationState *emu) { res_regd8(emu, emu->b, 2); }
void res_c2(EmulationState *emu) { res_regd8(emu, emu->c, 2); }
void res_d2(EmulationState *emu) { res_regd8(emu, emu->d, 2); }
void res_e2(EmulationState *emu) { res_regd8(emu, emu->e, 2); }
void res_h2(EmulationState *emu) { res_regd8(emu, emu->h, 2); }
void res_l2(EmulationState *emu) { res_regd8(emu, emu->l, 2); }
void res_hl2(EmulationState *emu) { res_rega16(emu, *emu->hl, 2); }
void res_a2(EmulationState *emu) { res_regd8(emu, emu->a, 2); }

void res_b3(EmulationState *emu) { res_regd8(emu, emu->b, 3); }
void res_c3(EmulationState *emu) { res_regd8(emu, emu->c, 3); }
void res_d3(EmulationState *emu) { res_regd8(emu, emu->d, 3); }
void res_e3(EmulationState *emu) { res_regd8(emu, emu->e, 3); }
void res_h3(EmulationState *emu) { res_regd8(emu, emu->h, 3); }
void res_l3(EmulationState *emu) { res_regd8(emu, emu->l, 3); }
void res_hl3(EmulationState *emu) { res_rega16(emu, *emu->hl, 3); }
void res_a3(EmulationState *emu) { res_regd8(emu, emu->a, 3); }

void res_b4(EmulationState *emu) { res_regd8(emu, emu->b, 4); }
void res_c4(EmulationState *emu) { res_regd8(emu, emu->c, 4); }
void res_d4(EmulationState *emu) { res_regd8(emu, emu->d, 4); }
void res_e4(EmulationState *emu) { res_regd8(emu, emu->e, 4); }
void res_h4(EmulationState *emu) { res_regd8(emu, emu->h, 4); }
void res_l4(EmulationState *emu) { res_regd8(emu, emu->l, 4); }
void res_hl4(EmulationState *emu) { res_rega16(emu, *emu->hl, 4); }
void res_a4(EmulationState *emu) { res_regd8(emu, emu->a, 4); }

void res_b5(EmulationState *emu) { res_regd8(emu, emu->b, 5); }
void res_c5(EmulationState *emu) { res_regd8(emu, emu->c, 5); }
void res_d5(EmulationState *emu) { res_regd8(emu, emu->d, 5); }
void res_e5(EmulationState *emu) { res_regd8(emu, emu->e, 5); }
void res_h5(EmulationState *emu) { res_regd8(emu, emu->h, 5); }
void res_l5(EmulationState *emu) { res_regd8(emu, emu->l, 5); }
void res_hl5(EmulationState *emu) { res_rega16(emu, *emu->hl, 5); }
void res_a5(EmulationState *emu) { res_regd8(emu, emu->a, 5); }

void res_b6(EmulationState *emu) { res_regd8(emu, emu->b, 6); }
void res_c6(EmulationState *emu) { res_regd8(emu, emu->c, 6); }
void res_d6(EmulationState *emu) { res_regd8(emu, emu->d, 6); }
void res_e6(EmulationState *emu) { res_regd8(emu, emu->e, 6); }
void res_h6(EmulationState *emu) { res_regd8(emu, emu->h, 6); }
void res_l6(EmulationState *emu) { res_regd8(emu, emu->l, 6); }
void res_hl6(EmulationState *emu) { res_rega16(emu, *emu->hl, 6); }
void res_a6(EmulationState *emu) { res_regd8(emu, emu->a, 6); }

void res_b7(EmulationState *emu) { res_regd8(emu, emu->b, 7); }
void res_c7(EmulationState *emu) { res_regd8(emu, emu->c, 7); }
void res_d7(EmulationState *emu) { res_regd8(emu, emu->d, 7); }
void res_e7(EmulationState *emu) { res_regd8(emu, emu->e, 7); }
void res_h7(EmulationState *emu) { res_regd8(emu, emu->h, 7); }
void res_l7(EmulationState *emu) { res_regd8(emu, emu->l, 7); }
void res_hl7(EmulationState *emu) { res_rega16(emu, *emu->hl, 7); }
void res_a7(EmulationState *emu) { res_regd8(emu, emu->a, 7); }

void set_b0(EmulationState *emu) { set_regd8(emu, emu->b, 0); }
void set_c0(EmulationState *emu) { set_regd8(emu, emu->c, 0); }
void set_d0(EmulationState *emu) { set_regd8(emu, emu->d, 0); }
void set_e0(EmulationState *emu) { set_regd8(emu, emu->e, 0); }
void set_h0(EmulationState *emu) { set_regd8(emu, emu->h, 0); }
void set_l0(EmulationState *emu) { set_regd8(emu, emu->l, 0); }
void set_hl0(EmulationState *emu) { set_rega16(emu, *emu->hl, 0); }
void set_a0(EmulationState *emu) { set_regd8(emu, emu->a, 0); }

void set_b1(EmulationState *emu) { set_regd8(emu, emu->b, 1); }
void set_c1(EmulationState *emu) { set_regd8(emu, emu->c, 1); }
void set_d1(EmulationState *emu) { set_regd8(emu, emu->d, 1); }
void set_e1(EmulationState *emu) { set_regd8(emu, emu->e, 1); }
void set_h1(EmulationState *emu) { set_regd8(emu, emu->h, 1); }
void set_l1(EmulationState *emu) { set_regd8(emu, emu->l, 1); }
void set_hl1(EmulationState *emu) { set_rega16(emu, *emu->hl, 1); }
void set_a1(EmulationState *emu) { set_regd8(emu, emu->a, 1); }

void set_b2(EmulationState *emu) { set_regd8(emu, emu->b, 2); }
void set_c2(EmulationState *emu) { set_regd8(emu, emu->c, 2); }
void set_d2(EmulationState *emu) { set_regd8(emu, emu->d, 2); }
void set_e2(EmulationState *emu) { set_regd8(emu, emu->e, 2); }
void set_h2(EmulationState *emu) { set_regd8(emu, emu->h, 2); }
void set_l2(EmulationState *emu) { set_regd8(emu, emu->l, 2); }
void set_hl2(EmulationState *emu) { set_rega16(emu, *emu->hl, 2); }
void set_a2(EmulationState *emu) { set_regd8(emu, emu->a, 2); }

void set_b3(EmulationState *emu) { set_regd8(emu, emu->b, 3); }
void set_c3(EmulationState *emu) { set_regd8(emu, emu->c, 3); }
void set_d3(EmulationState *emu) { set_regd8(emu, emu->d, 3); }
void set_e3(EmulationState *emu) { set_regd8(emu, emu->e, 3); }
void set_h3(EmulationState *emu) { set_regd8(emu, emu->h, 3); }
void set_l3(EmulationState *emu) { set_regd8(emu, emu->l, 3); }
void set_hl3(EmulationState *emu) { set_rega16(emu, *emu->hl, 3); }
void set_a3(EmulationState *emu) { set_regd8(emu, emu->a, 3); }

void set_b4(EmulationState *emu) { set_regd8(emu, emu->b, 4); }
void set_c4(EmulationState *emu) { set_regd8(emu, emu->c, 4); }
void set_d4(EmulationState *emu) { set_regd8(emu, emu->d, 4); }
void set_e4(EmulationState *emu) { set_regd8(emu, emu->e, 4); }
void set_h4(EmulationState *emu) { set_regd8(emu, emu->h, 4); }
void set_l4(EmulationState *emu) { set_regd8(emu, emu->l, 4); }
void set_hl4(EmulationState *emu) { set_rega16(emu, *emu->hl, 4); }
void set_a4(EmulationState *emu) { set_regd8(emu, emu->a, 4); }

void set_b5(EmulationState *emu) { set_regd8(emu, emu->b, 5); }
void set_c5(EmulationState *emu) { set_regd8(emu, emu->c, 5); }
void set_d5(EmulationState *emu) { set_regd8(emu, emu->d, 5); }
void set_e5(EmulationState *emu) { set_regd8(emu, emu->e, 5); }
void set_h5(EmulationState *emu) { set_regd8(emu, emu->h, 5); }
void set_l5(EmulationState *emu) { set_regd8(emu, emu->l, 5); }
void set_hl5(EmulationState *emu) { set_rega16(emu, *emu->hl, 5); }
void set_a5(EmulationState *emu) { set_regd8(emu, emu->a, 5); }

void set_b6(EmulationState *emu) { set_regd8(emu, emu->b, 6); }
void set_c6(EmulationState *emu) { set_regd8(emu, emu->c, 6); }
void set_d6(EmulationState *emu) { set_regd8(emu, emu->d, 6); }
void set_e6(EmulationState *emu) { set_regd8(emu, emu->e, 6); }
void set_h6(EmulationState *emu) { set_regd8(emu, emu->h, 6); }
void set_l6(EmulationState *emu) { set_regd8(emu, emu->l, 6); }
void set_hl6(EmulationState *emu) { set_rega16(emu, *emu->hl, 6); }
void set_a6(EmulationState *emu) { set_regd8(emu, emu->a, 6); }

void set_b7(EmulationState *emu) { set_regd8(emu, emu->b, 7); }
void set_c7(EmulationState *emu) { set_regd8(emu, emu->c, 7); }
void set_d7(EmulationState *emu) { set_regd8(emu, emu->d, 7); }
void set_e7(EmulationState *emu) { set_regd8(emu, emu->e, 7); }
void set_h7(EmulationState *emu) { set_regd8(emu, emu->h, 7); }
void set_l7(EmulationState *emu) { set_regd8(emu, emu->l, 7); }
void set_hl7(EmulationState *emu) { set_rega16(emu, *emu->hl, 7); }
void set_a7(EmulationState *emu) { set_regd8(emu, emu->a, 7); }

Instruction GB_INSTRUCTIONS_PREFIXED[256] = {
    [0x20] = {.execute = &sla_b},
    [0x21] = {.execute = &sla_c},
    [0x22] = {.execute = &sla_d},
    [0x23] = {.execute = &sla_e},
    [0x24] = {.execute = &sla_h},
    [0x25] = {.execute = &sla_l},
    [0x26] = {.execute = &sla_hl},
    [0x27] = {.execute = &sla_a},

    [0x28] = {.execute = &sra_b},
    [0x29] = {.execute = &sra_c},
    [0x2A] = {.execute = &sra_d},
    [0x2B] = {.execute = &sra_e},
    [0x2C] = {.execute = &sra_h},
    [0x2D] = {.execute = &sra_l},
    [0x2E] = {.execute = &sra_hl},
    [0x2F] = {.execute = &sra_a},

    [0x30] = {.execute = &swap_b},
    [0x31] = {.execute = &swap_c},
    [0x32] = {.execute = &swap_d},
    [0x33] = {.execute = &swap_e},
    [0x34] = {.execute = &swap_h},
    [0x35] = {.execute = &swap_l},
    [0x36] = {.execute = &swap_hl},
    [0x37] = {.execute = &swap_a},

    [0x38] = {.execute = &srl_b},
    [0x39] = {.execute = &srl_c},
    [0x3A] = {.execute = &srl_d},
    [0x3B] = {.execute = &srl_e},
    [0x3C] = {.execute = &srl_h},
    [0x3D] = {.execute = &srl_l},
    [0x3E] = {.execute = &srl_hl},
    [0x3F] = {.execute = &srl_a},

    [0x40] = {.execute = &bit_b0},
    [0x41] = {.execute = &bit_c0},
    [0x42] = {.execute = &bit_d0},
    [0x43] = {.execute = &bit_e0},
    [0x44] = {.execute = &bit_h0},
    [0x45] = {.execute = &bit_l0},
    [0x46] = {.execute = &bit_hl0},
    [0x47] = {.execute = &bit_a0},

    [0x48] = {.execute = &bit_b1},
    [0x49] = {.execute = &bit_c1},
    [0x4A] = {.execute = &bit_d1},
    [0x4B] = {.execute = &bit_e1},
    [0x4C] = {.execute = &bit_h1},
    [0x4D] = {.execute = &bit_l1},
    [0x4E] = {.execute = &bit_hl1},
    [0x4F] = {.execute = &bit_a1},

    [0x50] = {.execute = &bit_b2},
    [0x51] = {.execute = &bit_c2},
    [0x52] = {.execute = &bit_d2},
    [0x53] = {.execute = &bit_e2},
    [0x54] = {.execute = &bit_h2},
    [0x55] = {.execute = &bit_l2},
    [0x56] = {.execute = &bit_hl2},
    [0x57] = {.execute = &bit_a2},

    [0x58] = {.execute = &bit_b3},
    [0x59] = {.execute = &bit_c3},
    [0x5A] = {.execute = &bit_d3},
    [0x5B] = {.execute = &bit_e3},
    [0x5C] = {.execute = &bit_h3},
    [0x5D] = {.execute = &bit_l3},
    [0x5E] = {.execute = &bit_hl3},
    [0x5F] = {.execute = &bit_a3},

    [0x60] = {.execute = &bit_b4},
    [0x61] = {.execute = &bit_c4},
    [0x62] = {.execute = &bit_d4},
    [0x63] = {.execute = &bit_e4},
    [0x64] = {.execute = &bit_h4},
    [0x65] = {.execute = &bit_l4},
    [0x66] = {.execute = &bit_hl4},
    [0x67] = {.execute = &bit_a4},

    [0x68] = {.execute = &bit_b5},
    [0x69] = {.execute = &bit_c5},
    [0x6A] = {.execute = &bit_d5},
    [0x6B] = {.execute = &bit_e5},
    [0x6C] = {.execute = &bit_h5},
    [0x6D] = {.execute = &bit_l5},
    [0x6E] = {.execute = &bit_hl5},
    [0x6F] = {.execute = &bit_a5},

    [0x70] = {.execute = &bit_b6},
    [0x71] = {.execute = &bit_c6},
    [0x72] = {.execute = &bit_d6},
    [0x73] = {.execute = &bit_e6},
    [0x74] = {.execute = &bit_h6},
    [0x75] = {.execute = &bit_l6},
    [0x76] = {.execute = &bit_hl6},
    [0x77] = {.execute = &bit_a6},

    [0x78] = {.execute = &bit_b7},
    [0x79] = {.execute = &bit_c7},
    [0x7A] = {.execute = &bit_d7},
    [0x7B] = {.execute = &bit_e7},
    [0x7C] = {.execute = &bit_h7},
    [0x7D] = {.execute = &bit_l7},
    [0x7E] = {.execute = &bit_hl7},
    [0x7F] = {.execute = &bit_a7},

    [0x80] = {.execute = &res_b0},
    [0x81] = {.execute = &res_c0},
    [0x82] = {.execute = &res_d0},
    [0x83] = {.execute = &res_e0},
    [0x84] = {.execute = &res_h0},
    [0x85] = {.execute = &res_l0},
    [0x86] = {.execute = &res_hl0},
    [0x87] = {.execute = &res_a0},

    [0x88] = {.execute = &res_b1},
    [0x89] = {.execute = &res_c1},
    [0x8A] = {.execute = &res_d1},
    [0x8B] = {.execute = &res_e1},
    [0x8C] = {.execute = &res_h1},
    [0x8D] = {.execute = &res_l1},
    [0x8E] = {.execute = &res_hl1},
    [0x8F] = {.execute = &res_a1},

    [0x90] = {.execute = &res_b2},
    [0x91] = {.execute = &res_c2},
    [0x92] = {.execute = &res_d2},
    [0x93] = {.execute = &res_e2},
    [0x94] = {.execute = &res_h2},
    [0x95] = {.execute = &res_l2},
    [0x96] = {.execute = &res_hl2},
    [0x97] = {.execute = &res_a2},

    [0x98] = {.execute = &res_b3},
    [0x99] = {.execute = &res_c3},
    [0x9A] = {.execute = &res_d3},
    [0x9B] = {.execute = &res_e3},
    [0x9C] = {.execute = &res_h3},
    [0x9D] = {.execute = &res_l3},
    [0x9E] = {.execute = &res_hl3},
    [0x9F] = {.execute = &res_a3},

    [0xA0] = {.execute = &res_b4},
    [0xA1] = {.execute = &res_c4},
    [0xA2] = {.execute = &res_d4},
    [0xA3] = {.execute = &res_e4},
    [0xA4] = {.execute = &res_h4},
    [0xA5] = {.execute = &res_l4},
    [0xA6] = {.execute = &res_hl4},
    [0xA7] = {.execute = &res_a4},

    [0xA8] = {.execute = &res_b5},
    [0xA9] = {.execute = &res_c5},
    [0xAA] = {.execute = &res_d5},
    [0xAB] = {.execute = &res_e5},
    [0xAC] = {.execute = &res_h5},
    [0xAD] = {.execute = &res_l5},
    [0xAE] = {.execute = &res_hl5},
    [0xAF] = {.execute = &res_a5},

    [0xB0] = {.execute = &res_b6},
    [0xB1] = {.execute = &res_c6},
    [0xB2] = {.execute = &res_d6},
    [0xB3] = {.execute = &res_e6},
    [0xB4] = {.execute = &res_h6},
    [0xB5] = {.execute = &res_l6},
    [0xB6] = {.execute = &res_hl6},
    [0xB7] = {.execute = &res_a6},

    [0xB8] = {.execute = &res_b7},
    [0xB9] = {.execute = &res_c7},
    [0xBA] = {.execute = &res_d7},
    [0xBB] = {.execute = &res_e7},
    [0xBC] = {.execute = &res_h7},
    [0xBD] = {.execute = &res_l7},
    [0xBE] = {.execute = &res_hl7},
    [0xBF] = {.execute = &res_a7},

    [0xC0] = {.execute = &set_b0},
    [0xC1] = {.execute = &set_c0},
    [0xC2] = {.execute = &set_d0},
    [0xC3] = {.execute = &set_e0},
    [0xC4] = {.execute = &set_h0},
    [0xC5] = {.execute = &set_l0},
    [0xC6] = {.execute = &set_hl0},
    [0xC7] = {.execute = &set_a0},

    [0xC8] = {.execute = &set_b1},
    [0xC9] = {.execute = &set_c1},
    [0xCA] = {.execute = &set_d1},
    [0xCB] = {.execute = &set_e1},
    [0xCC] = {.execute = &set_h1},
    [0xCD] = {.execute = &set_l1},
    [0xCE] = {.execute = &set_hl1},
    [0xCF] = {.execute = &set_a1},

    [0xD0] = {.execute = &set_b2},
    [0xD1] = {.execute = &set_c2},
    [0xD2] = {.execute = &set_d2},
    [0xD3] = {.execute = &set_e2},
    [0xD4] = {.execute = &set_h2},
    [0xD5] = {.execute = &set_l2},
    [0xD6] = {.execute = &set_hl2},
    [0xD7] = {.execute = &set_a2},

    [0xD8] = {.execute = &set_b3},
    [0xD9] = {.execute = &set_c3},
    [0xDA] = {.execute = &set_d3},
    [0xDB] = {.execute = &set_e3},
    [0xDC] = {.execute = &set_h3},
    [0xDD] = {.execute = &set_l3},
    [0xDE] = {.execute = &set_hl3},
    [0xDF] = {.execute = &set_a3},

    [0xE0] = {.execute = &set_b4},
    [0xE1] = {.execute = &set_c4},
    [0xE2] = {.execute = &set_d4},
    [0xE3] = {.execute = &set_e4},
    [0xE4] = {.execute = &set_h4},
    [0xE5] = {.execute = &set_l4},
    [0xE6] = {.execute = &set_hl4},
    [0xE7] = {.execute = &set_a4},

    [0xE8] = {.execute = &set_b5},
    [0xE9] = {.execute = &set_c5},
    [0xEA] = {.execute = &set_d5},
    [0xEB] = {.execute = &set_e5},
    [0xEC] = {.execute = &set_h5},
    [0xED] = {.execute = &set_l5},
    [0xEE] = {.execute = &set_hl5},
    [0xEF] = {.execute = &set_a5},

    [0xF0] = {.execute = &set_b6},
    [0xF1] = {.execute = &set_c6},
    [0xF2] = {.execute = &set_d6},
    [0xF3] = {.execute = &set_e6},
    [0xF4] = {.execute = &set_h6},
    [0xF5] = {.execute = &set_l6},
    [0xF6] = {.execute = &set_hl6},
    [0xF7] = {.execute = &set_a6},

    [0xF8] = {.execute = &set_b7},
    [0xF9] = {.execute = &set_c7},
    [0xFA] = {.execute = &set_d7},
    [0xFB] = {.execute = &set_e7},
    [0xFC] = {.execute = &set_h7},
    [0xFD] = {.execute = &set_l7},
    [0xFE] = {.execute = &set_hl7},
    [0xFF] = {.execute = &set_a7},
};
