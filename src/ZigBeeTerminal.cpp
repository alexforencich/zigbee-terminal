/************************************************************************/
/* ZigBeeTerminal                                                       */
/*                                                                      */
/* ZigBee Terminal                                                      */
/*                                                                      */
/* ZigBeeTerminal.cpp                                                   */
/*                                                                      */
/* Alex Forencich <alex@alexforencich.com>                              */
/*                                                                      */
/* Copyright (c) 2011 Alex Forencich                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files(the "Software"), to deal in the Software without restriction,  */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* The above copyright notice and this permission notice shall be       */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  */
/* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   */
/* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    */
/* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     */
/* SOFTWARE.                                                            */
/*                                                                      */
/************************************************************************/

#include "ZigBeeTerminal.h"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

ZigBeeTerminal::ZigBeeTerminal()
{
        set_title("ZigBee Terminal");
        set_position(Gtk::WIN_POS_CENTER);
        
        add(vbox1);
        
        // Main menu
        
        vbox1.pack_start(main_menu, false, true, 0);
        
        file_menu_item.set_label("_File");
        file_menu_item.set_use_underline(true);
        main_menu.append(file_menu_item);
        
        file_menu_item.set_submenu(file_menu);
        
        file_quit_item.set_label(Gtk::Stock::QUIT.id);
        file_quit_item.set_use_stock(true);
        file_quit_item.signal_activate().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_file_quit_item_activate) );
        file_menu.append(file_quit_item);
        
        config_menu_item.set_label("_Config");
        config_menu_item.set_use_underline(true);
        main_menu.append(config_menu_item);
        
        config_menu_item.set_submenu(config_menu);
        
        config_port_item.set_label("Port");
        config_port_item.signal_activate().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_config_port_item_activate) );
        config_menu.append(config_port_item);
        
        config_close_port_item.set_label("Close Port");
        config_close_port_item.signal_activate().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_config_close_port_item_activate) );
        config_menu.append(config_close_port_item);
        
        // status bar
        
        status.push("Not connected");
        vbox1.pack_start(status, false, true, 0);
        
        baud = 115200;
        port = "";
        
        dlgPort.set_port(port);
        dlgPort.set_baud(baud);
        
        ser_int.port_opened().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_open) );
        ser_int.port_closed().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_close) );
        //ser_int.port_error().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_error) );
        ser_int.port_receive_data().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_receive_data) );
        
        show_all_children();
}


ZigBeeTerminal::~ZigBeeTerminal()
{
        
}


void ZigBeeTerminal::on_file_quit_item_activate()
{
        gtk_main_quit();
}


void ZigBeeTerminal::on_config_port_item_activate()
{
        int response;
        
        dlgPort.set_port(port);
        dlgPort.set_baud(baud);
        response = dlgPort.run();
        
        if (response == Gtk::RESPONSE_OK)
        {
                close_port();
                
                port = dlgPort.get_port();
                baud = dlgPort.get_baud();
                
                open_port();
        }
}


void ZigBeeTerminal::on_config_close_port_item_activate()
{
        close_port();
}


void ZigBeeTerminal::on_port_open()
{
        gsize num;
        
        std::cout << "on_port_open()" << std::endl;
        
        port = ser_int.get_port();
        baud = ser_int.get_baud();
        
        status.pop();
        status.push(port + ": " + Glib::ustring::format(baud));
        
        ser_int.write("test", 4, num);
}


void ZigBeeTerminal::on_port_close()
{
        status.pop();
        status.push("Not connected");
}


void ZigBeeTerminal::on_port_receive_data()
{
        gsize num;
        static char buf[64];
        
        ser_int.read(buf, 64, num);
        for (int i = 0; i < num; i++)
                std::cout << buf[i];
        std::cout << std::endl;
}


void ZigBeeTerminal::open_port()
{
        if (ser_int.is_open())
                close_port();
        
        ser_int.set_port(port);
        ser_int.set_baud(baud);
        ser_int.open_port();
}


void ZigBeeTerminal::close_port()
{
        ser_int.close_port();
}



