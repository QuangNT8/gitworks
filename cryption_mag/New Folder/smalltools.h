/*************************************************************************************************************
*    Fichier:   smalltools.h               
*    But:       Fichier d'en-tête du fichier smalltools.c
*    Date:      16/1/2007
*************************************************************************************************************/

/************************************************************************************************************
 ** @file smalltools.h
 ** @brief Fichier d'en-tête du fichier smalltools.c
 ************************************************************************************************************/
 
#ifndef __SMALLTOOLS_H__
#define __SMALLTOOLS_H__

/*************************************************************************************************************
	Unions et structures
*************************************************************************************************************/
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

#endif
