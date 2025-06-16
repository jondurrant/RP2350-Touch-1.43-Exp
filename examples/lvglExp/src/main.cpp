#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
extern "C"{
#include "bsp_i2c.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "bsp_battery.h"
#include "Widgets.h"
}

#define LVGL_TICK_PERIOD_MS 1

#define DISP_HOR_RES 466
#define DISP_VER_RES 466


void set_cpu_clock(uint32_t freq_Mhz)
{
    set_sys_clock_hz(freq_Mhz * MHZ, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        freq_Mhz * MHZ,
        freq_Mhz * MHZ);
}

static bool repeating_lvgl_timer_cb(struct repeating_timer *t)
{
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
    return true;
}

int main()
{
    stdio_init_all();
    set_cpu_clock(250);
    bsp_i2c_init();

    lv_init();
	lv_port_disp_init(DISP_HOR_RES, DISP_VER_RES, 0, false);
	lv_port_indev_init(DISP_HOR_RES, DISP_VER_RES, 0);
	static struct repeating_timer lvgl_timer;
	add_repeating_timer_ms(LVGL_TICK_PERIOD_MS, repeating_lvgl_timer_cb, NULL, &lvgl_timer);


	Widgets_Init();


	while (true){
	   lv_timer_handler();
	   sleep_ms(LVGL_TICK_PERIOD_MS);
	}



}
