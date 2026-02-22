/*
 * ProMode.h
 *
 *  Created on: 6 Jan 2026
 *      Author: pawel
 */

#ifndef PROMODE_H_
#define PROMODE_H_

#include "main.h"

#define NUMBERCONFIGPARAMETER 10
#define NUMBERRECEIVEDCONFIGPARAMETER 11 //Odpowiada ilość przysłanych paraemtrów
#define NUMBERKINDOFCONFIGPARAMETER 10 //Odpowiada liczbie rodzajów przysłanych parametrów
#define MAXPROGATTEMPTS 3



void Programming_Mode(flashFlag_t* programmingFlag);

#endif /* PROMODE_H_ */
