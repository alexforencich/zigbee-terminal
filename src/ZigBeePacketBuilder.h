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
class ZigBeePacketBuilder : public Gtk::VBox
{
public:
        ZigBeePacketBuilder();
        virtual ~ZigBeePacketBuilder();
        
        void read_packet();
        
        void set_packet(ZigBeePacket p);
        ZigBeePacket get_packet();
        
protected:
        //Signal handlers:
        void on_type_change();
        
        void on_field_change(int offset, int index);
        void on_data_change();
        void on_hex_data_toggle();
        
        void update_data();
        
        unsigned long parse_number(Glib::ustring str);
        
        //Child widgets:
        Gtk::Table tbl;
        Gtk::Label label_type;
        Gtk::ComboBoxText cmbt_type;
        Gtk::Label label_identifier;
        Gtk::Entry ent_identifier;
        Gtk::ScrolledWindow sw_data;
        Gtk::TextView tv_data;
        Gtk::Alignment al_hex_data;
        Gtk::CheckButton hex_data;
        
        std::vector< std::tr1::shared_ptr< Gtk::Label > > labels;
        std::vector< std::tr1::shared_ptr< Gtk::Entry > > fields;
        
        std::vector<int> identifier_list;
        
        int current_identifier;
        
        bool updating_fields;
        
        ZigBeePacket pkt;
        
};

#endif //__ZIGBEE_PACKET_BUILDER_H
