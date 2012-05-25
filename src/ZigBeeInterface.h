/************************************************************************/
/* ZigBeeInterface                                                      */
/*                                                                      */
/* ZigBee Terminal - ZigBee Interface                                   */
/*                                                                      */
/* ZigBeeInterface.h                                                    */
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

#ifndef __ZIGBEE_INTERFACE_H
#define __ZIGBEE_INTERFACE_H

#include <gtkmm.h>

#include "ZigBeePacket.h"
#include "SerialInterface.h"

#include <string>
#include <tr1/memory>
#include <vector>
#include <deque>
#include <inttypes.h>

/** ZigBee Interface
 * 
 * The ZigBee interface class is used to manage transmission and reception
 * of ZigBee packets through a serial interface to a ZigBee module.
 */
class ZigBeeInterface
{
public:
        /**
         * Create a ZigBee Interface.
         */
        ZigBeeInterface();
        virtual ~ZigBeeInterface();
        
        /**
         * Connect a serial interface
         * @param si shared_ptr to the serial interface object to associate.
         * with
         * @see clear_serial_interface()
         * @see has_serial_interface()
         */
        void set_serial_interface(std::tr1::shared_ptr<SerialInterface> si);
        
        /**
         * Disconnect from currently connected serial interface.
         * @see set_serial_interface()
         * @see has_serial_interface()
         */
        void clear_serial_interface();
        
        /**
         * Check if serial interface is associated.
         * @return true if associated to an interface, false if not
         * @see set_serial_interface()
         * @see clear_serial_interface()
         */
        bool has_serial_interface();
        
        /**
         * Check if connected.
         * @return true if associated to an interface and the interface is
         * connected
         * @see has_serial_interface()
         */
        bool is_connected();
        
        /**
         * Clear receive buffer.
         * @see read_data_queue
         */
        void reset_buffer();
        
        /**
         * Transmit a packet.
         * @param pkt packet to transmit
         */
        void send_packet(ZigBeePacket pkt);
        
        /**
         * Set debug status.  If debug mode is enabled, received byte counts
         * will be printed to stdout.  
         * @return debug state
         * @see debug
         * @see get_debug()
         */
        bool set_debug(bool d);
        
        /**
         * Get debug status.
         * @return debug state
         * @see debug
         * @see set_debug()
         */
        bool get_debug();
        
        /**
         * Receive packet signal. 
         * @par Prototype:
         * <tt>void on_my_%receive_pacekt(ZigBeePacket pkt)</tt>
         */
        sigc::signal<void, ZigBeePacket> signal_receive_packet();
        
        /**
         * Send raw data signal. 
         * @par Prototype:
         * <tt>void on_my_%send_raw_data(const char *data, size_t n)</tt>
         */
        sigc::signal<void, const char*, size_t> signal_send_raw_data();
        
        /**
         * Receive raw data signal. 
         * @par Prototype:
         * <tt>void on_my_%receive_raw_data(const char *data, size_t n)</tt>
         */
        sigc::signal<void, const char*, size_t> signal_receive_raw_data();
        
        /**
         * Error signal. 
         * @par Prototype:
         * <tt>void on_my_%error()</tt>
         */
        sigc::signal<void> signal_error();
        
protected:
        /**
         * Serial interface receive data event handler.
         */
        void on_receive_data();
        
        /**
         * Shared pointer to serial interface instance.
         * @see set_serial_interface()
         * @see clear_serial_interface()
         * @see has_serial_interface()
         */
        std::tr1::shared_ptr<SerialInterface> ser_int;
        
        /**
         * Double ended read data queue.
         */
        std::deque<char> read_data_queue;
        
        /**
         * Debug mode.
         * @see set_debug()
         * @see get_debug()
         */
        bool debug;
        
        /**
         * Receive packet signal.
         */
        sigc::signal<void, ZigBeePacket> m_signal_receive_packet;
        
        /**
         * Send raw data signal.
         */
        sigc::signal<void, const char*, size_t> m_signal_send_raw_data;
        
        /**
         * Receive raw data signal.
         */
        sigc::signal<void, const char*, size_t> m_signal_receive_raw_data;
        
        /**
         * Error signal.
         */
        sigc::signal<void> m_signal_error;
        
        /**
         * Serial interface port opened signal connection.
         */
        sigc::connection c_port_opened;
        
        /**
         * Serial interface port closed signal connection.
         */
        sigc::connection c_port_closed;
        
        /**
         * Serial interface port receive data signal connection.
         */
        sigc::connection c_port_receive_data;
};

#endif //__ZIGBEE_INTERFACE_H
