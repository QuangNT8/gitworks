/*************************************************************************************************************
*    Titre:         Module d'acc�s pour eeprom interne
*    Fichier:       eeprom.c
*    Date:          06/02/2007 modifi� pour atmega 8 29/07/2007
*************************************************************************************************************/

#include <avr/io.h>

#include "eeprom.h"

/**************************************************************************************************************
 ** @file eeprom.c
 ** @brief	Module d'acc�s pour eeprom interne
 **************************************************************************************************************/														

/*************************************************************************************************************
 	Fonction :		EEPROM_write  
 	Description :	�criture sequentielle sur le EEPROM interne � partir d'un tampon.
 	Param�tres: 	Adresse de d�part sur le EEPROM, longueur � �crire, tampon
 	Valeur retour:	Aucune.
**************************************************************************************************************/
/** @brief	�criture sequentielle sur le EEPROM � partir d'un tampon.
**************************************************************************************************************/
void EEPROM_write(unsigned int adr, unsigned int len, char *buf)
{
	unsigned int i;
	
	/** Attends si une �criture est en cours sur le eeprom */
	while(EECR & (1<<EEWE));
	
	/** Envoie la chaine de caract�res en m�moire eeprom */
	for(i=0;i<len;i++)
	{
		/** Place l'adresse et le data � mettre en EEPROM */
		EEAR = adr+i;
		EEDR = buf[i];
		
		/** D�marre l'�criture */
		EECR |= (1<<EEMWE);
		EECR |= (1<<EEWE);
		
		/** Attends si une �criture est en cours sur le eeprom */
		while(EECR & (1<<EEWE));	
	}
}

/*************************************************************************************************************
 	Fonction :		EEPROM_read  
 	Description :	Lecture sequentielle du EEPROM dans un tampon.
 	Param�tres: 	Adresse interne du EEPROM de d�part, longueur � lire, tampon
 	Valeur retour:	Aucune.
**************************************************************************************************************/
/** @brief	Lecture sequentielle du EEPROM dans un tampon.
**************************************************************************************************************/
void EEPROM_read(unsigned int adr, unsigned int len, char *buf)
{
	unsigned int i;
	
	/** Attends si une �criture est en cours sur le eeprom */
	while(EECR & (1<<EEWE));
	
	/** Lis la chaine de caract�res en m�moire eeprom */
	for(i=0;i<len;i++)
	{
		/** Place l'adresse � lire en EEPROM */
		EEAR = adr+i;
		
		/** D�marre la lecture */
		EECR |= (1<<EERE);
		
		/** Place le caractere dans la chaine */
		buf[i]=EEDR;
	}
}
