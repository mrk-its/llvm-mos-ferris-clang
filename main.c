#include <stdio.h>

extern short ferris_data_len;
extern char ferris_data[10620];

extern char dlist;
extern char dlist1;
extern char dlist_end;

char **DLPTRS = (char **)0x230;
char *SDMCTL = (char *)0x22f;

typedef struct {
    unsigned char colpf0;
    unsigned char colpf1;
    unsigned char colpf2;
    unsigned char colpf3;
    unsigned char colbk;
} color_regs_t;

color_regs_t *color_regs = (color_regs_t *)0x2c4;
char * hscroll = (char *)0xd404;

void set_video_mem(unsigned char offset) {
  *hscroll = 15 - (offset & 3);
  char * video_mem = ferris_data + offset / 4;
  char *ptr = &dlist1 + 1;

  char hi_mem = (int)video_mem / 256;
  char lo_mem0 = (int)video_mem & 0xff;
  char lo_mem1 = lo_mem0 + 64;
  char lo_mem2 = lo_mem1 + 64;
  char lo_mem3 = lo_mem2 + 64;

  while(ptr < &dlist_end) {
    *(ptr++) = lo_mem0;
    *(ptr++) = hi_mem;
    ptr++;
    *(ptr++) = lo_mem1;
    *(ptr++) = hi_mem;
    ptr++;
    *(ptr++) = lo_mem2;
    *(ptr++) = hi_mem;
    ptr++;
    *(ptr++) = lo_mem3;
    *(ptr++) = hi_mem;
    ptr++;

    hi_mem++;
  }
  // for(char *ptr=&dlist1 + 1; ptr < &dlist_end; ptr += 3) {
  //   *((char **)ptr) = video_mem;
  //   video_mem += 64;
  // }
}

void wait_vbl(char delay) {
  volatile char *clock = (volatile char *)20;
  char dest = *clock + delay;
  while(*clock != dest);
}

int main() {
  color_regs->colpf2 = 0xf;
  color_regs->colpf1 = 0x34;  // main color
  color_regs->colpf0 = 0x31;  // shadow

  *SDMCTL = 0x21;
  unsigned char cnt = 0;

  for(;;) {
    int y_offs = cnt & 127;
    if(y_offs>=64) y_offs = 127 - y_offs;
    *((char **)(&dlist_end - 2)) = &dlist + y_offs;
    *DLPTRS = &dlist + y_offs;

    set_video_mem(cnt);
    cnt++;
    wait_vbl(1);
  }

  return 0;
}
