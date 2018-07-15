/*************************************************************************************************************
*    Titre:         Module d'accès pour le RTC Dallas DS1337
*    Fichier:       rtc_ds1337.c
*    Date:          07/09/2007
*************************************************************************************************************/

#include <avr/io.h>

#include "rtc_ds1337.h"
#include "twi.h"

/**************************************************************************************************************
 ** @file rtc.c
 ** @brief	Module d'accès pour le RTC Dallas DS1337
 **************************************************************************************************************/														

/*************************************************************************************************************
 	Fonction :		bcd2dec  
 	Description :	Conversion BCD à décimal.  
 	Paramètres: 	Nombre en BCD
 	Valeur retour:	Nombre en décimal
**************************************************************************************************************/
/** @brief	Conversion BCD à décimal.
**************************************************************************************************************/
unsigned char bcd2dec(unsigned char bcd)
{
	return ((bcd>>4)*10)+bcd%16;
}

/*************************************************************************************************************
 	Fonction :		dec2bcd  
 	Description :	Conversion décimal à BCD.  
 	Paramètres: 	Nombre en décimal
 	Valeur retour:	Nombre en BCD
**************************************************************************************************************/
/** @brief	Conversion décimal à BCD.
**************************************************************************************************************/
unsigned char dec2bcd(unsigned char dec)
{
	return ((dec/10)<<4)+(dec%10);
}

/*************************************************************************************************************
 	Fonction :		RTC_init  
 	Description :	Initialisation du RTC.  
 	Paramètres: 	aucun
 	Valeur retour:	État du TWI.
**************************************************************************************************************/
/** @brief	Fonction permettant d'initialiser le RTC.
**************************************************************************************************************/
unsigned char RTC_init(void)
{
	unsigned char temp[2];
	tx_type tx_frame[2];

	tx_frame[0].slave_adr = RTC_ADR+W;		/** Adresse du RTC DS1337, en écriture */
	tx_frame[0].size = 2;					/** Nombre de données à transmettre */
	tx_frame[0].data_ptr = temp;			/** Fait pointer le pointeur sur le tampon temporaire */
	tx_frame[0].data_ptr[0] = RTC_CONTROL;	/** Adresse de base des registres de controle du RTC */
	tx_frame[0].data_ptr[1] = 0x08;			/** Valeurs par défaut, 4.096KHz clock pulse vers int0 */
	
	tx_frame[1].slave_adr = OWN_ADR;		/** Fin de la trame TWI */

	return(Send_to_TWI(tx_frame));			/** Appelle la fonction d'envoi sur le TWI */
}

/*************************************************************************************************************
 	Fonction :		RTC_get_time  
 	Description :	Lecture des variables de temps contenues dans le RTC vers la structure globale.  
 	Paramètres: 	aucun
 	Valeur retour:	État du TWI.
**************************************************************************************************************/
/** @brief	Lecture des variables de temps contenues dans le RTC vers la structure globale. 
**************************************************************************************************************/
unsigned char RTC_get_time(void)
{
	unsigned char etat;
	unsigned char temp0;
	unsigned char temp1[16];
	unsigned int an;
	
	tx_type tx_frame[3];

	etat = SUCCESS;
	
	tx_frame[0].slave_adr = RTC_ADR+W;		/** Adresse du RTC DS1338C, en écriture */
	tx_frame[0].size = 1;					/** Nombre de données à transmettre */
	tx_frame[0].data_ptr = &temp0;			/** Fait pointer le pointeur sur le tampon temporaire */
	tx_frame[0].data_ptr[0] = RTC_SECONDS; 	/** Passe l'adresse de base du DS1337 pour la lecture */
	
	tx_frame[1].slave_adr = RTC_ADR+R;		/** Démarre une lecture maitre */
	tx_frame[1].size = 16;					/** Nombre de données à lires */
	tx_frame[1].data_ptr = temp1;			/** Fait pointer le pointeur sur le tampon temporaire */

	tx_frame[2].slave_adr = OWN_ADR;		/** Fin de la trame TWI */

	etat = Send_to_TWI(tx_frame);			/** Appelle la fonction d'envoi sur le TWI */

	if(etat != SUCCESS)						/** En cas d'erreur, retourner le code d'erreur, fin */
		return etat;	
	
	if(temp1[RTC_MONTHS_C]>>7)			/** Calcul à partir de la variable siècle (0=2000 1=1900) */
		an=1900;
	else
		an=2000;
	
	/** Conversions et placement des variable de temps dans la structure de temps globale */
	heure_actuelle.heure=bcd2dec(temp1[RTC_HOURS]&0x3F);
	heure_actuelle.minute=bcd2dec(temp1[RTC_MINUTES]&0x7F);
	heure_actuelle.seconde=bcd2dec(temp1[RTC_SECONDS]&0x7F);

	heure_actuelle.annee=an+bcd2dec(temp1[RTC_YEARS]);
	heure_actuelle.mois=bcd2dec(temp1[RTC_MONTHS_C]&0x1F);
	heure_actuelle.jour=bcd2dec(temp1[RTC_DAYS]&0x3F);

	heure_actuelle.jour_semaine=bcd2dec(temp1[RTC_WEEKDAYS]&0x07);

	heure_actuelle.batt_low=temp1[RTC_STATUS]>>7;
	
	return(etat);			/** Retourne l'état du TWI */
}

