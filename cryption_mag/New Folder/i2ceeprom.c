/*************************************************************************************************************
*    Titre:         Module d'acc�s pour eeprom I2c (24lcXXX)
*    Fichier:       i2ceeprom.c
*    Date:          16/11/2005
*************************************************************************************************************/

#include <avr/io.h>

#include "i2ceeprom.h"
#include "twi.h"
#include "smalltools.h"

/**************************************************************************************************************
 ** @file i2ceeprom.c
 ** @brief	Module d'acc�s pour eeprom I2c (24lcXXX)
 **************************************************************************************************************/														

/*************************************************************************************************************
 	Fonction :		I2CEEPROM_write  
 	Description :	�criture sequentielle sur le EEPROM � partir d'un tampon.
 	Param�tres: 	Adresse interne du EEPROM de d�part, longueur � �crire, tampon
 	Valeur retour:	�tat du TWI.
**************************************************************************************************************/
/** @brief	�criture sequentielle sur le EEPROM � partir d'un tampon.
**************************************************************************************************************/
char I2CEEPROM_write(unsigned long adr, unsigned int len, char *buf)
{
	unsigned int i;
	char temp[3];
	long_char eeadr;

	tx_type tx_frame[2];

	if((adr+len)>=I2CEEPROM_TOP) /** �criture d�passe le I2CEEPROM, erreur */
	{
		return(0);
	}

	for(i=0;i<len;i++)
	{	
		eeadr.var_long=adr+(unsigned long)i;

		tx_frame[0].slave_adr = (I2CEEPROM_ADR+W)|((eeadr.var_char[2]&0x03)<<1);	/** Adresse du EEPROM, en �criture */
		tx_frame[0].size = 3;						/** Nombre de donn�es � transmettre */
		tx_frame[0].data_ptr = (unsigned char*)temp;		/** Fait pointer le pointeur sur le tampon temporaire */
		tx_frame[0].data_ptr[0] = eeadr.var_char[1];	 	/** Partie haute de l'adresse */
		tx_frame[0].data_ptr[1] = eeadr.var_char[0]; 		/** Partie basse de l'adresse */
		tx_frame[0].data_ptr[2] = *(buf+i);	 		/** Donn�e */
	
		tx_frame[1].slave_adr = OWN_ADR;				/** Fin de la trame TWI */

		while(Send_to_TWI(tx_frame)!=SUCCESS);			/** Appelle la fonction d'envoi sur le TWI */
	}
	
	return(SUCCESS);
}

/*************************************************************************************************************
 	Fonction :		I2CEEPROM_read  
 	Description :	Lecture sequentielle du EEPROM dans un tampon.
 	Param�tres: 	Adresse interne du EEPROM de d�part, longueur � lire, tampon
 	Valeur retour:	�tat du TWI.
**************************************************************************************************************/
/** @brief	Lecture sequentielle du EEPROM dans un tampon.
**************************************************************************************************************/
char I2CEEPROM_read(unsigned long adr, unsigned int len, char *buf)
{
	char temp0[2];
	long_char eeadr;
	
	eeadr.var_long=adr;
	
	tx_type tx_frame[3];

	if((adr+len)>=I2CEEPROM_TOP) /** Lecture d�passe le I2CEEPROM, erreur */
	{
		return(0);
	}

	tx_frame[0].slave_adr = (I2CEEPROM_ADR+W)|((eeadr.var_char[2]&0x03)<<1);	/** Adresse du EEPROM, en �criture */
	tx_frame[0].size = 2;						/** Nombre de donn�es � transmettre */
	tx_frame[0].data_ptr = (unsigned char*)temp0;		/** Fait pointer le pointeur sur le tampon temporaire */
	tx_frame[0].data_ptr[0] = eeadr.var_char[1];	 	/** Partie haute de l'adresse */
	tx_frame[0].data_ptr[1] = eeadr.var_char[0]; 		/** Partie basse de l'adresse */
	
	
	tx_frame[1].slave_adr = (I2CEEPROM_ADR+R)|((eeadr.var_char[2]&0x03)<<1);	/** D�marre une lecture maitre */
	tx_frame[1].size = len;						/** Nombre de donn�es � lires */
	tx_frame[1].data_ptr = (unsigned char*)buf;	/** Fait pointer le pointeur sur le tampon de r�ception */

	tx_frame[2].slave_adr = OWN_ADR;			/** Fin de la trame TWI */

	while(Send_to_TWI(tx_frame)!=SUCCESS);		/** Appelle la fonction d'envoi sur le TWI */
	
	return(SUCCESS);
}

/*************************************************************************************************************
 	Fonction :		format_mem 
 	Description :	Remise � z�ro de la m�moire EEPROM.
 	Param�tres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Remise � z�ro de la m�moire EEPROM.
**************************************************************************************************************/
void format_mem(void)
{
	char temp[0x81];
	unsigned char i;
	unsigned long adr;

	long_char eeadr;

	tx_type tx_frame[2];

	tx_frame[0].slave_adr = (I2CEEPROM_ADR+W)|((eeadr.var_char[2]&0x03)<<1);	/** Adresse du EEPROM, en �criture */
	tx_frame[0].size = 0x81;						/** Nombre de donn�es � transmettre */
	tx_frame[0].data_ptr = (unsigned char*)temp;		/** Fait pointer le pointeur sur le tampon temporaire */
	for(i=0;i<0x7F;i++)
		tx_frame[0].data_ptr[i+2] = 0xFF;			/** Passage des 0xFF */
	
	tx_frame[1].slave_adr = OWN_ADR;				/** Fin de la trame TWI */

	for(adr=0;adr<I2CEEPROM_TOP;adr+=0x7F)
	{
		eeadr.var_long=adr;
		tx_frame[0].data_ptr[0] = eeadr.var_char[1];	 	/** Partie haute de l'adresse */
		tx_frame[0].data_ptr[1] = eeadr.var_char[0]; 		/** Partie basse de l'adresse */

		while(Send_to_TWI(tx_frame)!=SUCCESS);			/** Appelle la fonction d'envoi sur le TWI */
	}
}
