/*************************************************************************************************************
*    Titre:         Définitions pour le module d'accès pour eeprom I2c (24lcXXX)
*    Fichier:       i2ceeprom.h
*    Date:          16/11/2005
*************************************************************************************************************/

/************************************************************************************************************
 ** @file 12ceeprom.h                                                                                            
 ** @brief Définitions pour le module d'accès pour eeprom I2c (24lcXXX)                                                             
 ************************************************************************************************************/

#ifndef __I2CEEPROM_H__
#define __I2CEEPROM_H__

/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
#define	I2CEEPROM_ADR	0xA0		/** Adresse esclave du I2CEEPROM */

//#define I2CEEPROM_TOP	0x1FFFF		/** Adresse de fin du eeprom, pour 1x 24lc1024 */
#define I2CEEPROM_TOP	0x3FFFF		/** Adresse de fin du eeprom, pour 2x 24lc1024 */

/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
char I2CEEPROM_write(unsigned long, unsigned int, char*);
char I2CEEPROM_read(unsigned long, unsigned int, char*);

void format_mem(void);

#endif
