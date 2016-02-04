#include "stm8.h"
#include "watchdog.h"

void watchdog_kick(void) {
	IWDG_KR = IWDG_KR_REFRESH;
}

void watchdog_init(void) {

	IWDG_KR = IWDG_KR_ENABLE;
	IWDG_KR = IWDG_KR_ACCESS;
	IWDG_PR = IWDG_PR_64;
	IWDG_KR = IWDG_KR_REFRESH;

}
