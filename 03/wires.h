#ifndef _COMBINED_H_
#define _COMBINED_H_

#include <stdint.h>

#define BUFSIZE 2000000
#define X_ORIGIN 0
#define Y_ORIGIN 1000

uint_fast32_t update_min(uint_fast32_t current, uint_fast8_t ind);
uint_fast32_t manhattan_dist(uint_fast32_t x, uint_fast32_t y, uint_fast32_t x_o, uint_fast32_t y_o);


#endif /* _COMBINED_H_ */
