#ifndef CONFIG_H
#define CONFIG_H

#define ORPHY_MIN_POINTS 1
#define ORPHY_MAX_POINTS 2000
#define ORPHY_MIN_PERIOD 0.02 //en ms
#define ORPHY_MAX_PERIOD 32.767 //en ms


#define ORPHY_MAX_FREQUENCY (1000/ORPHY_MIN_PERIOD)
#define ORPHY_MIN_FREQUENCY (1000/ORPHY_MAX_PERIOD)

//#define DEBUG_MODE//Utilise la sortie standard Ã  la place de la base


#endif
