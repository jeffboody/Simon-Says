/*
 Started: 12-26-2012
 Spark Fun Electronics

 The SparkFun Simon Says game has been through dozens of revisions over the years. This header
 file attempts to map and support all the different hardware versions.
*/

// Uncomment one of the following, corresponding to the board you have.
//#define BOARD_REV_6_25_08
//#define BOARD_REV_4_9_2009
#define BOARD_REV_PTH

#ifdef BOARD_REV_PTH

#define CHIP_ATMEGA168

#define LED_A	(1 << 0)
#define LED_B	(1 << 1)
#define LED_C	(1 << 2)
#define LED_D	(1 << 3)

/* LED pin definitions */
#define LED_A_PIN		2
#define LED_A_PORT		PORTB
#define LED_B_PIN		3
#define LED_B_PORT		PORTD
#define LED_C_PIN		5
#define LED_C_PORT		PORTB
#define LED_D_PIN		5
#define LED_D_PORT		PORTD

/* Button pin definitions */
#define BUTTON_A_PIN	1
#define	BUTTON_A_PORT	PINB
#define BUTTON_B_PIN	2
#define	BUTTON_B_PORT	PIND
#define BUTTON_C_PIN	4
#define	BUTTON_C_PORT	PINB
#define BUTTON_D_PIN	6
#define	BUTTON_D_PORT	PIND

/* Buzzer pin definitions */
#define BUZZER1		4
#define BUZZER1_PORT	PORTD
#define BUZZER2		7
#define BUZZER2_PORT	PORTD

#endif  /* BOARD_REV_PTH */

#ifdef BOARD_REV_6_25_08

#define CHIP_ATMEGA168

#define LED_A	(1 << 0)
#define LED_B	(1 << 1)
#define LED_C	(1 << 2)
#define LED_D	(1 << 3)

/* LED pin definitions */
#define LED_A_PIN		3
#define LED_A_PORT		PORTC
#define LED_B_PIN		2
#define LED_B_PORT		PORTD
#define LED_C_PIN		0
#define LED_C_PORT		PORTC
#define LED_D_PIN		5
#define LED_D_PORT		PORTD

/* Button pin definitions */
#define BUTTON_A_PIN	2
#define	BUTTON_A_PORT	PINC
#define BUTTON_B_PIN	5
#define	BUTTON_B_PORT	PINC
#define BUTTON_C_PIN	1
#define	BUTTON_C_PORT	PINC
#define BUTTON_D_PIN	6
#define	BUTTON_D_PORT	PIND

/* Buzzer pin definitions */
#define BUZZER1		3
#define BUZZER1_PORT	PORTD
#define BUZZER2		4
#define BUZZER2_PORT	PORTD

#endif  /* BOARD_REV_6_25_08 */

#ifdef BOARD_REV_4_9_2009

#define LED_A		(1 << 0)
#define LED_B	(1 << 1)
#define LED_C	(1 << 2)
#define LED_D	(1 << 3)

#define CHIP_ATMEGA168

/* LED pin definitions */
#define LED_C_PIN		5
#define LED_C_PORT		PORTB
#define LED_D_PIN		5
#define LED_D_PORT		PORTD
#define LED_A_PIN		2
#define LED_A_PORT		PORTB
#define LED_B_PIN		2
#define LED_B_PORT		PORTD

/* Button pin definitions */
#define BUTTON_A_PIN	0
#define	BUTTON_A_PORT	PINB
#define BUTTON_B_PIN	1
#define	BUTTON_B_PORT	PINB
#define BUTTON_C_PIN	7
#define	BUTTON_C_PORT	PIND
#define BUTTON_D_PIN	6
#define	BUTTON_D_PORT	PIND

/* Buzzer pin definitions */
#define BUZZER1		3
#define BUZZER1_PORT	PORTD
#define BUZZER2		4
#define BUZZER2_PORT	PORTD

#endif  /* BOARD_REV_4_9_2009 */

