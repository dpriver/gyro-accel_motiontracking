/*******************************************************************************
 *	test_lcd_1602A.h
 *
 *  1602A LCD peripheral test
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

#include <util/delay.h>

#include <uc/system.h>
#include <uc/usart.h>
#include <boards/arduinoUNO.h>
#include <peripherals/1602A.h>
#include <systick.h>


int main ( void ) {
  
    system_init();
    systick_init();
    usart_init();
    
    usart_print("Init LCD...");
    
    /**** Initialization ****/
    lcd_1602a_init();
    usart_print("OK\n");
    
    
    //0100 0001 
    //0010 1  000
    //     D3
 
 
    lcd_1602a_print("  Hello World ", LCD_ROW1_POS(0));
    _delay_ms(3000);
    lcd_1602a_print("    Works!!", LCD_ROW2_POS(0));

 
    usart_print("Values printed\n");
    
    while(1);
    


   return 0; 
}
