/*************************************************************************************************************
*    Titre:         Module d'accès pour le MAGTEK 21006576 en interruptions.
*    Fichier:       magtek.c
*    Date:          9/12/2007
*************************************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdio.h>

#include "magtek.h"

/**************************************************************************************************************
 ** @file magtek.c
 ** @brief	Module d'accès pour le MAGTEK 21006576 en interruptions.
 **************************************************************************************************************/														

/*************************************************************************************************************
 	Fonction :		init_MAGTEK  
 	Description :	Initialisation du module de lecture de cartes magnétiques MAGTEK.
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Initialisation du module de lecture de cartes magnétiques MAGTEK.
**************************************************************************************************************/
void init_MAGTEK(void)
{
	unsigned int i;
	
	/* !CARD = PD3 * !STROBE = PB2 * !DATA = PD4 */
	DDRD &= ~((1<<3)|(1<<4));
	DDRB &= ~((1<<2));
	/* Pull-ups */	
	PORTD |= ((1<<3)|(1<<4));
	PORTB |= ((1<<2));
	
	/** Efface les chaines précédentes */
	for(i=0;i<MAGTEK_BITS;i++)
	{
		MAGTEK_bits[i]=0;
	}
	
	MAGTEK_str[0]=0x00;
		
	/** Initialisation des status Read in Progress =0 et Read Ended =0 */
	MAGTEK_rip=0;
	MAGTEK_re=0;

	/** Initialisation du status StartSentinel Received */		
	MAGTEK_f_bit=0;
			
	/* Interruption sur changement de INT1(!CARD) */
	MCUCR |= (1<<ISC10);
	/* Interruption sur descente de INT2(!STROBE) */
	MCUCSR &= ~(1<<ISC2);

	GICR |= ((1<<INT1)|(1<<INT2));
}

/*************************************************************************************************************
 	Fonction :		SIG_INTERUPT6 
 	Description :	Le signal !CARD PRESENT change de bas en haut ou de haut en bas.
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Le signal !CARD PRESENT change de bas en haut ou de haut en bas.
**************************************************************************************************************/
SIGNAL(SIG_INTERRUPT1)
{
	unsigned int i;
	
	if(PIND & (1<<3)) /** Le signal !CARD PRESENT signale une fin de lecture */
	{
		if(!MAGTEK_analyse())	/** Si la lecture est valide */
		{
			MAGTEK_re=1;	/** Read Ended = 1 */

		}

		/** Initialisation des status Read in Progress = 0 et First Bit = 0 */
		MAGTEK_rip=0;
		MAGTEK_f_bit=0;

	}
	else  /** Le signal !CARD PRESENT signale un debut de lecture */
	{
		/** Efface les chaines précédentes */
		for(i=0;i<MAGTEK_BITS;i++)
		{
			MAGTEK_bits[i]=0;
		}

		MAGTEK_str[0]=0x00;

		/** Initialisation des status Read in Progress = 1 et Read Ended = 0 */
		MAGTEK_rip=1;
		MAGTEK_re=0;

		/** Initialisation du status First Bit = 0 */		
		MAGTEK_f_bit=0;

	}	
}

/*************************************************************************************************************
 	Fonction :		SIG_INTERUPT7 
 	Description :	Le signal !STROBE a été enclanché.
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Le signal !STROBE a été enclanché.
**************************************************************************************************************/
SIGNAL(SIG_INTERRUPT2)
{
	unsigned char bit_read;

	bit_read = (((~PIND) & (1<<4))>>4);	/** Lecture de la pin !DATA */
	
	if(MAGTEK_f_bit!=0)
	{
		if(bit_read)
		{
			MAGTEK_bits[MAGTEK_bit_cnt]=1;
		}
		else
		{
			MAGTEK_bits[MAGTEK_bit_cnt]=0;		
		}
		
		if(MAGTEK_bit_cnt<MAGTEK_BITS)
		{
			MAGTEK_bit_cnt++;
		}
		
	}
	else
	{
		if(bit_read)
		{
			MAGTEK_bits[0]=1;
			MAGTEK_bit_cnt=1;
			MAGTEK_f_bit=1;
		}
	}	
}

