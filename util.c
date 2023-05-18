#include "./util.h"
#include "./types.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// https://gbdev.io/pandocs/The_Cartridge_Header.html#0147--cartridge-type
const char *ROM_TYPES[256] = {
    [0x00] = "ROM ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1+RAM",
    [0x03] = "MBC1+RAM+BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2+BATTERY",
    [0x08] = "ROM+RAM 1",
    [0x09] = "ROM+RAM+BATTERY 1",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01+RAM",
    [0x0D] = "MMM01+RAM+BATTERY",
    [0x0F] = "MBC3+TIMER+BATTERY",
    [0x10] = "MBC3+TIMER+RAM+BATTERY 2",
    [0x11] = "MBC3",
    [0x12] = "MBC3+RAM 2",
    [0x13] = "MBC3+RAM+BATTERY 2",
    [0x19] = "MBC5",
    [0x1A] = "MBC5+RAM",
    [0x1B] = "MBC5+RAM+BATTERY",
    [0x1C] = "MBC5+RUMBLE",
    [0x1D] = "MBC5+RUMBLE+RAM",
    [0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
    [0xFC] = "POCKET CAMERA",
    [0xFD] = "BANDAI TAMA5",
    [0xFE] = "HuC3",
    [0xFF] = "HuC1+RAM+BATTERY",
};

// https://gbdev.io/pandocs/The_Cartridge_Header.html#0144-0145--new-licensee-code
const char *NEW_LICENSEE_CODE[256] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)",
};

const char *DESTINATION_CODE[2] = {
    [0x00] = "Japan (and possibly overseas)",
    [0x01] = "Overseas only",
};

const char *OLD_LICENSEE_CODE[256] = {
    [0x00] = "None",
    [0x01] = "Nintendo",
    [0x08] = "Capcom",
    [0x09] = "Hot-B",
    [0x0A] = "Jaleco",
    [0x0B] = "Coconuts Japan",
    [0x0C] = "Elite Systems",
    [0x13] = "EA (Electronic Arts)",
    [0x18] = "Hudsonsoft",
    [0x19] = "ITC Entertainment",
    [0x1A] = "Yanoman",
    [0x1D] = "Japan Clary",
    [0x1F] = "Virgin Interactive",
    [0x24] = "PCM Complete",
    [0x25] = "San-X",
    [0x28] = "Kotobuki Systems",
    [0x29] = "Seta",
    [0x30] = "Infogrames",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Indicates that the New licensee code should be used instead.",
    [0x34] = "Konami",
    [0x35] = "HectorSoft",
    [0x38] = "Capcom",
    [0x39] = "Banpresto",
    [0x3C] = ".Entertainment i",
    [0x3E] = "Gremlin",
    [0x41] = "Ubisoft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "Angel",
    [0x47] = "Spectrum Holoby",
    [0x49] = "Irem",
    [0x4A] = "Virgin Interactive",
    [0x4D] = "Malibu",
    [0x4F] = "U.S. Gold",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American Sammy",
    [0x54] = "GameTek",
    [0x55] = "Park Place",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x59] = "Milton Bradley",
    [0x5A] = "Mindscape",
    [0x5B] = "Romstar",
    [0x5C] = "Naxat Soft",
    [0x5D] = "Tradewest",
    [0x60] = "Titus",
    [0x61] = "Virgin Interactive",
    [0x67] = "Ocean Interactive",
    [0x69] = "EA (Electronic Arts)",
    [0x6E] = "Elite Systems",
    [0x6F] = "Electro Brain",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "Sculptered Soft",
    [0x75] = "The Sales Curve",
    [0x78] = "t.hq",
    [0x79] = "Accolade",
    [0x7A] = "Triffix Entertainment",
    [0x7C] = "Microprose",
    [0x7F] = "Kemco",
    [0x80] = "Misawa Entertainment",
    [0x83] = "Lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x8B] = "Bullet-Proof Software",
    [0x8C] = "Vic Tokai",
    [0x8E] = "Ape",
    [0x8F] = "I’Max",
    [0x91] = "Chunsoft Co.",
    [0x92] = "Video System",
    [0x93] = "Tsubaraya Productions Co.",
    [0x95] = "Varie Corporation",
    [0x96] = "Yonezawa/S’Pal",
    [0x97] = "Kaneko",
    [0x99] = "Arc",
    [0x9A] = "Nihon Bussan",
    [0x9B] = "Tecmo",
    [0x9C] = "Imagineer",
    [0x9D] = "Banpresto",
    [0x9F] = "Nova",
    [0xA1] = "Hori Electric",
    [0xA2] = "Bandai",
    [0xA4] = "Konami",
    [0xA6] = "Kawada",
    [0xA7] = "Takara",
    [0xA9] = "Technos Japan",
    [0xAA] = "Broderbund",
    [0xAC] = "Toei Animation",
    [0xAD] = "Toho",
    [0xAF] = "Namco",
    [0xB0] = "acclaim",
    [0xB1] = "ASCII or Nexsoft",
    [0xB2] = "Bandai",
    [0xB4] = "Square Enix",
    [0xB6] = "HAL Laboratory",
    [0xB7] = "SNK",
    [0xB9] = "Pony Canyon",
    [0xBA] = "Culture Brain",
    [0xBB] = "Sunsoft",
    [0xBD] = "Sony Imagesoft",
    [0xBF] = "Sammy",
    [0xC0] = "Taito",
    [0xC2] = "Kemco",
    [0xC3] = "Squaresoft",
    [0xC4] = "Tokuma Shoten Intermedia",
    [0xC5] = "Data East",
    [0xC6] = "Tonkinhouse",
    [0xC8] = "Koei",
    [0xC9] = "UFL",
    [0xCA] = "Ultra",
    [0xCB] = "Vap",
    [0xCC] = "Use Corporation",
    [0xCD] = "Meldac",
    [0xCE] = ".Pony Canyon or",
    [0xCF] = "Angel",
    [0xD0] = "Taito",
    [0xD1] = "Sofel",
    [0xD2] = "Quest",
    [0xD3] = "Sigma Enterprises",
    [0xD4] = "ASK Kodansha Co.",
    [0xD6] = "Naxat Soft",
    [0xD7] = "Copya System",
    [0xD9] = "Banpresto",
    [0xDA] = "Tomy",
    [0xDB] = "LJN",
    [0xDD] = "NCS",
    [0xDE] = "Human",
    [0xDF] = "Altron",
    [0xE0] = "Jaleco",
    [0xE1] = "Towa Chiki",
    [0xE2] = "Yutaka",
    [0xE3] = "Varie",
    [0xE5] = "Epcoh",
    [0xE7] = "Athena",
    [0xE8] = "Asmik ACE Entertainment",
    [0xE9] = "Natsume",
    [0xEA] = "King Records",
    [0xEB] = "Atlus",
    [0xEC] = "Epic/Sony Records",
    [0xEE] = "IGS",
    [0xF0] = "A Wave",
    [0xF3] = "Extreme Entertainment",
    [0xFF] = "LJN",
};

