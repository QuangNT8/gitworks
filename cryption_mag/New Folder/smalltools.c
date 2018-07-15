/*************************************************************************************************************
*    Titre:         Librairie de petites outils.
*    Fichier:       smalltools.c
*    Date:          16/1/2008
*************************************************************************************************************/

#include "smalltools.h"
#include "usart.h"
#include "eeprom.h"															

/**************************************************************************************************************
 ** @file smalltools.c
 ** @brief	Librairie de petites outils.
 **************************************************************************************************************/
/*************************************************************************************************************
 	Fonction :		USART_getstring  
 	Description :	Fonction évoluée de gets, adaptée à nos besoins.
 	Paramètres: 	Type de renvoi (RIEN,ECHO,ÉTOILES), nombre de caractères max, chaine
 	Valeur retour:	Nombre de caractères
**************************************************************************************************************/
/** @brief	Fonction évoluée de gets, adaptée à nos besoins.
**************************************************************************************************************/
unsigned int USART_getstring(unsigned char echo, unsigned int nbcarmax, char* chaine)
{
	char c;
	unsigned int nbcar=0;
	unsigned char fini=FAUX;
		
	while(nbcar<nbcarmax && !fini)
	{
		c = USART_getchar(NULL);
		switch(c)
		{
			case 0x08:	// BackSpace
			{
				if(nbcar>0)	// Au moins un caractère dans la chaine
				{
					nbcar--;
					USART_putchar(0x08, NULL);
				}
				else
					USART_putchar(0x07, NULL);	// Sinon, DING!
				break;
			}
			case 0x0D:	// Return ou enter
			{
				chaine[nbcar]=0;
				if(nbcar==0)
				{
					return(FAUX);
				}
				fini=VRAI;
			}
			break;
		default:
			if(c>=32 && c<=126 && nbcar<(nbcarmax-1))	//Si c'est un caractère ASCII imprimable
			{
				chaine[nbcar]=c;
				nbcar++;
				if(echo==ECHO)	// Echo!
					USART_putchar(c, NULL);
				else if(echo==STAR_ECHO)	//Echo étoilé!
					USART_putchar('*', NULL);
			}
			else
				USART_putchar(0x07, NULL);	// Sinon, DING!
		}
		
	}
	
	return(nbcar);
}

/*************************************************************************************************************
 	Fonction :		strtoi
 	Description :	Conversion d'une chaine de caractère en nombre entier.
 	Paramètres: 	Chaine de caractère.
 	Valeur retour:	Nombre entier.
**************************************************************************************************************/
/** @brief	Conversion d'une chaine de caractère en nombre entier.
**************************************************************************************************************/
unsigned int strtoi(char *str)
{
   unsigned int val = 0, digit;
   char c;

   while ((c = *str++) != 0x00) 
   {
        if (c >= '0' && c <= '9')
            digit = (unsigned int) (c - '0');
        else
            break;

        val = (val * 10) + digit;
    }

    return val;
}

/*************************************************************************************************************
 	Fonction :		stringcomp
 	Description :	Comparaison de deux chaines de caractères.
 	Paramètres: 	Deux pointeurs sur les chaines à comparer.
 	Valeur retour:	0 = pareilles, 1 = différentes
**************************************************************************************************************/
/** @brief	Comparaison de deux chaines de caractères.
**************************************************************************************************************/
int stringcomp(char *a, char *b)
{
	char *aa=a;
	char *bb=b;

	while(*aa !=0x00 && *bb !=0x00)
	{
		if(*aa!=*bb)
			return(1);
		aa++;
		bb++;
	}
	if(*aa!=*bb)
		return(1);

	return(0);
}

/*************************************************************************************************************
 	Fonction :		stringcpy
 	Description :	Copie d'une chaine de caractères dans une autre.
 	Paramètres: 	Deux pointeurs sur les chaines destination etn source.
 	Valeur retour:	rien
**************************************************************************************************************/
/** @brief	Copie d'une chaine de caractères dans une autre.
**************************************************************************************************************/
void stringcpy(char *dst, char *src, int n)
{
	int i;
	
	for(i=0;i<n && *src !=0x00;i++)
		*(dst+i)=*(src+i);
}

/*************************************************************************************************************
 	Fonction :		read_eeptr  
 	Description :	Lecture du pointeur d'insertion en mémoire EEPROM.
 	Paramètres: 	aucun
 	Valeur retour:	unsigned long pointeur sur la mémoire EEPROM
**************************************************************************************************************/
/** @brief	Lecture du pointeur d'insertion en mémoire EEPROM.
**************************************************************************************************************/
void read_eeptr(unsigned long *eeptr)
{
	long_char eeadr;
	
	EEPROM_read(200, 4, (char *)&eeadr);
	
	*eeptr=eeadr.var_long;
}

/*************************************************************************************************************
 	Fonction :		write_eeptr  
 	Description :	Mise en mémoire du pointeur d'insertion en mémoire EEPROM.
 	Paramètres: 	aucun
 	Valeur retour:	unsigned long pointeur sur la mémoire EEPROM
**************************************************************************************************************/
/** @brief	Mise en mémoire du pointeur d'insertion en mémoire EEPROM.
**************************************************************************************************************/
void write_eeptr(unsigned long eeptr)
{
	long_char eeadr;
	
	eeadr.var_long=eeptr;
		
	EEPROM_write(200, 4, (char *)&eeadr);
}

/*************************************************************************************************************
 	Fonction :		wait
 	Description :	Attente.
 	Paramètres: 	Fraction de 1/4096 de seconde
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Attente.
**************************************************************************************************************/
void wait(unsigned int t)
{
	delai=t;

	while(delai);
}
/*************************************************************************************************************
 	Fonction :		crcccitt
 	Description :	Calcule le CRC d'un chaine à la façon du CCITT (Ethernet)
 	Paramètres: 	nombre de départ pour le CRC, chaine de car., longueur de la chaine
 	Valeur retour:	CRC16 (CCITT)
**************************************************************************************************************/
/** @brief	Attente.
**************************************************************************************************************/
unsigned int crcccitt(unsigned int crc_init, char donnee[], unsigned long int size)
{
	char index;
 	unsigned long int i;

  	for(i = 0; i < size; i++)
   	{
    		crc_init = (unsigned int)(crc_init ^ (donnee[i] << 8));
 
     		for(index = 0; index <= 7; index++)
     		{
      			if(crc_init & 0x8000)
       				crc_init = (unsigned int)((crc_init << 1) ^ 0x1021);
      			else
       				crc_init = (unsigned int)(crc_init << 1);
       		}
  	}
 	return crc_init;
}

