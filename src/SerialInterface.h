/************************************************************************/
/* SerialInterface                                                      */
/*                                                                      */
/* ZigBee Terminal - Serial Port Interface                              */
/*                                                                      */
/* SerialInterface.h                                                    */
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

#ifndef __SERIALINTERFACE_H
#define __SERIALINTERFACE_H

#include <string>
#include <vector>
#include <gtkmm.h>

#ifdef __unix__
#include <termios.h>
#elif defined _WIN32
#include <windows.h>
#endif

/** Serial Interface
 * 
 * Cross-platform serial interface module.  Tested on windows and linux.  
 */
class SerialInterface
{
public:
        /**
         * Port status.
         */
        typedef enum
        {
                SS_Success = 0,
                SS_Error = 1,
                SS_Timeout = 2,
                SS_EOF = 3,
                SS_PortNotOpen = 4,
        }
        SerialStatus;
        
        /**
         * Flow control.
         */
        typedef enum
        {
                SF_None = 0,
                SF_Hardware = 1,
                SF_XonXoff = 2,
        }
        SerialFlow;
        
        /**
         * Parity.
         */
        typedef enum
        {
                SP_None = 0,
                SP_Odd = 1,
                SP_Even = 2,
        }
        SerialParity;
        
        /**
         * Create a Serial Interface.
         */
        SerialInterface();
        virtual ~SerialInterface();
        
        /**
         * Write data.
         * @param buf pointer to data
         * @param count number of bytes to send
         * @param bytes_written return number of bytes written
         * @return status
         */
        SerialStatus write(const char *buf, gsize count, gsize& bytes_written);
        
        /**
         * Read data.
         * @param buf pointer to data
         * @param count number of bytes to send
         * @param bytes_read return number of bytes read
         * @return status
         */
        SerialStatus read(char *buf, gsize count, gsize& bytes_read);
        
        /**
         * Open port.
         * @return status
         */
        SerialStatus open_port();
        
        /**
         * Close port.
         * @return status
         */
        SerialStatus close_port();
        
        /**
         * Check port open.
         * @return true if open
         */
        bool is_open();
        
        /**
         * Set serial port.
         * @param p port
         * @return port
         */
        Glib::ustring set_port(Glib::ustring p);
        
        /**
         * Get serial port.
         * @return port
         */
        Glib::ustring get_port();
        
        /**
         * Set baud rate.
         * @param b baud rate
         * @return baud rate
         */
        unsigned long set_baud(unsigned long b);
        
        /**
         * Get baud rate.
         * @return baud rate
         */
        unsigned long get_baud();
        
        /**
         * Set number of bits.
         * @param b bits
         * @return bits
         */
        int set_bits(int b);
        
        /**
         * Get number of bits.
         * @return bits
         */
        int get_bits();
        
        /**
         * Set flow control.
         * @param f flow control
         * @return flow control
         */
        SerialFlow set_flow(SerialFlow f);
        
        /**
         * Get flow control.
         * @return flow control
         */
        SerialFlow get_flow();
        
        /**
         * Set parity.
         * @param p parity
         * @return parity
         */
        SerialParity set_parity(SerialParity p);
        
        /**
         * Get parity.
         * @return parity
         */
        SerialParity get_parity();
        
        /**
         * Set stop bit count.
         * @param s bits
         * @return bits
         */
        int set_stop(int s);
        
        /**
         * Get stop bit count.
         * @return bits
         */
        int get_stop();
        
        /**
         * Set debug mode.  If debug mode is enabled, all bytes read and
         * written are printed in hex to stdout.
         * @param d debug mode
         * @return debug mode
         */
        bool set_debug(bool d);
        
        /**
         * Get debug mode.  If debug mode is enabled, all bytes read and
         * written are printed in hex to stdout.
         * @return debug mode
         */
        bool get_debug();
        
        /**
         * Get status string.  Returns a short representation of the
         * connection configuration.  
         * @return status string
         */
        Glib::ustring get_status_string();
        
