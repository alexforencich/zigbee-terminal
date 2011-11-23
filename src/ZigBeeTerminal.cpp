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
        
        config_menu.append(config_sep1);
        
        config_local_echo.set_label("Local Echo");
        config_menu.append(config_local_echo);
        
        config_api_mode.set_label("Use API Mode");
        config_menu.append(config_api_mode);
        
        // Tabs
        note.set_border_width(5);
        vbox1.pack_start(note, true, true, 0);
        
        // Terminal tab
        note.append_page(vbox_term, "Terminal");
        
        tv_term.set_size_request(400,200);
        tv_term.modify_font(Pango::FontDescription("monospace"));
        tv_term.set_editable(false);
        tv_term.signal_key_press_event().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_tv_key_press), false );
        
        sw_term.add(tv_term);
        vbox_term.pack_start(sw_term, true, true, 0);
        
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


bool ZigBeeTerminal::on_tv_key_press(GdkEventKey *key)
{
        guint u = gdk_keyval_to_unicode(key->keyval);
        Glib::ustring str = "";
        gsize num;
        
        if (u > 0)
        {
                str = Glib::ustring(1, u);
        }
        else
        {
                switch (key->keyval)
                {
                        case GDK_Return:
                                str = "\n";
                                break;
                }
        }
        
        if (str.length() > 0)
        {
                if (ser_int.is_open())
                {
                        ser_int.write(str.c_str(), 1, num);
                }
        }
}


void ZigBeeTerminal::on_port_open()
{
        gsize num;
        
        std::cout << "on_port_open()" << std::endl;
        
        port = ser_int.get_port();
        baud = ser_int.get_baud();
        
        status.pop();
        status.push(ser_int.get_status_string());
}


void ZigBeeTerminal::on_port_close()
{
        status.pop();
        status.push(ser_int.get_status_string());
}


void ZigBeeTerminal::on_port_receive_data()
{
        gsize num;
        int status;
        static char buf[1024];
        
        status = ser_int.read(buf, 1024, num);
        
        if (status == SI_ERROR)
        {
                std::cout << "Read error!" << std::endl;
                return;
        }
        
        Glib::ustring str;
        for (int i = 0; i < num; i++)
        {
                if (buf[i] != 0)
                        str += buf[i];
        }
        
        Glib::RefPtr<Gtk::TextBuffer> buffer = tv_term.get_buffer();
        buffer->insert(buffer->end(), str);
        
        Glib::RefPtr<Gtk::TextMark> end_mark = buffer->create_mark (buffer->end()); 
        tv_term.scroll_to(end_mark);
        buffer->delete_mark(end_mark);
        
        std::cout << "Read " << num << " bytes: ";
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



