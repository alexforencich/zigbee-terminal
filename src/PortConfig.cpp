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
        
        cmbPort_tm = Gtk::ListStore::create(cText);
        cmbPort.set_model(cmbPort_tm);
        
        refresh_ports();
        
        cmbPort.pack_start(cText.name);
        
        cmbPort.set_active(0);
        
        table.attach(cmbPort, 0, 1, 1, 2);
        
        cmbSpeed_tm = Gtk::ListStore::create(cText);
        cmbSpeed.set_model(cmbSpeed_tm);
        
        Gtk::TreeModel::Row row;
        
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "300";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "600";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "1200";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "2400";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "4800";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "9600";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "19200";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "38400";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "57600";
        row = *(cmbSpeed_tm->append());
        row[cText.name] = "115200";
        
        cmbSpeed.pack_start(cText.name);
        
        cmbSpeed.set_active(0);
        
        table.attach(cmbSpeed, 1, 2, 1, 2);
        
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
        
        cmbPort_tm->clear();
        
        for (it = ports.begin(); it != ports.end(); ++it)
        {
                row = *(cmbPort_tm->append());
                row[cText.name] = *it;
                if (ind == -1 && (*it).find("ttyUSB") != std::string::npos)
                        ind = i;
                i++;
        }
        
        if (ind == -1)
                ind = 0;
        
        cmbPort.set_active(ind);
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
        Glib::ustring str;
        
        Gtk::TreeModel::iterator iter = cmbPort.get_active();
        if (iter)
        {
                Gtk::TreeModel::Row row = *iter;
                if (row)
                {
                        port = row[cText.name];
                }
        }
        
        iter = cmbSpeed.get_active();
        if (iter)
        {
                Gtk::TreeModel::Row row = *iter;
                if (row)
                {
                        str = row[cText.name];
                        baud = atol(str.c_str());
                }
        }
        
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
                Glib::ustring str;
                Gtk::TreeModel::iterator iter;
                Gtk::TreeModel::Row row;
                for (iter = cmbPort_tm->children().begin(); iter != cmbPort_tm->children().end(); ++iter)
                {
                        row = *iter;
                        str = row[cText.name];
                        if (str == p)
                        {
                                cmbPort.set_active(iter);
                                return str;
                        }
                }
        }
        return "";
}

unsigned long PortConfig::select_baud(unsigned long b)
{
        if (b > 0)
        {
                Glib::ustring str;
                Gtk::TreeModel::iterator iter;
                Gtk::TreeModel::Row row;
                unsigned long l;
                for (iter = cmbSpeed_tm->children().begin(); iter != cmbSpeed_tm->children().end(); ++iter)
                {
                        row = *iter;
                        str = row[cText.name];
                        l = atol(str.c_str());
                        if (l == b)
                        {
                                cmbSpeed.set_active(iter);
                                return b;
                        }
                }
        }
        return 0;
}



