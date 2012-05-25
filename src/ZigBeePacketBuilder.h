/************************************************************************/
/* ZigBeePacketBuilder                                                  */
/*                                                                      */
/* ZigBee Terminal - ZigBee Packet Builder                              */
/*                                                                      */
/* ZigBeePacketBuilder.h                                                */
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

#ifndef __ZIGBEE_PACKET_BUILDER_H
#define __ZIGBEE_PACKET_BUILDER_H

#include <gtkmm.h>
#include <tr1/memory>

#include "ZigBeePacket.h"

// ZigBeePacketBuilder class
/** ZigBee Packet Builder
 * 
 * The ZigBee Packet Builder is a specialized widget for for creating ZigBee
 * packets for Digi XBee radios.  The packet builder will configure itself for
 * the selected packet type.  
 */
class ZigBeePacketBuilder : public Gtk::VBox
{
public:
        /**
         * Create a ZigBee Packet Builder.
         */
        ZigBeePacketBuilder();
        virtual ~ZigBeePacketBuilder();
        
        /**
         * Reads the associated packet and rebuilds the UI accordingly.
         * @see pkt
         */
        void read_packet();
        
        /**
         * Load packet p and update UI.
         * @param p packet to read
         * @see pkt
         * @see read_packet()
         */
        void set_packet(ZigBeePacket p);
        
        /**
         * Get built packet
         * @return built ZigBee packet.
         * @see pkt
         */
        ZigBeePacket get_packet();
        
        /**
         * Packet changed signal.
         * @par Prototype:
         * <tt>void on_my_%changed()</tt>
         */
        sigc::signal<void> signal_changed();
        
protected:
        //Signal handlers:
        /**
         * Type combo change event.
         */
        void on_type_change();
        
        /**
         * Field change signal.
         * @param offset field offset in packet
         * @param index widget index in field list
         */
        void on_field_change(int offset, int index);
        
        /**
         * Data change signal.
         */
        void on_data_change();
        
        /**
         * Hex data checkbox toggle signal.
         */
        void on_hex_data_toggle();
        
        /**
         * Build packet and emit changed signal.
         * @see pkt
         * @see signal_changed()
         */
        void update_packet();
        
        /**
         * Update data text box with data from packet.
         */
        void update_data();
        
        /**
         * Parse hex or decimal number from string.
         * @param str string containing decimal or hexidecimal integer
         * @return value of integer
         */
        unsigned long parse_number(Glib::ustring str);
        
        //Child widgets:
        /**
         * Table widget for packet fields.
         */
        Gtk::Table tbl;
        
        /**
         * Packet type combo label.
         */
        Gtk::Label label_type;
        
        /**
         * Packet type combo box.
         */
        Gtk::ComboBoxText cmbt_type;
        
        /**
         * Identifier label.
         */
        Gtk::Label label_identifier;
        
        /**
         * Identifier text box.
         */
        Gtk::Entry ent_identifier;
        
        /**
         * Scrolled window for data text view.
         */
        Gtk::ScrolledWindow sw_data;
        
        /**
         * Data text view.
         */
        Gtk::TextView tv_data;
        
        /**
         * Alignment for hex data check box.
         */
        Gtk::Alignment al_hex_data;
        
        /**
         * Hex data check box.
         */
        Gtk::CheckButton hex_data;
        
        /**
         * List of field label references.
         */
        std::vector< std::tr1::shared_ptr< Gtk::Label > > labels;
        
        /**
         * List of field text boxes.
         */
        std::vector< std::tr1::shared_ptr< Gtk::Entry > > fields;
        
        /**
         * List of packet identifier values in combo box.
         */
        std::vector<int> identifier_list;
        
        /**
         * Currently selected identifier.
         */
        int current_identifier;
        
        /**
         * Updating fields flag to prevent infinite loops.
         */
        bool updating_fields;
        
        /**
         * Local packet for field updates.
         * @see update_packet()
         * @see set_packet()
         */
        ZigBeePacket pkt;
        
        /**
         * Changed signal.
         */
        sigc::signal<void> m_signal_changed;
        
};

#endif //__ZIGBEE_PACKET_BUILDER_H
