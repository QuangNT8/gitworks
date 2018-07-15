/*************************************************************************************************************
*    Titre:         Fichier d'en-t�te du module d'acc�s au TWI.
*    Fichier:       twi.h
*    Date:          16/11/2005
*************************************************************************************************************/

/************************************************************************************************************
 ** @file twi.h                                                                                            
 ** @brief Fichier d'en-t�te du module d'acc�s au TWI.                                                             
 ************************************************************************************************************/

#ifndef __TWI_H__
#define __TWI_H__

/*************************************************************************************************************
	D�finitions des constantes
*************************************************************************************************************/
#define TWI_CPU         7372800UL			/* 7.3728Mhz */

#define W 				0					/** Write = 0 */
#define R 				1					/** Read = 1 */
#define OWN_ADR 		60  				/** Adresse de maitre */
#define SUCCESS 		0xFF				/** Succ�s = 0xFF (-1) */

/*************************************************************************************************************
	D�finition des r�ponses d'�tats du I2C
*************************************************************************************************************/
/** Codes d'�tats g�n�raux du maitre */										
#define START		0x08					/** START as bien �t� transmis */	
#define	REP_START	0x10					/** Repeated START as bien �t� transmis */ 
		
/** Codes d'�tats g�n�raux du maitre transmetteur */											
#define	MTX_ADR_ACK		0x18				/** SLA+W as bien �t� transmis, ACK re�u */	
#define	MTX_ADR_NACK	0x20				/** SLA+W as bien �t� transmis, NACK re�u */		
#define	MTX_DATA_ACK	0x28				/** Une donn�e as bien �t� transmise, ACK re�u */			
#define	MTX_DATA_NACK	0x30				/** Une donn�e as bien �t� transmise, NACK re�u */		
#define	MTX_ARB_LOST	0x38				/** S�quence d'arbitration perdue pour SLA+W ou pour les donn�es */
	
/** Codes d'�tats g�n�raux du maitre r�cepteur */	
#define	MRX_ARB_LOST	0x38				/** S�quence d'arbitration perdue pour SLA+R ou pour le bit NACK */
#define	MRX_ADR_ACK		0x40				/** SLA+R as bien �t� re�u, ACK re�u */	
#define	MRX_ADR_NACK	0x48				/** SLA+R as bien �t� re�u, NACK re�u */		
#define	MRX_DATA_ACK	0x50				/** Une donn�e as bien �t� re�ue, ACK transmis */
#define	MRX_DATA_NACK	0x58				/** Une donn�e as bien �t� re�ue, NACK transmis */

/*************************************************************************************************************
	D�finition de la structure de transmission TWI
*************************************************************************************************************/
typedef struct
{
	unsigned char slave_adr;				/** Adresse I2C de l'esclave + !W/R */
	unsigned char size;						/** Nombre d'octets � envoyer ou � re�evoir */  	
	unsigned char *data_ptr;				/** Pointeur sur les donn�es et l'adresse interne */
}tx_type; 

/*************************************************************************************************************
	D�finition des fonctions
*************************************************************************************************************/
extern void Init_TWI(unsigned long);			/**	Fonction permettant d'initialiser le TWI.*/
extern unsigned char Send_to_TWI(tx_type*);		/** Envoi d'une s�quence sur le bus TWI. */
unsigned char Send_byte(unsigned char);	/** Envoi d'un octet de donn�es sur le bus TWI. */
unsigned char Get_byte(unsigned char*,char);	/** R�ception d'un octet de donn�es sur le bus TWI. */
unsigned char Send_start(void);					/** Envoi la commande START sur le bus TWI. */
unsigned char Send_adr(unsigned char);			/** Envoi de l'adresse de l'esclave sur le bus TWI. */
void Send_stop(void);							/** Envoi la commande STOP sur le bus TWI. */
void Wait_TWI_int(void);						/**	Attends le drapeau d'interruption du TWI. */								

#endif