/*************************************************************************************************************
 	Fonction :		RTC_set_time  
 	Description :	Modification des variables de temps dans le RTC depuis la structure globale.  
 	Paramètres: 	aucun
 	Valeur retour:	État du TWI.
**************************************************************************************************************/
/** @brief	Modification des variables de temps dans le RTC depuis la structure globale. 
**************************************************************************************************************/
unsigned char RTC_set_time(void)
{
	unsigned char siecle;
	unsigned char temp0[8];
	unsigned char temp1[3];
	tx_type tx_frame[3];
	
	/** Ajustement de la variable siecle selon l'année recue */
	if(heure_actuelle.annee>=2000)
	{
		heure_actuelle.annee=heure_actuelle.annee-2000;
		siecle=0;
	}
	else
	{
		heure_actuelle.annee=heure_actuelle.annee-1900;
		siecle=1;
	}

	tx_frame[0].slave_adr = RTC_ADR+W;		/** Adresse du RTC DS1337, en écriture */
	tx_frame[0].size = 8;					/** Nombre de données à transmettre */
	tx_frame[0].data_ptr = temp0;			/** Fait pointer le pointeur sur le tampon temporaire */
	tx_frame[0].data_ptr[0] = RTC_SECONDS;	/** Adresse de base des registres de temps du RTC */
	tx_frame[0].data_ptr[1] = dec2bcd(heure_actuelle.seconde);		/** Régistre secondes */
	tx_frame[0].data_ptr[2] = dec2bcd(heure_actuelle.minute);		/** Régistre minutes */
	tx_frame[0].data_ptr[3] = dec2bcd(heure_actuelle.heure);		/** Régistre heures */
	tx_frame[0].data_ptr[4] = dec2bcd(heure_actuelle.jour_semaine);			/** Régistre jours/semaine */
	tx_frame[0].data_ptr[5] = dec2bcd(heure_actuelle.jour);			/** Régistre jours */
	tx_frame[0].data_ptr[6] = dec2bcd(heure_actuelle.mois)|(siecle<<7);		/** Régistre mois/siecle */
	tx_frame[0].data_ptr[7] = dec2bcd((unsigned char)heure_actuelle.annee);	/** Régistre années */
	
	tx_frame[1].slave_adr = RTC_ADR+W;		/** Adresse du RTC DS1337, en écriture */
	tx_frame[1].size = 2;					/** Nombre de données à transmettre */
	tx_frame[1].data_ptr = temp1;			/** Fait pointer le pointeur sur le tampon temporaire */
	tx_frame[1].data_ptr[0] = RTC_STATUS;	/** Adresse de base des registres de statut du RTC */
	tx_frame[1].data_ptr[1] = 0x00;			/** Valeurs toutes à zéro (descente des flags) */

	tx_frame[2].slave_adr = OWN_ADR;		/** Fin de la trame TWI */

	return(Send_to_TWI(tx_frame));			/** Appelle la fonction d'envoi sur le TWI */
}


