/*  
 * Copyright (c) 2018 Guillem96
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "utils/btn.h"
#include "menu/gui/gui_argon_menu.h"
#include "menu/gui/gui_menu.h"
#include "menu/gui/gui_menu_pool.h"

#include "gfx/gfx.h"

#include "utils/types.h"
#include "utils/fs_utils.h"
#include "utils/dirlist.h"
#include "utils/util.h"
#include "utils/touch.h"
#include "utils/ini.h"

#include "core/launcher.h"
#include "core/payloads.h"
#include "core/custom-gui.h"

#include "mem/heap.h"

#define COLUMNS 3  
#define ROWS 1
#define ELEM_SIZE 230
#define MARGIN_TOP 330
#define MARGIN_LEFT 45
//emunand trigger
u32 iconH = 0;
u32 iconW = 0;
u32 iconY = 0;
u32 iconX = 0;

u32 buttonH = 0;
u32 buttonW = 0;
u32 buttonY = 0;
u32 buttonX = 0;

static int tool_reboot_rcm(void* param);
static int tool_power_off(void* param);
static int tool_emu(void* param);


/* Init needed menus for ArgonNX */
void gui_init_argon_boot(void)
{
    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");
	
	//show display without icons
    gui_menu_open2(menu);
	
	//waith user input
    bool cancel_auto_chainloading = btn_read() & BTN_VOL_UP;
    if (!cancel_auto_chainloading)
	launch_payload("atmosphere/boot_menu/bin/Atmosphere.bin");
gui_menu_pool_cleanup();
gui_init_argon_menu();
}

void gui_init_argon_menu(void)
{

    /* Init pool for menu */
    gui_menu_pool_init();

    gui_menu_t* menu = gui_menu_create("ArgonNX");
//control panel
iconH = 300;
iconW = 300;
iconY = 210;
iconX = 145;

buttonH = 300;
buttonW = 100;
//create menu entries
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-CFW.bmp"),iconX, iconY, iconW, iconH, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/Atmosphere.bin"));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/boot-Stock.bmp"),iconX + 350, iconY, iconW, iconH, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/stock.bin"));
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/gear.bmp"),iconX + 700, iconY, iconW, iconH, NULL, NULL));
/*
gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX,buttonY, buttonH, buttonW, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/Atmosphere.bin"));

gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Boot custom FW", buttonX + 70, buttonY + 35, 150, 100, NULL, NULL));

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX + 350, buttonY, buttonH, buttonW, (int (*)(void *))launch_payload, (void*)"atmosphere/boot_menu/bin/stock.bin"));

gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Boot original FW", buttonX + 430, buttonY + 35, 150, 100, NULL, NULL));

gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX + 700, buttonY, buttonH, buttonW, NULL, NULL));
gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("Options", buttonX + 770, buttonY + 35, 150, 100, NULL, NULL));
*/

	
    /*
    if (sd_file_exists ("emummc/emummc.ini"))
{
        gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX - 120,buttonY + 825, buttonH, buttonW,tool_emu, NULL)); //- 80, - 500
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC enabled", buttonX - 45, buttonY + 865, 150, 100, NULL, NULL));
		if (sd_file_exists("emummc/emummc.ini"))
        {
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC enabled", buttonX - 45, buttonY + 865, 150, 100, NULL, NULL));
        }

        if (sd_file_exists("emummc/emummc.ini.bak"))
        {
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC disabled", buttonX - 45, buttonY + 865, 150, 100, NULL, NULL)); // - 45. + 865
        }
}*/
//check emunand Status
if (sd_file_exists("emummc/emummc.ini") || sd_file_exists("emummc/emummc.ini.off"))
{
    gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),139,620, buttonH, buttonW,NULL, NULL)); //- 80, - 500
    if (sd_file_exists("emummc/emummc.ini"))
        {
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC enabled", 205, 645, 150, 100, tool_emu, NULL));
        }
        if (sd_file_exists("emummc/emummc.ini.off"))
        {
        gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuMMC disabled", 205, 645, 150, 100, tool_emu, NULL));
        }
}

/*
	if (emuNAND != 0)
	{
		gui_menu_append_entry(menu,gui_create_menu_entry("",sd_file_read("atmosphere/boot_menu/gfx/button.bmp"),buttonX - 80,buttonY - 500, buttonH, buttonW,tool_emu, NULL));
	
		if(emuNAND == 1)
		{
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuNAND enabled", buttonX - 3, buttonY + 260, 150, 100, NULL, NULL));
		}else{
		gui_menu_append_entry(menu,gui_create_menu_entry_no_bitmap("EmuNAND disabled", buttonX - 3, buttonY + 260, 150, 100, NULL, NULL));
		}
	}
    */

     gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Screenshot", 700, 680, 150, 100, (int (*)(void *))screenshot, NULL));

    /* Generate reboot rcm and shutdown entry */
    gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Power off", 900, 680, 150, 100, tool_power_off, NULL));

    gui_menu_append_entry(menu, 
            gui_create_menu_entry_no_bitmap("Reboot RCM", 1100, 680, 150, 100, tool_reboot_rcm, NULL));

    /* Start menu */
    gui_menu_open(menu);

    /* Clear all entries and menus */
    gui_menu_pool_cleanup();
}


static int tool_reboot_rcm(void* param)
{
    gui_menu_pool_cleanup();
    reboot_rcm();
    return 0;
}

static int tool_power_off(void* param)
{
    gui_menu_pool_cleanup();
    power_off();
    return 0;
}
static int tool_emu(void* param)
{
sd_mount();
        if (!g_sd_mounted)
        {return 0;}
if (sd_file_exists ("emummc/emummc.ini"))
{f_unlink("emummc/emummc.ini.bak");}

if (sd_file_exists ("emummc/emummc.ini"))
{
f_rename("emummc/emummc.ini","emummc/emummc.ini.bak");
}else{
f_rename("emummc/emummc.ini.bak","emummc/emummc.ini");
}
    gui_init_argon_menu();
return 0;
}