/************************************************************************************************************
*    Titre:         Définitions pour le RTC Dallas DS1337
*    Fichier:       rtc_ds1337.h
*    Date:          07/09/2007
************************************************************************************************************/

/************************************************************************************************************
 ** @file rtc.h                                                                                            
 ** @brief Définitions pour le RTC Dallas DS1337                                                             
 ************************************************************************************************************/

#ifndef __RTC_H__
#define __RTC_H__

/*************************************************************************************************************
	Définitions des constantes
*************************************************************************************************************/
#define	RTC_ADR	0xD0			/** Adresse esclave du RTC */

#define RTC_SECONDS			0x00	/** Adresse du registre des secondes */
#define RTC_MINUTES			0x01	/** Adresse du registre des minutes */
#define RTC_HOURS			0x02	/** Adresse du registre des heures */
#define RTC_WEEKDAYS		0x03	/** Adresse du registre des jours de semaine */
#define RTC_DAYS			0x04	/** Adresse du registre des jours */
#define RTC_MONTHS_C		0x05	/** Adresse du registre des mois et siecles */
#define RTC_YEARS			0x06	/** Adresse du registre des années */
#define RTC_SECONDS_ALM1	0x07	/** Adresse du registre d'alarme 1 des secondes */
#define RTC_MINUTES_ALM1	0x08	/** Adresse du registre d'alarme 1 des minutes */	
#define RTC_HOUR_ALM1		0x09	/** Adresse du registre d'alarme 1 des heures */
#define RTC_DAY_ALM1		0x0A	/** Adresse du registre d'alarme 1 des jours de semaine / jour */
#define RTC_MINUTES_ALM2	0x0B	/** Adresse du registre d'alarme 2 des minutes */
#define RTC_HOUR_ALM2		0x0C	/** Adresse du registre d'alarme 2 des heures */
#define RTC_DAY_ALM2		0x0D	/** Adresse du registre d'alarme 2 des jours de semaine / jour */
#define RTC_CONTROL			0x0E	/** Adresse du registre de contrôle */
#define RTC_STATUS			0x0F	/** Adresse du registre du statut */

/*************************************************************************************************************
	Définition de la structure du temps
*************************************************************************************************************/
typedef struct
{
	unsigned int annee;
	unsigned char mois;
	unsigned char jour;
	unsigned char jour_semaine;
	unsigned char heure;
	unsigned char minute;
	unsigned char seconde;
	unsigned char batt_low;
}temps;

/*************************************************************************************************************
	Variables globales 
*************************************************************************************************************/
temps heure_actuelle;			/** NOTE: il faut effectuer une RTC_get_time() avant de lire. */

/*************************************************************************************************************
	Définition des fonctions
*************************************************************************************************************/
unsigned char RTC_init(void);
unsigned char RTC_get_time(void);
unsigned char RTC_set_time(void);

unsigned char bcd2dec(unsigned char);
unsigned char dec2bcd(unsigned char);

void print_jour(unsigned char);

#endif
