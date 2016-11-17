/*******************************************************************************
 *	1602A.c
 *
 *  LCD 1602a peripheral
 *
 *
 *  This file is part of aavrlib
 * 
 *  Copyright (C) 2015  Dennis Pinto Rivero
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/


/**
 * @file 1602A.h
 * @brief 1602A lcd display functionality
 * 
 * The 1602A lcd display includes an internal controller, whose registers can 
 * be accessed thought the on-board pins in a parallel fashion.
 * 
 * This parallel port can be configured to work in full or half word, being 
 * the last, the option used by this library. The used board pins can be found 
 * in the config.h file.
 */

#ifndef __LCD_1602A
#define __LCD_1602A





#define LCD_ROW1_POS(pos) (0x0 + (pos))
#define LCD_ROW2_POS(pos) (0x40 + (pos))


/* LCD 1602A COMMANDS */
#define LCD_1602A_CLEAR() lcd_1602a_command(0x1)  //!< Clear LCD screen
#define LCD_1602A_HOME()  lcd_1602a_command(0x2)  //!< Return cursor to home position
#define LCD_1602A_MOVE_LEFT() lcd_1602a_command(0x10)  //!< Move cursor 1 position to the left
#define LCD_1602A_MOVE_RIGHT() lcd_1602a_command(0x14) //!< Move cursor 1 position to the right
#define LCD_1602A_SHIFT_LEFT() lcd_1602a_command(0x18)  //!< Shift all the display characters to the left
#define LCD_1602A_SHIFT_RIGHT() lcd_1602a_command(0x1c) //!< Shift all the display characters to the right
#define LCD_1602A_SET_TARGET_INT_MEM(addr) lcd_1602a_command(0x40 | (addr))  //!< Set the target to be the internal memory
#define LCD_1602A_SET_TARGET_DISP_MEM(addr) lcd_1602a_command(0x80 | (addr)) //!< Set the target to be the display memory
#define LCD_1602A_WRITE_CHAR(c) lcd_1602a_command(0x200 | (c)) //!< Write a char into the current target memory



#define LCD_READ_STATUS()         //!< Read the lcd status 0x1100
#define LCD_READ_RAM()            //!< Read a byte from the current target memory 0x300


void lcd_1602a_init();
uint8_t lcd_1602a_command(uint16_t command);
void lcd_1602a_print(char *string, uint8_t position);

#endif /* __LCD_1602A */
