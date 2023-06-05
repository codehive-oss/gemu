#include "emulation.h"
#include "instruction.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

// https://gbdev.io/pandocs/Interrupts.html
bool handle_interrupt(EmulationState *emu) {
  // https://gbdev.io/pandocs/Interrupt_Sources.html#int-40--vblank-interrupt
  if (emu->mem[LY] == 144) {
    emu->mem[IF] |= IF_VBLANK;
  }

  u8 interrupt = emu->mem[IE] & emu->mem[IF];
  if (emu->ime && interrupt) {
    if (interrupt & IF_VBLANK) {
      nop(emu);
      call(emu, 0x0040);
      emu->mem[IF] &= ~IF_VBLANK;
      emu->ime = false;
      return true;
    } else if (interrupt & IF_LCD_STAT) {
      nop(emu);
      call(emu, 0x0048);
      emu->mem[IF] &= ~IF_LCD_STAT;
      emu->ime = false;
      return true;
    } else if (interrupt & IF_TIMER) {
      nop(emu);
      call(emu, 0x0050);
      emu->ime = false;
      return true;
    } else if (interrupt & IF_SERIAL) {
      nop(emu);
      call(emu, 0x0058);
      emu->mem[IF] &= ~IF_SERIAL;
      emu->ime = false;
      return true;
    } else if (interrupt & IF_JOYPAD) {
      nop(emu);
      call(emu, 0x0060);
      emu->mem[IF] &= ~IF_JOYPAD;
      emu->ime = false;
      return true;
    }
  }
  return false;
}

bool handle_instruction(EmulationState *emu, u8 inst) {
  *emu->pc += 1;
  Instruction instruction = GB_INSTRUCTIONS[inst];
  instruction.execute(emu);

  // TODO: Fix this with real mcycle
  emu->mcycle += 1;
  emu->mem[LY] = ((emu->mem[LY] + rand() % 5) % 154);
  return true;
}

// https://gbdev.io/pandocs/Joypad_Input.html
void handle_gameinput(EmulationState *emu, GameInput *input) {
  if (!(emu->mem[IO] & (1 << 4))) {
    // Direction button select
    BIT_SET(emu->mem[IO], 0, !input->right);
    BIT_SET(emu->mem[IO], 1, !input->left);
    BIT_SET(emu->mem[IO], 2, !input->up);
    BIT_SET(emu->mem[IO], 3, !input->down);
  } else if (!(emu->mem[IO] & (1 << 5))) {
    // Action button select
    BIT_SET(emu->mem[IO], 0, !input->a);
    BIT_SET(emu->mem[IO], 1, !input->b);
    BIT_SET(emu->mem[IO], 2, !input->select);
    BIT_SET(emu->mem[IO], 3, !input->start);
  } else {
    emu->mem[IO] |= 0x0F;
  }
}

void print_header(EmulationState *emu) {
  printf("-------------START HEADERS--------------\n");
  printf("Title: %s\n", emu->header->title);
  printf("Rom Type: %s\n", ROM_TYPES[(u8)emu->header->type]);
  printf("Rom Size: %dKB\n", 32 * (1 << emu->header->rom_size));
  const char *license;
  if (emu->header->lic_code == 0x33) {
    license = NEW_LICENSEE_CODE[emu->header->new_lic_code];
  } else {
    license = OLD_LICENSEE_CODE[emu->header->lic_code];
  }
  printf("License: %s\n", license);
  printf("Destination: %s\n", DESTINATION_CODE[emu->header->dest_code]);
  printf("Version: %d\n", emu->header->version);
  printf("SGB support: %s\n", emu->header->sgb_flag == 0x03 ? "true" : "false");

  // https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
  u8 checksum = 0;
  for (u16 address = 0x0134; address <= 0x014C; address++)
    checksum = checksum - emu->rom[address] - 1;

  printf("Header Checksum: %d\n", emu->header->checksum);
  printf("Computed Value:  %d\n", checksum);
  printf("--------------END HEADERS---------------\n\n");
}
