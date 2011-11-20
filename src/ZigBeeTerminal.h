/************************************************************************/
/* ZigBeeTerminal                                                       */
/*                                                                      */
/* ZigBee Terminal                                                      */
/*                                                                      */
/* ZigBeeTerminal.h                                                     */
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

#ifndef __ZIGBEE_TERMINAL_H
#define __ZIGBEE_TERMINAL_H

#include <gtkmm.h>

#include "PortConfig.h"
#include "SerialInterface.h"

// Template class
class ZigBeeTerminal : public Gtk::Window
{
public:
        ZigBeeTerminal();
        virtual ~ZigBeeTerminal();
        
protected:
        //Signal handlers:
        void on_file_quit_item_activate();
        void on_config_port_item_activate();
        void on_config_close_port_item_activate();
        
        void on_port_open();
        void on_port_close();
        void on_port_receive_data();
        
        void open_port();
        void close_port();
        
        //Child widgets:
        // window
        Gtk::VBox vbox1;
        // menu bar
        Gtk::MenuBar main_menu;
        Gtk::MenuItem file_menu_item;
        Gtk::Menu file_menu;
        Gtk::ImageMenuItem file_quit_item;
        Gtk::MenuItem config_menu_item;
        Gtk::Menu config_menu;
        Gtk::ImageMenuItem config_port_item;
        Gtk::ImageMenuItem config_close_port_item;
        // status bar
        Gtk::Statusbar status;
        
        PortConfig dlgPort;
        
        Glib::ustring port;
        unsigned long baud;
        
        SerialInterface ser_int;
        
};

#endif //__ZIGBEE_TERMINAL_H
