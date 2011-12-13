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

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

PortConfig::PortConfig() :
        cmbtPort(true),
        cmbtSpeed(true)
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
        
        table.resize(4, 3);
        table.set_col_spacings(10);
        table.set_row_spacings(5);
        table.set_border_width(5);
        
        frame.add(table);
        
        label1.set_label("Port:");
        table.attach(label1, 0, 1, 0, 1);
        
        refresh_ports();
        
        table.attach(cmbtPort, 0, 1, 1, 2);
        
        label2.set_label("Speed:");
        table.attach(label2, 1, 2, 0, 1);
        
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
        
        label3.set_label("Parity:");
        table.attach(label3, 2, 3, 0, 1);
        
        cmbtParity.append("none");
        cmbtParity.append("odd");
        cmbtParity.append("even");
        
        cmbtParity.set_active(0);
        
        table.attach(cmbtParity, 2, 3, 1, 2);
        
        label4.set_label("Bits:");
        table.attach(label4, 0, 1, 2, 3);
        
        cmbtBits.append("5");
        cmbtBits.append("6");
        cmbtBits.append("7");
        cmbtBits.append("8");
        
        cmbtBits.set_active(3);
        
        table.attach(cmbtBits, 0, 1, 3, 4);
        
        label5.set_label("Stop Bits:");
        table.attach(label5, 1, 2, 2, 3);
        
        cmbtStopBits.append("1");
        cmbtStopBits.append("2");
        
        cmbtStopBits.set_active(0);
        
        table.attach(cmbtStopBits, 1, 2, 3, 4);
        
        label6.set_label("Flow Control:");
        table.attach(label6, 2, 3, 2, 3);
        
        cmbtFlowControl.append("none");
        cmbtFlowControl.append("hardware (RTS/CTS)");
        cmbtFlowControl.append("software (Xon/Xoff)");
        
        cmbtFlowControl.set_active(0);
        
        table.attach(cmbtFlowControl, 2, 3, 3, 4);
        
        port = cmbtPort.get_entry_text();
        baud = 115200;
        parity = SerialInterface::SP_None;
        bits = 8;
        stop_bits = 1;
        flow_control = SerialInterface::SF_None;
        
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
        
        if (port == "" && ind > 0)
                port = ports[ind];
        
        select_port(port);
}

void PortConfig::on_show()
{
        refresh_ports();
        
        select_port(port);
        select_baud(baud);
        select_parity(parity);
        select_bits(bits);
        select_stop_bits(stop_bits);
        select_flow_control(flow_control);
        
        Gtk::Dialog::on_show();
}

void PortConfig::on_ok_click()
{
        port = cmbtPort.get_entry_text();
        baud = atol(cmbtSpeed.get_entry_text().c_str());
        
        switch (cmbtParity.get_active_row_number())
        {
                case 0:
                        parity = SerialInterface::SP_None;
                        break;
                case 1:
                        parity = SerialInterface::SP_Even;
                        break;
                case 2:
                        parity = SerialInterface::SP_Odd;
                        break;
        }
        
        switch (cmbtBits.get_active_row_number())
        {
                case 0:
                        bits = 5;
                        break;
                case 1:
                        bits = 6;
                        break;
                case 2:
                        bits = 7;
                        break;
                case 3:
                        bits = 8;
                        break;
        }
        
        switch (cmbtStopBits.get_active_row_number())
        {
                case 0:
                        stop_bits = 1;
                        break;
                case 1:
                        stop_bits = 2;
                        break;
        }
        
        switch (cmbtFlowControl.get_active_row_number())
        {
                case 0:
                        flow_control = SerialInterface::SF_None;
                        break;
                case 1:
                        flow_control = SerialInterface::SF_Hardware;
                        break;
                case 2:
                        flow_control = SerialInterface::SF_XonXoff;
                        break;
        }
        
        hide();
}

void PortConfig::on_cancel_click()
{
        hide();
}

void PortConfig::set_port(Glib::ustring p)
{
        port = select_port(p);
}

void PortConfig::set_baud(unsigned long b)
{
        baud = select_baud(b);
}

void PortConfig::set_parity(SerialInterface::SerialParity p)
{
        parity = select_parity(p);
}

void PortConfig::set_bits(int b)
{
        bits = select_bits(b);
}

void PortConfig::set_stop_bits(int sb)
{
        stop_bits = select_stop_bits(sb);
}

void PortConfig::set_flow_control(SerialInterface::SerialFlow f)
{
        flow_control = select_flow_control(f);
}

Glib::ustring PortConfig::get_port()
{
        return port;
}

unsigned long PortConfig::get_baud()
{
        return baud;
}

SerialInterface::SerialParity PortConfig::get_parity()
{
        return parity;
}

int PortConfig::get_bits()
{
        return bits;
}

int PortConfig::get_stop_bits()
{
        return stop_bits;
}

SerialInterface::SerialFlow PortConfig::get_flow_control()
{
        return flow_control;
}

Glib::ustring PortConfig::select_port(Glib::ustring p)
{
        if (p.length() > 0)
        {
                cmbtPort.set_active_text(p);
                return p;
        }
        return port;
}

unsigned long PortConfig::select_baud(unsigned long b)
{
        if (b > 0)
        {
                cmbtSpeed.set_active_text(Glib::ustring::format(b));
                return b;
        }
        return baud;
}

SerialInterface::SerialParity PortConfig::select_parity(SerialInterface::SerialParity p)
{
        switch (p)
        {
                case SerialInterface::SP_None:
                        cmbtParity.set_active(0);
                        return p;
                case SerialInterface::SP_Even:
                        cmbtParity.set_active(1);
                        return p;
                case SerialInterface::SP_Odd:
                        cmbtParity.set_active(2);
                        return p;
        }
        
        return parity;
}

int PortConfig::select_bits(int b)
{
        switch (b)
        {
                case 5:
                        cmbtBits.set_active(0);
                        return b;
                case 6:
                        cmbtBits.set_active(1);
                        return b;
                case 7:
                        cmbtBits.set_active(2);
                        return b;
                case 8:
                        cmbtBits.set_active(3);
                        return b;
        }
        
        return bits;
}

int PortConfig::select_stop_bits(int sb)
{
        switch (sb)
        {
                case 1:
                        cmbtStopBits.set_active(0);
                        return sb;
                case 2:
                        cmbtStopBits.set_active(1);
                        return sb;
        }
        
        return stop_bits;
}

SerialInterface::SerialFlow PortConfig::select_flow_control(SerialInterface::SerialFlow f)
{
        switch (f)
        {
                case SerialInterface::SF_None:
                        cmbtFlowControl.set_active(0);
                        return f;
                case SerialInterface::SF_Hardware:
                        cmbtFlowControl.set_active(1);
                        return f;
                case SerialInterface::SF_XonXoff:
                        cmbtFlowControl.set_active(2);
                        return f;
        }
        
        return flow_control;
}



