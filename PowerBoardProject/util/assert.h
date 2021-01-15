/*
 * assert.h
 *
 *  Created on: Jan. 9, 2021
 *      Author: Leo Clouet
 */

#ifndef ASSERT_H_
#define ASSERT_H_

#ifdef DEBUG
#define PB_ASSERT(x) {if(!(x)) __disable_irq(); while(1);}
#else
#error not def yet
//#define PB_ASSERT(x) {if(!(x)) __disable_irq(); }
#endif

#endif /* ASSERT_H_ */
