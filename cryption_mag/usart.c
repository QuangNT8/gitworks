/*************************************************************************************************************
*    Fichier:   usart.c              
*    But:       Module de communication sur le usart utilisant des tampons en interruption.
*    Date:      6/11/2005 modifi� pour atmega 8 29/07/2007
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
 	Param�tres: 	Vitesse de tansmission en bauds
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Fonction permettant d'initialiser le USART.
**************************************************************************************************************/
void USART_init(unsigned long bauds) 
{
	unsigned int ubrr;
	
	/** Calcul des valeurs � appliquer � UBRR afin d'atteindre le nombre de bauds voulues */
	ubrr=(unsigned short)((F_CPU / (bauds * 8l)) - 1);
    
    /** Application des valeurs aux registres d'UBRR */
    UBRRH = (unsigned char)(ubrr>>8);
    UBRRL = (unsigned char)ubrr;

	/** Double vitesse */
    UCSRA = (1<<U2X);
	/** TX0 activ�, RX0 activ� ainsi que l'interruption sur RX0 */
    UCSRB = ((1<<RXCIE)|(1<<RXEN)|(1<<TXEN));
    /** 8 bits, 1 stop bit, utilisation de UBRR */
    UCSRC = ((1<<UCSZ0)|(1<<UCSZ1)|(1<<URSEL));

	/** Accrochage des fonction USART aux commandes d'STDIO.H */
	stdout = stdin = &USART_str; 		
}

/*************************************************************************************************************
 	Interruption:	SIG_UART_RECV  
 	Description:	Interruption sur r�ception de caractere sur le USART. 
 	Param�tres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Interruption sur r�ception de caractere sur le USART.
**************************************************************************************************************/
SIGNAL(SIG_UART_RECV) 
{
	char c;

	/** R�ception du caract�re dans le tampon de r�ception */	
	c = UDR;

	tampon_rx[rx_in & (NB_TAMPON_RX-1)] = c;
	rx_in++;
}

/*************************************************************************************************************
 	Interruption:	SIG_UART_DATA 
 	Description:	Interruption sur fin de transmission du USART.
 	Param�tres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Interruption sur fin de transmission du USART.
**************************************************************************************************************/
SIGNAL(SIG_UART_DATA) 
{
	/** Si nous avons un autre caract�re � transmettre dans le tampon de transmission */
	if(tx_in != tx_out) 
	{	
		/** Transmission du caract�re */
		UDR = tampon_tx[tx_out & (NB_TAMPON_TX-1)];
		tx_out++;		
	}
	else 
	{
		/** Sinon, d�sactivation de l'interruption sur fin de transmission */
		UCSRB &= ~(1<<UDRIE);
	}
}

/*************************************************************************************************************
 	Fonction :		USART_putchar 
 	Description :	Permet d'envoyer un caract�re sur le USART.
 	Param�tres: 	Data : Caract�re a envoyer.
 	Valeur retour:	
**************************************************************************************************************/
/** @brief	Fonction permettant d'envoyer un caract�re sur le USART.
**************************************************************************************************************/
int USART_putchar(char c, FILE *unused)
{
	/** Attendre que le tampon de transmission se vide */
	while((NB_TAMPON_TX - (char)(tx_in - tx_out)) <= 2);
	
	/** Entr�e des caract�res � transmettre dans le tampon de transmission */
	tampon_tx[tx_in & (NB_TAMPON_TX-1)] = c;
	tx_in++;
	
	/** Activer l'interruption sur fin de transmission */
	UCSRB |= (1<<UDRIE);			// Enable UDR empty interrupt

	return(0);
}

/*************************************************************************************************************
 	Fonction :		USART_getchar  
 	Description :	Permet de recevoir un caract�re du USART.
 	Param�tres: 	aucun	
 	Valeur retour:	Caract�re a recevoir.
**************************************************************************************************************/
/** @brief	Fonction permettant de recevoir un caractere du USART.
**************************************************************************************************************/
int USART_getchar(FILE *unused)
{
	char c;

	/** Attendre qu'il y ait quelque chose dans le tampon de r�ception */
	while((char)(rx_in - rx_out) == 0);
	
	/** R�ception du caract�re � partir du tampon de r�ception */
	c = tampon_rx[rx_out & (NB_TAMPON_TX-1)];
	rx_out++;
			
	return((int)c);
}

