/*************************************************************************************************************
*    Titre:         Module d'acc�s au TWI (en polling).
*    Fichier:       twi.c
*    Date:          16/11/2005
*************************************************************************************************************/

#include <avr/io.h>

#include "twi.h"															

/**************************************************************************************************************
 ** @file twi.c
 ** @brief	Module d'acc�s au TWI.
 **************************************************************************************************************/
 
/*************************************************************************************************************
 	Fonction :		Init_TWI  
 	Description :	Initialisation du TWI.  
 	Param�tres: 	Vitesse de tansmission en Hz
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Fonction permettant d'initialiser le TWI.
**************************************************************************************************************/
void Init_TWI(unsigned long twi_freq)
{
	TWAR = OWN_ADR;								/** Assigne sa propre adresse */
	TWSR = 0;									/** Prescaler = 1 */							 
	TWBR = ((TWI_CPU / twi_freq - 16) / 2); /* Ajuste la vitesse du BUS TWI */
//	TWCR = (1<<TWEN);							/** D�marre l'interface TWI */
}    

/*************************************************************************************************************
 	Fonction :		Wait_TWI_int 
 	Description :	Attends le drapeau d'interruption du TWI. (mode polling)  
 	Param�tres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Attends le drapeau d'interruption du TWI. (mode polling)
**************************************************************************************************************/
void Wait_TWI_int(void)
{
	while (!(TWCR & (1<<TWINT))); 
}    

/*************************************************************************************************************
 	Fonction :		Send_start 
 	Description :	Envoi la commande de d�mmarage de s�quence sur le bus TWI et attends la r�ponse.  
 	Param�tres: 	aucun
 	Valeur retour:	�tat du bus TWI
**************************************************************************************************************/
/** @brief	Envoi la commande de d�mmarage de s�quence sur le bus TWI et attends la r�ponse.
**************************************************************************************************************/
unsigned char	Send_start(void)
{
	TWCR = ((1<<TWINT)|(1<<TWSTA)|(1<<TWEN));	/** Envoi de la commande START */
	
	Wait_TWI_int();								/** Attends le drapeau d'interruption TWI */

    if(((TWSR & 0xF8) != START)&&((TWSR & 0xF8) != REP_START))
		return TWSR;						
	return SUCCESS;								/** Retourne l'�tat du bus TWI */    										
}
							
/*************************************************************************************************************
 	Fonction :		Send_stop 
 	Description :	Envoi la commande d'arr�t de s�quence sur le bus TWI. 
 	Param�tres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Envoi la commande d'arr�t de s�quence sur le bus TWI.
**************************************************************************************************************/
void Send_stop(void)
{
	TWCR = ((1<<TWEN)|(1<<TWINT)|(1<<TWSTO));	/** Envoi de la commande STOP */
}        

/*************************************************************************************************************
 	Fonction :		Send_to_TWI 
 	Description :	Envoi d'une s�quence sur le bus TWI.
 	Param�tres: 	Structure de s�quence tx_type comprenant adresse I2C, commande et donn�es.
 	Valeur retour:	�tat du bus TWI
**************************************************************************************************************/
/** @brief	Envoi d'une s�quence sur le bus TWI.
**************************************************************************************************************/    
unsigned char Send_to_TWI(tx_type *data_pack)
{
	unsigned char state,i,j;

	state = SUCCESS;
	
	for(i=0;(data_pack[i].slave_adr != OWN_ADR)&&(state == SUCCESS);i++)
	{
		state = Send_start();								/** Envoi de la commande START */
		if (state == SUCCESS)				
			state = Send_adr(data_pack[i].slave_adr);		/** Envoi de l'adresse esclave I2C + !W/R */
		
		/** Envoi ou recoit des donn�es, selon !W/R fourni � la fonction */
		if(!(data_pack[i].slave_adr & R))					/** Si on d�sire �crire vers l'esclave... */
		{
			 /** Transmission des donn�es en boucle */
			for(j=0;((j<data_pack[i].size)&&(state == SUCCESS));j++)
				state = Send_byte(data_pack[i].data_ptr[j]);
		}	
		else												/** Sinon, on d�sire lire de l'esclave... */ 
		{
			 /** R�ception des donn�es en boucle */
			for(j=0;((j<data_pack[i].size)&&(state == SUCCESS));j++)
				state = Get_byte(data_pack[i].data_ptr++,data_pack[i].size-j-1);
		}					
	}
  	Send_stop();											/** Envoi de la commande STOP */
  	  
	return state;											/** Retourne l'�tat du bus TWI */  
}

