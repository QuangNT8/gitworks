/*************************************************************************************************************
 *    Titre	       : Fichier pour la définition des variables à stocker en eeprom.
 *    Fichier      : eepvars.h
 **************************************************************************************************************/

/**************************************************************************************************************
 ** @file	eepvars.h
 ** @brief	Fichier pour la définition des variables à stocker en eeprom.
 **************************************************************************************************************/
 
#ifndef __EEPVARS_H__
#define __EEPVARS_H__

#include "main.h"
#include "aes.h"

/*************************************************************************************************************
	Variables dans le EEPROM, propre à la version
**************************************************************************************************************/
typedef struct 
{
	char E_memtest; 					
	char E_conname[9];
	char E_enkey[3];				
	char E_passl1[26];
	char E_passl2[26];
	char E_crypt[CRYPTO_KEY_SIZE+1]; 
	char E_version[33];
	char E_key[16];
	char E_end;

} eepvars_t; 

eepvars_t EEPVars __attribute__((section(".eeprom"))) = {	'!',		// memtest
															CONNAME,	// conname
															ENKEY,		// enkey
															PASSL1,		// passl1
															PASSL2,		// passl2
															CRYPT,		// crypt
															VERSION,	// version
															{KEY0,KEY1,KEY2,KEY3,KEY4,KEY5,KEY6,KEY7,KEY8,KEY9,KEYA,KEYB,KEYC,KEYD,KEYE,KEYF},	//key mapping
															END,		// type de fin
														};
#endif
