/*************************************************************************************************************
*    Titre:         Module pour le clavier 4x4 (test clavier)
*    Fichier:       kb.c
*    Date:          04/02/2008
*************************************************************************************************************/

#include <avr/io.h>

#include "kb.h"
#include "main.h"

/**************************************************************************************************************
 ** @file kb.c
 ** @brief	Module pour le clavier 4x4
 **************************************************************************************************************/														
/*************************************************************************************************************
 	Fonction :		KB_Init  
 	Description :	Initialisation du clavier 4x4.  
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Fonction permettant d'initialiser le clavier 4x4.
**************************************************************************************************************/
void KB_Init(void)
{
	unsigned int i;

	/** Initialisation des variables de clavier */
	DDRA = 0xF0;	/* PA0-PA3 input, PA4-PA7 output */
	PORTA = 0xEF;	/* Pullup des inputs + ligne 0 (1110)*/

	KB_scan_line=0; /* 0, 1, 2, 3 */
	KB_key=0; 		/* Clef appuyée  */
	KB_keypressed=0; /* FAUX=0, VRAI=n'importe quoi d'autre */
	KB_ready=0; /* Pas de nouvelle clef */
	KB_timer=0; /* Pas de timer actif*/
	/** Remise à zéro de la chaine de caratères reçus */
    for(i=0;i<KB_CHARS;i++)
    {
        KB_buf[i]=0x00;
    }
	KB_char_cnt=0; /* Nombre de caractères dans la chaine */
	KB_in_read=FALSE; /* Pas en lecture de chaine*/
}

/*************************************************************************************************************
 	Fonction :		KB_int_proc
 	Description :	Procédure de lecture en interruption du clavier 4x4.  
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Procédure de lecture en interruption du clavier 4x4.
**************************************************************************************************************/
void KB_int_proc(void)
{
	unsigned char key;

	key=~(0xF0|(PINA&0x0F)); /* Lecture de la ligne correspondante */
	if(key==4)
		key=3;
	if(key==8)
		key=4;
	
	if(key) /* Si une clef est appuyée */
	{
		key=key-1+(4*(KB_scan_line%4));

		/* Conversion de scan code à clef en passant par le keymap */
		KB_key=keymap[key];		

		if(!KB_keypressed)
		{
			KB_ready=1; /* Nouvelle clef! */
		}

		KB_keypressed=KB_keypressed|(1<<(KB_scan_line%4)); /* Flag appuyé */
	}
	else /* Sinon */
	{
		KB_keypressed=KB_keypressed&~(1<<(KB_scan_line%4)); /* Flag relâché */
	}

	/* Ligne suivante */
	KB_scan_line++;
	PORTA = (0x0F|(~(1<<(KB_scan_line%4)))<<4);
}
