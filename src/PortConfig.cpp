/************************************************************************/
/* PortConfig                                                           */
/*                                                                      */
/* ZigBee Terminal - Port Configuration                                 */
/*                                                                      */
/* PortConfig.cpp                                                       */
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

#include "PortConfig.h"
#include "SerialInterface.h"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

PortConfig::PortConfig()
{
        set_title("Port Configuration");
        set_border_width(5);
        
        btnOK = add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
        btnOK->signal_clicked().connect( sigc::mem_fun(*this, &PortConfig::on_ok_click) );
        set_default(*btnOK);
        btnCancel = add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        btnCancel->signal_clicked().connect( sigc::mem_fun(*this, &PortConfig::on_cancel_click) );
        
        frame.set_label("Serial Port");
        
        get_vbox()->pack_start(frame, TRUE, TRUE, 0);
        
        table.resize(2, 2);
        table.set_col_spacings(10);
        table.set_row_spacings(5);
        table.set_border_width(5);
        
        frame.add(table);
        
        label1.set_label("Port:");
        table.attach(label1, 0, 1, 0, 1);
        
        label2.set_label("Speed:");
        table.attach(label2, 1, 2, 0, 1);
        
        refresh_ports();
        
        cmbtPort.set_active(0);
        
        table.attach(cmbtPort, 0, 1, 1, 2);
        
        cmbtSpeed.append("300");
        cmbtSpeed.append("600");
        cmbtSpeed.append("1200");
        cmbtSpeed.append("2400");
        cmbtSpeed.append("4800");
        cmbtSpeed.append("9600");
        cmbtSpeed.append("19200");
        cmbtSpeed.append("38400");
        cmbtSpeed.append("57600");
        cmbtSpeed.append("115200");
        
        cmbtSpeed.set_active(0);
        
        table.attach(cmbtSpeed, 1, 2, 1, 2);
        
        show_all_children();
}

PortConfig::~PortConfig()
{
        
}

void PortConfig::refresh_ports()
{
        int ind = -1;
        int i = 0;
        std::vector<std::string> ports = SerialInterface::enumerate_ports();
        std::vector<std::string>::iterator it;
        
        Gtk::TreeModel::Row row;
        
        cmbtPort.remove_all();
        
        for (it = ports.begin(); it != ports.end(); ++it)
        {
                cmbtPort.append(*it);
                
                #ifdef __unix__
                if (ind == -1 && (*it).find("ttyUSB") != std::string::npos)
                        ind = i;
                #endif
                
                i++;
        }
        
        if (ind == -1)
                ind = 0;
        
        cmbtPort.set_active(ind);
        select_port(port);
}

void PortConfig::on_show()
{
        refresh_ports();
        
        select_port(port);
        select_baud(baud);
        
        Gtk::Dialog::on_show();
}

void PortConfig::on_ok_click()
{
        port = cmbtPort.get_active_text();
        
        baud = atol(cmbtSpeed.get_active_text().c_str());
        
        hide();
}

void PortConfig::on_cancel_click()
{
        hide();
}

void PortConfig::set_port(Glib::ustring p)
{
        if (p.length() > 0)
        {
                if (select_port(p) != "")
                {
                        port = p;
                }
        }
}

void PortConfig::set_baud(unsigned long b)
{
        if (b > 0)
        {
                if (select_baud(b) != 0)
                {
                        baud = b;
                }
        }
}

Glib::ustring PortConfig::get_port()
{
        return port;
}

unsigned long PortConfig::get_baud()
{
        return baud;
}

Glib::ustring PortConfig::select_port(Glib::ustring p)
{
        if (p.length() > 0)
        {
                cmbtPort.set_active_text(p);
                return p;
        }
        return "";
}

unsigned long PortConfig::select_baud(unsigned long b)
{
        if (b > 0)
        {
                cmbtSpeed.set_active_text(Glib::ustring::format(b));
                return b;
        }
        return 0;
}



