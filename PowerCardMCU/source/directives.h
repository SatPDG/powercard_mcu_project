/*
 * directive.h
 *
 *  Created on: Jan. 9, 2021
 *      Author: Leo Clouet
 */

#ifndef DIRECTIVES_H_
#define DIRECTIVES_H_

//#define EVK_1020
#define POWER_CARD_BOARD

#if !defined(EVK_1020) && !defined(POWER_CARD_BOARD)
#error A bord type must be defined.
#endif
#if defined(EVK_1020) && defined(POWER_CARD_BOARD)
#error You cannot defined both board type.
#endif

#endif /* DIRECTIVES_H_ */