void print_bytes(void *p, size_t len) {
  for (size_t i = len; i > 0; i--) {
    printf("%02X", ((u8 *)p)[i - 1]);
  }
  printf("\n");
}

size_t read_rom(const char *path, u8 *dst) {
  FILE *file = fopen(path, "rb");

  if (file == NULL) {
    printf("Could not open file: %s\n", path);
    return 0;
  }

  size_t b = fread(dst, 1, 65536, file);

  fclose(file);

  return b;
}

void dump_file(const char *path, u8 *data, size_t length) {
  FILE *f = fopen(path, "wb");
  if (!f) {
    printf("Failed to open file.\n");
  }

  printf("Dumping %zu bytes\n", sizeof(u8) * length);
  fwrite(data, sizeof(u8), length, f);

  fclose(f);
}

int getch(void) {
  int            ch;
  struct termios oldt, newt;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

u8 get_palette_idx(u8 *tile_data, u8 i) {
  assert(i < 64);

  u8 memIdx    = 2 * (i / 8);
  u8 memOffset = 7 - i % 8;

  // 01010101
  // 01010101
  u8 p1 = (tile_data[memIdx] & (1 << memOffset)) >> memOffset;
  u8 p2 = (tile_data[memIdx + 1] & (1 << memOffset)) >> memOffset << 1;

  u8 idx = p1 | p2;

  return idx;
}

EmulationState *emu_init() {
  EmulationState *emu = (EmulationState *)malloc(sizeof(EmulationState));

  emu->mem    = (u8 *)malloc(65536);
  emu->reg    = (u8 *)malloc(12);
  emu->mcycle = 0;

  emu->rom  = emu->mem;
  emu->vram = emu->mem + 0x8000;
  emu->sram = emu->mem + 0xA000;
  emu->wram = emu->mem + 0xC000;
  emu->oam  = emu->mem + 0xFE00;
  emu->io   = emu->mem + 0xFF00;
  emu->hram = emu->mem + 0xFF80;
  emu->ie   = emu->mem + 0xFFFF;

  emu->a  = emu->reg + 1;
  emu->f  = emu->reg + 0;
  emu->af = (u16 *)emu->f;

  emu->b  = emu->reg + 3;
  emu->c  = emu->reg + 2;
  emu->bc = (u16 *)emu->c;

  emu->d  = emu->reg + 5;
  emu->e  = emu->reg + 4;
  emu->de = (u16 *)emu->e;

  emu->h  = emu->reg + 7;
  emu->l  = emu->reg + 6;
  emu->hl = (u16 *)emu->l;

  emu->sp = (u16 *)emu->reg + 8;
  emu->pc = (u16 *)emu->reg + 10;

  emu->header   = (RomHeader *)(emu->rom + 0x100);
  emu->tilemaps = emu->mem + 0x9800;
  emu->sprites  = (SpriteAttribute *)emu->oam;

  return emu;
}

void emu_print(EmulationState *emu) {
  printf("AF: ");
  PRINT_BYTES(*emu->af);

  printf("BC: ");
  PRINT_BYTES(*emu->bc);

  printf("DE: ");
  PRINT_BYTES(*emu->de);

  printf("HL: ");
  PRINT_BYTES(*emu->hl);

  printf("SP: ");
  PRINT_BYTES(*emu->sp);

  printf("PC: ");
  PRINT_BYTES(*emu->pc);
}

void emu_free(EmulationState *emu) {
  free(emu->mem);
  free(emu->reg);
  free(emu);
}

void set_flags(EmulationState *emu, char z, char n, char h, char c) {
  if (z != -1) {
    BIT_SET(*emu->f, 7, z);
  }
  if (n != -1) {
    BIT_SET(*emu->f, 6, n);
  }
  if (h != -1) {
    BIT_SET(*emu->f, 5, h);
  }
  if (c != -1) {
    BIT_SET(*emu->f, 4, c);
  }
}
