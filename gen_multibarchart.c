/*
    gen_multibarchart - create sample input data for multibarchart program
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

#include <stdint.h>

int main(int argc, char *argv[]) {

  int rnd_fd;

  uint64_t *rnds;

  long int num_rnds;
  long int rndno;

  ssize_t bytes_read;

  long int num_lines;

  long int lineno;
  
  char color_codes[5] = { 'R', 'G', 'B', 'P', 'Y' };
  
  rnd_fd = open("/dev/urandom", O_RDONLY);
  if (rnd_fd == -1) {
    perror("open");
    return -1;
  }
  
  num_rnds = 4;
  rnds = calloc(num_rnds, sizeof(uint64_t));
  if (rnds == NULL) {
    perror("calloc");
    return -1;
  }

  num_lines = 1920;

  for (lineno = 0; lineno < num_lines; lineno++) {

    bytes_read = read(rnd_fd, rnds, sizeof(uint64_t) * num_rnds);
    if (bytes_read != sizeof(uint64_t) * num_rnds) {
      perror("read");
      return -1;
    }

    for (rndno = 0; rndno < num_rnds; rndno++) {

      long int val;
      
      val = 250.0 * rnds[rndno] / 18446744073709551615.0;
      if (rndno > 0) {
	putchar(',');
      }
      printf("%c:%ld", color_codes[rndno], val);
      
    }

    putchar('\n');
    
  }
  
  close(rnd_fd);
  
  free(rnds);
  
  return 0;

}
