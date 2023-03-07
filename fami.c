
/*
Demonstrates the Famitone2 library for sound and music.
Press controller buttons to hear sound effects.
*/

#include "neslib.h"

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// setup Famitone library

//#link "famitone2.s"
void __fastcall__ famitone_update(void);
//#link "music_aftertherain.s"
extern char after_the_rain_music_data[];
//#link "music_dangerstreets.s"
extern char danger_streets_music_data[];
//#link "demosounds.s"
extern char demo_sounds[];
int i;

void draw_background()
{
  ppu_off();
  vram_adr(0x23c0);
  vram_adr(NTADR_A(0, 0));
  vram_fill(0, 32 * 22);
  vram_adr(NTADR_A(0, 22));
  vram_fill(2, 32 * 6);
  vram_adr(NTADR_A(2, 1));
  vram_fill(5, 29 * 1);
  vram_adr(NTADR_A(2, 2));
  vram_fill(6, 28 * 1);
  vram_adr(NTADR_A(6, 13));
  vram_fill(10, 20 * 1);
  vram_adr(NTADR_A(6, 14));
  vram_fill(10, 20 * 1);
  vram_adr(NTADR_A(6, 15));
  vram_fill(10, 20 * 1);
  vram_adr(NTADR_A(5, 16));
  vram_fill(13, 22 * 1);
  vram_adr(NTADR_A(4, 17));
  vram_fill(13, 24 * 1);
  vram_adr(NTADR_A(3, 18));
  vram_fill(13, 26 * 1);
  vram_adr(NTADR_A(2, 19));
  vram_fill(12, 28 * 1);
  vram_adr(NTADR_A(2, 20));
  vram_fill(5, 29 * 1);
  vram_adr(NTADR_A(2, 2));
  vram_put(28);
  vram_adr(NTADR_A(29, 2));
  vram_put(29);
  vram_adr(NTADR_A(2, 19));
  vram_put(30);
  vram_adr(NTADR_A(29, 19));
  vram_put(31);
  vram_adr(NTADR_A(3, 3));
  vram_put(26);
  vram_adr(NTADR_A(28, 3));
  vram_put(27);
  vram_adr(NTADR_A(3, 18));
  vram_put(16);
  vram_adr(NTADR_A(28, 18));
  vram_put(17);
  vram_adr(NTADR_A(4, 4));
  vram_put(24);
  vram_adr(NTADR_A(27, 4));
  vram_put(25);
  vram_adr(NTADR_A(4, 17));
  vram_put(18);
  vram_adr(NTADR_A(27, 17));
  vram_put(19);
  vram_adr(NTADR_A(5, 5));
  vram_put(22);
  vram_adr(NTADR_A(26, 5));
  vram_put(23);
  vram_adr(NTADR_A(5, 16));
  vram_put(20);
  vram_adr(NTADR_A(26, 16));
  vram_put(21);
  for (i = 0; i < 20; i++)
    { //Draw_Left Side Outer Box
    vram_adr(NTADR_A(1, 1 + i));
    vram_fill(5, 1 * 1);
    }
  for (i = 0; i < 16; i++)
    { //Draw_Left Side Inner Box
    vram_adr(NTADR_A(2, 3 + i));
    vram_fill(6, 1 * 1);
    }
  for (i = 0; i < 10; i++)
    { //Draw_Left Side Wall
    vram_adr(NTADR_A(5, 6 + i));
    vram_fill(59, 1 * 1);
    }
  for (i = 0; i < 20; i++)
    { //Draw_Right Side
    vram_adr(NTADR_A(30, 1 + i));
    vram_fill(5, 1 * 1);
    }
  for (i = 0; i < 16; i++)
    { //Draw_Right Side
    vram_adr(NTADR_A(29, 3 + i));
    vram_fill(12, 1 * 1);
    }
  for (i = 0; i < 10; i++)
    { //Draw_Right Side Wall
    vram_adr(NTADR_A(26, 6 + i));
    vram_fill(94, 1 * 1);
    }
  for (i = 0; i < 6; i++)
    { //Draw_Left Side Outer Box
    vram_adr(NTADR_A(0, 22 + i));
    vram_fill(5, 1 * 1);
    }
  vram_adr(NTADR_A(0, 22));
  vram_fill(5, 32 * 1);
  vram_adr(NTADR_A(0, 27));
  vram_fill(5, 32 * 1);
  vram_adr(NTADR_A(0, 28));
  vram_fill(0, 32 * 1);
  for (i = 0; i < 6; i++)
    { //Draw_Left Side Outer Box
    vram_adr(NTADR_A(31, 22 + i));
    vram_fill(5, 1 * 1);
    }
  ppu_on_bg();
}

void main(void)
{
  pal_col(1,0x04);
  pal_col(2,0x20);
  pal_col(3,0x30);
  draw_background();
  vram_adr(NTADR_A(2,2));
  vram_write("FAMITONE2 DEMO TESTING", 20);
  // initialize music system
  //famitone_init(after_the_rain_music_data);
  famitone_init(danger_streets_music_data);
  sfx_init(demo_sounds);
  // set music callback function for NMI
  nmi_set_callback(famitone_update);
  // play music
  music_play(0);
  //enable rendering
  ppu_on_all();
  // repeat forever
  while(1) {
    // poll controller 0
    char pad = pad_poll(0);
    // play sounds when buttons pushed
    if (pad & PAD_A) {
      sfx_play(0,0);
    }
    if (pad & PAD_B) {
      sfx_play(1,1);
    }
    if (pad & PAD_LEFT) {
      sfx_play(2,2);
    }
    if (pad & PAD_RIGHT) {
      sfx_play(3,3);
    }
  }
}
