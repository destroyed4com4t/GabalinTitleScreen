
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
//#link "music_dangerstreets.s"
extern char gabalin_music_data[];
//#link "demosounds.s"
extern char demo_sounds[];

int i;       //for loops
char oam_id; //for sprites
bool start_pressed = false; // Only allows one input from Start Button at a time.
int sfx_timer; //pauses music to play sound


const char ATTRIBUTE_TABLE_1[0x40] = {
  0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 0-3
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 4-7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 8-11
  0xAA, 0xAA, 0xAA, 0xAA, 0xEA, 0xFA, 0xAA, 0xAA, // rows 12-15
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // rows 16-19
  0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, // rows 20-23
  0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, // rows 24-27
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // rows 28-29
                                     };//Pallette Attributes

const char PALETTE[32] = {
    0x0f, // screen color
          0x16, 0x28, 0x30, // background palette 0
    0x0f, 0x24, 0x1A, 0x37, // background palette 1
    0x0f, 0x13, 0x22, 0x31, // background palette 2
    0x0f, 0x16, 0x22, 0x31, // background palette 3

    0x0f, 0x0F, 0x24, 0x37, // sprite palette 0
    0x0f, 0x32, 0x12, 0x37, // sprite palette 1
    0x0f, 0x0F, 0x24, 0x30, // sprite palette 2
    0x0f, 0x00, 0x10, 0x30  // sprite palette 3
                         }; //Screen Pallette

const char TITLE_SCREEN[960] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 
    0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 
    0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 
    0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x00, 0x00, 0x0D, 0x0E, 0x0F, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 
    0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x00, 0x00, 0x1D, 0x1E, 0x1F, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 
    0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0x00, 0x00, 0x2D, 0x2E, 0x2F, 0x3F, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0xB1, 0xB2, 0x00, 0x5B, 0x5C, 0x5D, 
    0x5E, 0x5F, 0x6E, 0x6F, 0xBB, 0xBC, 0xBD, 0x00, 0x00, 0x00, 0x3E, 0x3F, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7F, 0x8E, 
    0x8F, 0x9E, 0x9F, 0xAE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB3, 0xB4, 
    0xB5, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 
    0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 
    0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 
    0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0xF4, 
    0x00, 0xF6, 0xF7, 0xF8, 0xF9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCE, 0xCF, 
    0xED, 0xEE, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDD, 0xDE, 0xDF, 
    0xFD, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x52, 0x45, 0x53, 0x53, 0x00, 
    0x00, 0x53, 0x54, 0x41, 0x52, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x21, 0x22, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x05, 0x01, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0x09, 0x0A, 0x0B, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x14, 0x15, 0x01, 
    0x01, 0x16, 0x17, 0x01, 0x01, 0x01, 0x01, 0x18, 0x19, 0x1A, 0x1B, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x24, 0x25, 0x01, 
    0x01, 0x26, 0x27, 0x01, 0x01, 0x01, 0x01, 0x28, 0x29, 0x2A, 0x2B, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x30, 0x32, 0x33, 0x00, 0x00, 0x52, 0x41, 0x4E, 
    0x49, 0x00, 0x54, 0x49, 0x4D, 0x45, 0x4B, 0x45, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                                     };//Pallette Attributes


void draw_background()
{
  ppu_off();
  pal_all(PALETTE);
  vram_adr(0x23c0);
  vram_write(ATTRIBUTE_TABLE_1, sizeof(ATTRIBUTE_TABLE_1));
  for (i = 0; i < sizeof(TITLE_SCREEN); i++)
    { //Draw_Cityscape
    vram_put(TITLE_SCREEN[i]);
    }
  oam_id=oam_spr(136, 191, 235, 1,oam_id);
  oam_id=oam_spr(144, 191, 236, 1,oam_id);
  oam_id=oam_spr(136, 199, 251, 1,oam_id);
  oam_id=oam_spr(144, 199, 252, 1,oam_id);
  oam_id=oam_spr(192, 183, 220, 2,oam_id);
  ppu_on_all();
}

void main(void)
{
  draw_background();
  vram_adr(NTADR_A(2,2));
    // initialize music system
  //famitone_init(after_the_rain_music_data);
  famitone_init(gabalin_music_data);
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
    if (pad & PAD_A) 
    	{
      	// sfx_play(0,0); // commenting out sfx for now
    	}
    if (pad & PAD_B) 
    	{
      	// sfx_play(1,1); // commenting out sfx for now
    	}
    if (pad & PAD_LEFT) 
    	{
      	//sfx_play(2,2); // commenting out sfx for now
    	}
    if (pad & PAD_RIGHT) 
    	{
      	//sfx_play(3,3); // commenting out sfx for now
    	}
    if (pad & PAD_START) 
    	{
        if (!start_pressed)
    	    {
      	    sfx_timer=3500;
            music_pause(1);
            sfx_play(0,0); // Start Button Noise
    	    start_pressed = true;
            }
    	}
    else
  	{
    	start_pressed = false;
  	}
   if (sfx_timer)
        {
        music_pause(1);
        sfx_timer--;
        }
   else
        {
        music_pause(0);
        }  

  }
}
