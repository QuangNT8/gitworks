/*************************************************************************************************************
*    Fichier:   usart.c              
*    But:       Module de communication sur le usart utilisant des tampons en interruption.
*    Date:      6/11/2005 modifié pour atmega 8 29/07/2007
*************************************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "usart.h"

/**************************************************************************************************************
 ** @file usart.c
 ** @brief Module de communication sur le usart.
 **************************************************************************************************************/

volatile unsigned char tampon_tx[NB_TAMPON_TX];
volatile unsigned char tampon_rx[NB_TAMPON_RX];

volatile unsigned char tx_in;
volatile unsigned char tx_out;

volatile unsigned char rx_in;
volatile unsigned char rx_out;

FILE USART_str = FDEV_SETUP_STREAM(USART_putchar, USART_getchar, _FDEV_SETUP_RW);

/*************************************************************************************************************
 	Fonction :		USART_Init  
 	Description :	Initialisation du USART.  
 	Paramètres: 	Vitesse de tansmission en bauds
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Fonction permettant d'initialiser le USART.
**************************************************************************************************************/
void USART_init(unsigned long bauds) 
{
	unsigned int ubrr;
	
	/** Calcul des valeurs à appliquer à UBRR afin d'atteindre le nombre de bauds voulues */
	ubrr=(unsigned short)((F_CPU / (bauds * 8l)) - 1);
    
    /** Application des valeurs aux registres d'UBRR */
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;

	/** Double vitesse */
    UCSRA = (1<<U2X);
	/** TX0 activé, RX0 activé ainsi que l'interruption sur RX0 */
    UCSRB = ((1<<RXCIE)|(1<<RXEN)|(1<<TXEN));
    /** 8 bits, 1 stop bit, utilisation de UBRR */
    UCSRC = ((1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL));

	/** Accrochage des fonction USART aux commandes d'STDIO.H */
	stdout = stdin = &USART_str; 		
}

/*************************************************************************************************************
 	Interruption:	SIG_UART_RECV  
 	Description:	Interruption sur réception de caractere sur le USART. 
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Interruption sur réception de caractere sur le USART.
**************************************************************************************************************/
SIGNAL(SIG_UART_RECV) 
{
	char c;

	/** Réception du caractère dans le tampon de réception */	
	c = UDR;

	tampon_rx[rx_in & (NB_TAMPON_RX-1)] = c;
	rx_in++;
}

/*************************************************************************************************************
 	Interruption:	SIG_UART_DATA 
 	Description:	Interruption sur fin de transmission du USART.
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Interruption sur fin de transmission du USART.
**************************************************************************************************************/
SIGNAL(SIG_UART_DATA) 
{
	/** Si nous avons un autre caractère à transmettre dans le tampon de transmission */
	if(tx_in != tx_out) 
	{	
		/** Transmission du caractère */
		UDR = tampon_tx[tx_out & (NB_TAMPON_TX-1)];
		tx_out++;		
	}
	else 
	{
		/** Sinon, désactivation de l'interruption sur fin de transmission */
		UCSRB &= ~(1<<UDRIE);
	}
}

/*************************************************************************************************************
 	Fonction :		USART_putchar 
 	Description :	Permet d'envoyer un caractère sur le USART.
 	Paramètres: 	Data : Caractère a envoyer.
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Fonction permettant d'envoyer un caractère sur le USART.
**************************************************************************************************************/
int USART_putchar(char c, FILE *unused)
{
	/** Attendre que le tampon de transmission se vide */
	while((NB_TAMPON_TX - (char)(tx_in - tx_out)) <= 2);
	
	/** Entrée des caractères à transmettre dans le tampon de transmission */
	tampon_tx[tx_in & (NB_TAMPON_TX-1)] = c;
	tx_in++;
	
	/** Activer l'interruption sur fin de transmission */
	UCSRB |= (1<<UDRIE);			// Enable UDR empty interrupt

	return(0);
}

/*************************************************************************************************************
 	Fonction :		USART_getchar  
 	Description :	Permet de recevoir un caractère du USART.
 	Paramètres: 	aucun	
 	Valeur retour:	Caractère a recevoir.
**************************************************************************************************************/
/** @brief	Fonction permettant de recevoir un caractere du USART.
**************************************************************************************************************/
int USART_getchar(FILE *unused)
{
	char c;

	/** Attendre qu'il y ait quelque chose dans le tampon de réception */
	while((char)(rx_in - rx_out) == 0);
	
	/** Réception du caractère à partir du tampon de réception */
	c = tampon_rx[rx_out & (NB_TAMPON_TX-1)];
	rx_out++;
			
	return((int)c);
}