        /**
         * Enumerate serial ports.  Returns a vector of strings with the
         * names of likely usable ports.
         * @return list of ports
         */
        static std::vector<std::string> enumerate_ports();
        
        /**
         * Port opened signal.  
         * @par Prototype:
         * <tt>void on_my_%port_opened()</tt>
         */
        sigc::signal<void> port_opened();
        
        /**
         * Port closed signal.  
         * @par Prototype:
         * <tt>void on_my_%port_closed()</tt>
         */
        sigc::signal<void> port_closed();
        
        /**
         * Port error signal.  
         * @par Prototype:
         * <tt>void on_my_%port_error()</tt>
         */
        sigc::signal<void> port_error();
        
        /**
         * Port receive_data signal.  
         * @par Prototype:
         * <tt>void on_my_%port_receive_data()</tt>
         */
        sigc::signal<void> port_receive_data();
        
protected:
        /**
         * Select thread receive data event.  
         * @see select_thread()
         */
        void on_receive_data();
        
        /**
         * Select thread error event.  
         * @see select_thread()
         */
        void on_error();
        
        /**
         * Select thread for monitoring serial port.
         * @see launch_select_thread()
         * @see stop_select_thread()
         * @see signal_receive_data
         * @see signal_error
         * @see on_receive_data()
         * @see on_error()
         */
        void select_thread();
        
        /**
         * Start select thread.  
         * @see select_thread()
         * @see stop_select_thread()
         */
        void launch_select_thread();
        
        /**
         * Stop select thread.
         * @see select_thread()
         * @see launch_select_thread()
         */
        void stop_select_thread();
        
        /**
         * Configure serial port.
         */
        SerialStatus configure_port();
        
        /**
         * Receive data signal dispatcher
         * @see select_thread()
         * @see on_receive_data()
         */
        Glib::Dispatcher signal_receive_data;
        
        /**
         * Error signal dispatcher
         * @see select_thread()
         * @see on_error()
         */
        Glib::Dispatcher signal_error;
        
        #ifdef __unix__
        
        int port_fd;
        struct termios port_termios;
        struct termios port_termios_saved;
        
        #elif defined _WIN32
        
        HANDLE h_port;
        DCB dcb_serial_params;
        DCB dcb_serial_params_saved;
        HANDLE h_overlapped;
        HANDLE h_overlapped_thread;
        
        #endif
        
        /**
         * Running mutex
         * @see select_thread()
         */
        Glib::Mutex running_mutex;
        
        /**
         * Read mutex
         * @see read_cond
         * @see select_thread()
         */
        Glib::Mutex read_mutex;
        
        /**
         * Read condition
         * @see read_mutex
         * @see select_thread()
         */
        Glib::Cond read_cond;
        
        /**
         * Pointer for select thread
         * @see select_thread()
         */
        Glib::Thread *thread;
        
        /**
         * Thread running indicator
         * @see select_thread()
         */
        bool running;
        
        /**
         * Port.
         */
        Glib::ustring port;
        
        /**
         * Baud rate.
         */
        unsigned long baud;
        
        /**
         * Data bits.
         */
        int bits;
        
        /**
         * Flow control setting.
         */
        SerialFlow flow;
        
        /**
         * Parity setting.
         */
        SerialParity parity;
        
        /**
         * Stop bit count.
         */
        int stop;
        
        /**
         * Recieve data synchronization.  Needed to prevent closing the port
         * due to an error when in on_receive_data.
         */
        bool in_on_receive_data;
        
        /**
         * Close port called.  Needed to prevent closing the port due to an
         * error when in on_receive_data.
         */
        bool called_close_port;
        
        /**
         * Debug status.
         */
        bool debug;
        
        /**
         * Port opened signal.
         */
        sigc::signal<void> m_port_opened;
        
        /**
         * Port closed signal.
         */
        sigc::signal<void> m_port_closed;
        
        /**
         * Port error signal.
         */
        sigc::signal<void> m_port_error;
        
        /**
         * Port receive data signal.
         */
        sigc::signal<void> m_port_receive_data;
};

#endif //__SERIALINTERFACE_H


