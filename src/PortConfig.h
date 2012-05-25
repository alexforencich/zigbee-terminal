/************************************************************************/
/* PortConfig                                                           */
/*                                                                      */
/* ZigBee Terminal - Port Configuration                                 */
/*                                                                      */
/* PortConfig.h                                                         */
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

#ifndef __PORTCONFIG_H
#define __PORTCONFIG_H

#include "SerialInterface.h"

#include <string>

#include <gtkmm.h>

/** Port Config Dialog
 * 
 * Serial port configuration dialog.  Allows for easy selection of serial
 * interface parameters.  
 */
class PortConfig : public Gtk::Dialog
{
public:
        /**
         * Create a new Port Config dialog.
         */
        PortConfig();
        virtual ~PortConfig();
        
        /**
         * Update port list.
         */
        void refresh_ports();
        
        /**
         * Set serial port.
         * @param p port
         */
        void set_port(Glib::ustring p);
        
        /**
         * Get serial port.
         * @return port
         */
        Glib::ustring get_port();
        
        /**
         * Set baud rate.
         * @param b baud rate
         */
        void set_baud(unsigned long b);
        
        /**
         * Get baud rate.
         * @return baud rate
         */
        unsigned long get_baud();
        
        /**
         * Set number of bits.
         * @param b bits
         */
        void set_bits(int b);
        
        /**
         * Get number of bits.
         * @return bits
         */
        int get_bits();
        
        /**
         * Set flow control.
         * @param f flow control
         */
        void set_flow_control(SerialInterface::SerialFlow f);
        
        /**
         * Get flow control.
         * @return flow control
         */
        SerialInterface::SerialFlow get_flow_control();
        
        /**
         * Set parity.
         * @param p parity
         */
        void set_parity(SerialInterface::SerialParity p);
        
        /**
         * Get parity.
         * @return parity
         */
        SerialInterface::SerialParity get_parity();
        
        /**
         * Set stop bit count.
         * @param s bits
         */
        void set_stop_bits(int s);
        
        /**
         * Get stop bit count.
         * @return bits
         */
        int get_stop_bits();
        
protected:
        //Signal handlers:
        
        /**
         * Show signal handler
         */
        virtual void on_show();
        
        /**
         * OK button click signal handler
         */
        void on_ok_click();
        
        /**
         * Cancel button click signal handler
         */
        void on_cancel_click();
        
        /**
         * Select port in combo box
         * @param p port
         * @return port
         */
        Glib::ustring select_port(Glib::ustring p);
        
        /**
         * Select baud rate in combo box
         * @param b baud rate
         * @return baud rate
         */
        unsigned long select_baud(unsigned long b);
        
        /**
         * Select parity setting in combo box
         * @param p serial parity setting
         * @return serial parity setting
         */
        SerialInterface::SerialParity select_parity(SerialInterface::SerialParity p);
        
        /**
         * Select data bit count setting in combo box
         * @param b data bit count
         * @return data bit count
         */
        int select_bits(int b);
        
        /**
         * Select stop bit count setting in combo box
         * @param sb stop bit count
         * @return stop bit count
         */
        int select_stop_bits(int sb);
        
        /**
         * Select flow control setting in combo box
         * @param f flow control setting
         * @return flow control setting
         */
        SerialInterface::SerialFlow select_flow_control(SerialInterface::SerialFlow f);
        
        
        //Child widgets:
        Gtk::Button *btnOK;
        Gtk::Button *btnCancel;
        Gtk::Frame frame;
        Gtk::Table table;
        Gtk::Label label1;
        Gtk::Label label2;
        Gtk::Label label3;
        Gtk::Label label4;
        Gtk::Label label5;
        Gtk::Label label6;
        Gtk::ComboBoxText cmbtPort;
        Gtk::ComboBoxText cmbtSpeed;
        Gtk::ComboBoxText cmbtParity;
        Gtk::ComboBoxText cmbtBits;
        Gtk::ComboBoxText cmbtStopBits;
        Gtk::ComboBoxText cmbtFlowControl;
        
        Glib::ustring port;
        unsigned long baud;
        SerialInterface::SerialParity parity;
        int bits;
        int stop_bits;
        SerialInterface::SerialFlow flow_control;
};

// Prototypes


#endif //__PORTCONFIG_H
