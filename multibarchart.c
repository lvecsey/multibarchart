/*
    multibarchart - create a bar graph of sorts based on input data
    Copyright (C) 2020  Lester Vecsey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>

#include <stdint.h>

#include "readfile.h"
#include "writefile.h"

typedef struct {
  uint16_t r, g, b;
} pixel_t;
  
typedef struct {

  char color_code;

  long int ycount;
  
} pixevent;

int main(int argc, char *argv[]) {

  long int xres, yres;

  long int num_pixels;
  size_t img_sz;
  
  FILE *fp;
  size_t len;
  char *line;
  
  long int xpos, ypos;

  pixel_t blue = { 0, 0, 65535 };

  pixel_t dark_red = { 32768, 0, 0 };
  
  pixel_t red = { 65535, 0, 0 };
  pixel_t purple = { 32768, 0, 32768 };
  pixel_t green = { 0, 65535, 0 };
  
  pixel_t fill_color;

  long int yi, yh;
  
  pixevent *pixevents;

  long int num_pixevents;
  
  long int pixeventno;
  
  int retval;

  long int lineno;

  pixel_t *rgb;
  
  xres = 1920;
  yres = 1080;

  num_pixels = xres * yres;
  img_sz = num_pixels * sizeof(pixel_t);
  rgb = malloc(img_sz);
  if (rgb == NULL) {
    perror("malloc");
    return -1;
  }

  num_pixevents = argc>1 ? strtol(argv[1],NULL,10) : 1000;
  
  pixevents = calloc(num_pixevents, sizeof(pixevent));
  if (pixevents == NULL) {
    perror("calloc");
    return -1;
  }
  
  fp = stdin;
  line = NULL;
  len = 0;

  xpos = 0;

  lineno = 0;
  
  while ((retval = getline(&line,&len,fp)) != -1) {

    pixevent curpix;
    
    if (len > 0) {

      char buf[80];
      
      char *str;
      char *token;

      memset(buf, 0, sizeof(buf));
      strncpy(buf, line, sizeof(buf));
      
      pixeventno = 0;

      ypos = (yres - 1);

      str = buf;

      for (;;) {

	token = strtok(str, ",");

	if (token == NULL) {
	  break;
	}

	retval = sscanf(token, "%c:%ld", & (curpix.color_code), & (curpix.ycount));

	if (retval != 2) {
	  fprintf(stderr, "Error in data format.\n");
	  fprintf(stderr, "token %s\n", token);
	  return -1;
	}

	str = NULL;
	
	{
	
	  switch (curpix.color_code) {
	  case 'R':
	  case 'r': fill_color = red; break;
	  case 'G':
	  case 'g': fill_color = green; break;
	  case 'B':
	  case 'b': fill_color = blue;
	  case 'P':
	  case 'p': fill_color = purple;
	  default: fill_color = dark_red; break;
	  }

	  for (yi = 0, yh = curpix.ycount; yi < yh; yi++) {

	    rgb[ (ypos-yi) * xres + xpos] = fill_color;	
	
	  }

	  ypos -= yh;
	}
	
	pixevents[pixeventno] = curpix;

	pixeventno++;
	
      }

      lineno++;
      
      xpos++;

      if (xpos == xres) {
	fprintf(stderr, "%s: Reached end.\n", __FUNCTION__);
	break;
      }
      
    }
    
  }
    
  free(pixevents);
  
  {
    int out_fd;

    ssize_t bytes_written;

    out_fd = 1;
  
    bytes_written = writefile(out_fd, rgb, img_sz);
    if (bytes_written != img_sz) {
      perror("write");
      return -1;
    }

  }

  free(rgb);
  
  return 0;

}