/*************************************************************************************************************
 	Fonction :		MAGTEK_analyse
 	Description :	Analyse des données scannées et construction de la chaine de caratères.
 	Paramètres: 	aucun
 	Valeur retour:	Validité de la lecture (0 = Valide, 1 = Invalide )
**************************************************************************************************************/
/** @brief	Analyse des données scannées et construction de la chaine de caratères.
**************************************************************************************************************/
unsigned int MAGTEK_analyse(void)
{
	unsigned int ss_pos=0;
	unsigned int dir=0;
	
	unsigned int i,j,ii;

	if(MAGTEK_bits[0]==1 &&
	   MAGTEK_bits[1]==1 &&
	   MAGTEK_bits[2]==0 &&
	   MAGTEK_bits[3]==1 &&
	   MAGTEK_bits[4]==0)
	{
		ss_pos=0;	/** Il est trouvé, fin du for */
		dir=0;	/** Direction = sens normal */
	}
	else
	{
		for(i=MAGTEK_BITS;i>4;i--)	/** Recherche du Start Sentinel en sens inverse */
		{
			if(MAGTEK_bits[i]==1 &&
			   MAGTEK_bits[i-1]==1 &&
			   MAGTEK_bits[i-2]==0 &&
			   MAGTEK_bits[i-3]==1 &&
			   MAGTEK_bits[i-4]==0)
			{
				ss_pos=i;	/** Il est trouvé, fin du for */
				dir=1;	/** Direction = sens inverse */
				break;
			}
		}
		if(ss_pos==0)
		{
			return(1);	/** Sinon, nous avons un scan invalide */
		}
	}
	

	/** Partie 2 : Construction de la chaine de caractères */

	if(dir==0)	/** Si la position est en sens normal */
	{
		ii=ss_pos;
		
		for(i=0;i<MAGTEK_CAR && ii<MAGTEK_bit_cnt;i++)	/** On batit la chaine en sens normal à partir du SS */
		{
			MAGTEK_str[i]=0x00; /** Initialisation du byte à écrire */
				
			for(j=0;j<5;j++)
			{			
				if(ii<MAGTEK_bit_cnt)
				{				
					MAGTEK_str[i] |= (MAGTEK_bits[ii]<<j);			
					ii++;
				}
			}			
						
			MAGTEK_str[i] = (MAGTEK_str[i]&(0x0F))+ (0x30); /** Byte lue &0x0F + 0x30 = ASCII */
			
			if(MAGTEK_str[i]=='<')
				 MAGTEK_str[i]='[';
			if(MAGTEK_str[i]=='>')
				 MAGTEK_str[i]=']';
			
			if(MAGTEK_str[i]=='?')	/** Si nous atteignons un End Sentinel */
			{
				MAGTEK_str[i+1]=0x00; /** Fin de chaine */
				i=MAGTEK_CAR;	/** Sortie du for */
			}
		}
		MAGTEK_str[MAGTEK_CAR-1]=0x00;	/** Fin de chaine absolue */
	}
	else	/** Sinon la position est en sens inverse */
	{
		ii=ss_pos;
		
		for(i=0;i<MAGTEK_CAR && ii>0;i++) /** On batit la chaine en sens inverse à partir du SS */
		{
			MAGTEK_str[i]=0x00; /** Initialisation du byte à écrire */
					
			for(j=0;j<5;j++)
			{			
				if(ii>0)
				{			
					MAGTEK_str[i] |= (MAGTEK_bits[ii]<<j);
					ii--;
				}
			}
						
			MAGTEK_str[i] = (MAGTEK_str[i]&(0x0F))+ (0x30); /** Byte lue &0x0F + 0x30 = ASCII */
			
			if(MAGTEK_str[i]=='<')
				 MAGTEK_str[i]='[';
			if(MAGTEK_str[i]=='>')
				 MAGTEK_str[i]=']';
			
			if(MAGTEK_str[i]=='?')	/** Si nous atteignons un End Sentinel */
			{
				MAGTEK_str[i+1]=0x00; /** Fin de chaine */
				i=MAGTEK_CAR;	/** Sortie du for */
			}
		}
		MAGTEK_str[MAGTEK_CAR-1]=0x00;	/** Fin de chaine absolue */
	}
	
	return(0);	/** Scan valide */		
}

