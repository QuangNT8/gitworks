/*************************************************************************************************************
*    Titre:         Définitions du module d'encryption AES.
*    Fichier:       aes.h
*    Date:          07/02/2007
*************************************************************************************************************/

/************************************************************************************************************
 ** @file aes.h                                                                                            
 ** @brief Définitions du module d'encryption AES.                                                           
 ************************************************************************************************************/

#ifndef __AES_H__
#define __AES_H__
/*************************************************************************************************************
	Définition des constantes
*************************************************************************************************************/
#define CRYPTO_KEY_SIZE	32

/*************************************************************************************************************
	Définition des variables globales
*************************************************************************************************************/
char crypto_key[CRYPTO_KEY_SIZE+1];

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

#endif
