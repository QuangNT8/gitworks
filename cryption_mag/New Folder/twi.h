/*************************************************************************************************************
*    Titre:         Fichier d'en-tête du module d'accès au TWI.
*    Fichier:       twi.h
*    Date:          16/11/2005
*************************************************************************************************************/

/************************************************************************************************************
 ** @file twi.h                                                                                            
 ** @brief Fichier d'en-tête du module d'accès au TWI.                                                             
 ************************************************************************************************************/

#ifndef __TWI_H__
#define __TWI_H__

/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
#define TWI_CPU         7372800UL			/* 7.3728Mhz */

#define W 				0					/** Write = 0 */
#define R 				1					/** Read = 1 */
#define OWN_ADR 		60  				/** Adresse de maitre */
#define SUCCESS 		0xFF				/** Succès = 0xFF (-1) */

/*************************************************************************************************************
	Définition des réponses d'états du I2C
*************************************************************************************************************/
/** Codes d'états généraux du maitre */										
#define START		0x08					/** START as bien été transmis */	
#define	REP_START	0x10					/** Repeated START as bien été transmis */ 
		
/** Codes d'états généraux du maitre transmetteur */											
#define	MTX_ADR_ACK		0x18				/** SLA+W as bien été transmis, ACK reçu */	
#define	MTX_ADR_NACK	0x20				/** SLA+W as bien été transmis, NACK reçu */		
#define	MTX_DATA_ACK	0x28				/** Une donnée as bien été transmise, ACK reçu */			
#define	MTX_DATA_NACK	0x30				/** Une donnée as bien été transmise, NACK reçu */		
#define	MTX_ARB_LOST	0x38				/** Séquence d'arbitration perdue pour SLA+W ou pour les données */
	
/** Codes d'états généraux du maitre récepteur */	
#define	MRX_ARB_LOST	0x38				/** Séquence d'arbitration perdue pour SLA+R ou pour le bit NACK */
#define	MRX_ADR_ACK		0x40				/** SLA+R as bien été reçu, ACK reçu */	
#define	MRX_ADR_NACK	0x48				/** SLA+R as bien été reçu, NACK reçu */		
#define	MRX_DATA_ACK	0x50				/** Une donnée as bien été reçue, ACK transmis */
#define	MRX_DATA_NACK	0x58				/** Une donnée as bien été reçue, NACK transmis */

/*************************************************************************************************************
	Définition de la structure de transmission TWI
*************************************************************************************************************/
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

#endif