/*************************************************************************************************************
 	Fonction :		Send_byte
 	Description :	Envoi d'un octet de donn�es sur le bus TWI.
 	Param�tres: 	Donn�e � transmettre.
 	Valeur retour:	�tat du bus TWI
**************************************************************************************************************/
/** @brief	Envoi d'un octet de donn�es sur le bus TWI.
**************************************************************************************************************/
unsigned char Send_byte(unsigned char data)
{
	Wait_TWI_int();							/** Attends le drapeau d'interruption TWI */

	TWDR = data;
 	TWCR = ((1<<TWINT)|(1<<TWEN));   		/** Descends le drapeau pour envoyer la donn�e */ 

	Wait_TWI_int();							/** Attends le drapeau d'interruption TWI */

	if((TWSR & 0xF8) != MTX_DATA_ACK)				
		return TWSR;																
	return SUCCESS;							/** Retourne l'�tat du bus TWI */
}	

/*************************************************************************************************************
 	Fonction :		Send_adr
 	Description :	Envoi de l'adresse de l'esclave sur le bus TWI.
 	Param�tres: 	Adresse de l'esclave.
 	Valeur retour:	�tat du bus TWI
**************************************************************************************************************/
/** @brief	Envoi de l'adresse de l'esclave sur le bus TWI.
**************************************************************************************************************/
unsigned char Send_adr(unsigned char adr)
{
	Wait_TWI_int();							/** Attends le drapeau d'interruption TWI */

	TWDR = adr;
	TWCR = ((1<<TWINT)|(1<<TWEN));   		/** Descends le drapeau pour envoyer l'adresse */  

	Wait_TWI_int();							/** Attends le drapeau d'interruption TWI */

	if(((TWSR & 0xF8) != MTX_ADR_ACK)&&((TWSR & 0xF8) != MRX_ADR_ACK))
		return TWSR;
	return SUCCESS;							/** Retourne l'�tat du bus TWI */
}	

/*************************************************************************************************************
 	Fonction :		Get_byte
 	Description :	R�ception d'un octet de donn�es sur le bus TWI.
 	Param�tres: 	Pointeur de caractere pour la donn�e � recevoir, drapeau de fin de transmission
 	Valeur retour:	�tat du bus TWI
**************************************************************************************************************/
/** @brief	R�ception d'un octet de donn�es sur le bus TWI.
**************************************************************************************************************/
unsigned char Get_byte(unsigned char *rx_ptr,char last_byte)
{
	Wait_TWI_int();							/** Attends le drapeau d'interruption TWI */

	if(last_byte)							/** Si ce n'est pas la derni�re donn�e � recevoir... */	
		TWCR = ((1<<TWINT)|(1<<TWEA)|(1<<TWEN));	/** Descends le drapeau pour recevoir la donn�e + ACK */	
	else									/** Si c'est la derni�re donn�e � recevoir... */
		TWCR = ((1<<TWINT)|(1<<TWEN)); 			/** Descends le drapeau pour recevoir la donn�e sans ACK */	
	Wait_TWI_int();							/** Attends le drapeau d'interruption TWI */

	*rx_ptr = TWDR;							/** �cris la donn�e re�ue au pointeur */
	
 	if((((TWSR & 0xF8) == MRX_DATA_NACK)&&(last_byte == 0))||((TWSR & 0xF8) == MRX_DATA_ACK))
		return SUCCESS;	  
	return TWSR;							/** Retourne l'�tat du bus TWI */
}

