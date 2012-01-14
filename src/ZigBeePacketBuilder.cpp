/************************************************************************/
/* ZigBeePacketBuilder                                                  */
/*                                                                      */
/* ZigBee Terminal - ZigBee Packet Builder                              */
/*                                                                      */
/* ZigBeePacketBuilder.cpp                                              */
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

#include "ZigBeePacketBuilder.h"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std::tr1;

ZigBeePacketBuilder::ZigBeePacketBuilder()
{
        identifier_list = ZigBeePacket::get_valid_identifiers();
        
        current_identifier = -1;
        
        updating_fields = false;
        
        tbl.resize(2, 2);
        tbl.set_col_spacings(10);
        tbl.set_row_spacings(5);
        pack_start(tbl, false, false, 0);
        
        label_type.set_label("Type:");
        label_type.set_size_request(120, -1);
        tbl.attach(label_type, 0, 1, 0, 1);
        
        for (int i = 0; i < identifier_list.size(); i++)
        {
                std::stringstream ss;
                ss << ZigBeePacket::get_type_desc(identifier_list[i]);
                ss << " (0x" << std::setfill('0') << std::setw(2) << std::hex << identifier_list[i] << ")";
                cmbt_type.append_text(ss.str());
        }
        
        cmbt_type.signal_changed().connect( sigc::mem_fun(*this, &ZigBeePacketBuilder::on_type_change) );
        cmbt_type.set_active(0);
        
        tbl.attach(cmbt_type, 1, 2, 0, 1);
        
        label_identifier.set_label("Identifier:");
        tbl.attach(label_identifier, 0, 1, 1, 2);
        
        ent_identifier.set_editable(false);
        tbl.attach(ent_identifier, 1, 2, 1, 2);
        
        tv_data.set_wrap_mode(Gtk::WRAP_WORD_CHAR);
        tv_data.get_buffer()->signal_changed().connect( sigc::mem_fun(*this, &ZigBeePacketBuilder::on_data_change) );
        sw_data.add(tv_data);
        sw_data.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        sw_data.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        
        hex_data.set_label("Hex");
        hex_data.set_active(true);
        hex_data.signal_toggled().connect( sigc::mem_fun(*this, &ZigBeePacketBuilder::on_hex_data_toggle) );
        al_hex_data.add(hex_data);
        al_hex_data.set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, 0, 0);
        
}


ZigBeePacketBuilder::~ZigBeePacketBuilder()
{
        labels.clear();
        fields.clear();
}


void ZigBeePacketBuilder::on_type_change()
{
        pkt.identifier = ZigBeePacket::ZBP_Identifier(identifier_list[cmbt_type.get_active_row_number()]);
        
        read_packet();
}

void ZigBeePacketBuilder::on_field_change(int offset, int index)
{
        if (updating_fields)
                return;
        
        if (pkt.frame_id_offset == offset)
        {
                pkt.frame_id = parse_number(fields[index]->get_text());
        }
        
        if (pkt.at_cmd_offset == offset)
        {
                Glib::ustring str = fields[index]->get_text();
                pkt.at_cmd[0] = ' ';
                pkt.at_cmd[1] = ' ';
                if (str.size() > 0)
                        pkt.at_cmd[0] = str[0];
                if (str.size() > 1)
                        pkt.at_cmd[1] = str[1];
        }
        
        if (pkt.status_offset == offset)
        {
                pkt.status = parse_number(fields[index]->get_text());
        }
        
        if (pkt.options_offset == offset)
        {
                pkt.options = parse_number(fields[index]->get_text());
        }
        
        if (pkt.dest64_offset == offset)
        {
                pkt.dest64 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.dest16_offset == offset)
        {
                pkt.dest16 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.src64_offset == offset)
        {
                pkt.src64 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.src16_offset == offset)
        {
                pkt.src16 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.sender64_offset == offset)
        {
                pkt.sender64 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.sender16_offset == offset)
        {
                pkt.sender16 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.parent16_offset == offset)
        {
                pkt.parent16 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.new64_offset == offset)
        {
                pkt.new64 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.new16_offset == offset)
        {
                pkt.new16 = parse_number(fields[index]->get_text());
        }
        
        if (pkt.src_ep_offset == offset)
        {
                pkt.src_ep = parse_number(fields[index]->get_text());
        }
        
        if (pkt.dest_ep_offset == offset)
        {
                pkt.dest_ep = parse_number(fields[index]->get_text());
        }
        
        if (pkt.cluster_id_offset == offset)
        {
                pkt.cluster_id = parse_number(fields[index]->get_text());
        }
        
        if (pkt.profile_id_offset == offset)
        {
                pkt.profile_id = parse_number(fields[index]->get_text());
        }
        
        if (pkt.radius_offset == offset)
        {
                pkt.radius = parse_number(fields[index]->get_text());
        }
        
        if (pkt.transmit_retries_offset == offset)
        {
                pkt.transmit_retries = parse_number(fields[index]->get_text());
        }
        
        if (pkt.delivery_status_offset == offset)
        {
                pkt.delivery_status = parse_number(fields[index]->get_text());
        }
        
        if (pkt.discovery_status_offset == offset)
        {
                pkt.discovery_status = parse_number(fields[index]->get_text());
        }
        
        if (pkt.digital_mask_offset == offset)
        {
                pkt.digital_mask = parse_number(fields[index]->get_text());
        }
        
        if (pkt.analog_mask_offset == offset)
        {
                pkt.analog_mask = parse_number(fields[index]->get_text());
        }
        
        if (pkt.num_samples_offset == offset)
        {
                pkt.num_samples = parse_number(fields[index]->get_text());
        }
        
        if (pkt.route_records_offset == offset)
        {
                pkt.route_records.clear();
                
                Glib::ustring str = fields[index]->get_text();
                
                for (int i = 0; i < str.size(); i++)
                {
                        int k, num;
                        
                        if (sscanf(str.c_str()+i, "%4x%n", &k, &num) > 0)
                        {
                                i += num-1;
                                pkt.route_records.push_back(k);
                        }
                }
        }
        
        update_packet();
}

void ZigBeePacketBuilder::on_data_change()
{
        if (updating_fields)
                return;
        
        pkt.data.clear();
        
        Glib::ustring str = tv_data.get_buffer()->get_text();
        
        if (hex_data.get_active())
        {
                for (int i = 0; i < str.size(); i++)
                {
                        int k, num;
                        uint8_t b;
                        
                        if (sscanf(str.c_str()+i, "%2x%n", &k, &num) > 0)
                        {
                                i += num-1;
                                b = k;
                                pkt.data.push_back(b);
                        }
                }
        }
        else
        {
                for (int i = 0; i < str.size(); i++)
                {
                        pkt.data.push_back(str[i]);
                }
        }
        
        update_packet();
}

void ZigBeePacketBuilder::on_hex_data_toggle()
{
        update_data();
}

void ZigBeePacketBuilder::update_packet()
{
        pkt.build_packet();
        
        m_signal_changed.emit();
}

void ZigBeePacketBuilder::update_data()
{
        std::stringstream ss;
        
        for (int j = 0; j < pkt.data.size(); j++)
        {
                if (hex_data.get_active())
                {
                        if (j > 0)
                                ss << " ";
                        ss << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.data[j];
                }
                else
                {
                        ss << pkt.data[j];
                }
        }
        
        updating_fields = true;
        
        tv_data.get_buffer()->set_text(Glib::convert(ss.str(), "utf-8", "iso-8859-1"));
        
        updating_fields = false;
}

unsigned long ZigBeePacketBuilder::parse_number(Glib::ustring str)
{
        unsigned long l = 0;
        size_t n = str.find("0x");
        
        if (n != std::string::npos)
        {
                l = std::strtol(str.substr(n).c_str(), 0, 16);
        }
        else
        {
                l = std::strtol(str.c_str(), 0, 10);
        }
        
        return l;
}

void ZigBeePacketBuilder::read_packet()
{
        int row;
        std::stringstream ss;
        
        if (pkt.identifier != current_identifier)
        {
                row = 2;
                
                current_identifier = pkt.identifier;
                
                ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << pkt.identifier;
                ent_identifier.set_text(ss.str());
                
                labels.clear();
                fields.clear();
                if (sw_data.get_parent())
                {
                        tbl.remove(sw_data);
                        tbl.remove(al_hex_data);
                }
                tbl.resize(3, 2);
                
                if (!pkt.set_offsets())
                        return;
                
                pkt.build_packet();
                
                tbl.resize(pkt.field_count+1, 2);
                
                for (int i = 1; i <= pkt.min_length; i++)
                {
                        if (pkt.frame_id_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Frame ID:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.at_cmd_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("AT Command:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.status_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Status:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.options_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Options:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.dest64_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Dest 64:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.dest16_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Dest 16:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.src64_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Source 64:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.src16_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Source 16:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.sender64_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Sender 64:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.sender16_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Sender 16:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.parent16_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Parent 16:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.new64_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("New 64:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.new16_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("New 16:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.src_ep_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Source EP:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.dest_ep_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Dest EP:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.cluster_id_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Cluster ID:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.profile_id_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Profile ID:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.radius_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Radius:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.transmit_retries_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Transmit Retries:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.delivery_status_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Delivery Status:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.discovery_status_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Discovery Status:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.digital_mask_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Digital Mask:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.analog_mask_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Analog Mask:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.num_samples_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Samples:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                        if (pkt.data_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Data:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                tbl.attach(al_hex_data, 0, 1, row+1, row+2);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                tbl.attach(sw_data, 1, 2, row, row+2);
                                
                                row += 2;
                        }
                        
                        if (pkt.route_records_offset == i)
                        {
                                labels.push_back(shared_ptr<Gtk::Label>(new Gtk::Label()));
                                labels.back()->set_label("Route Records:");
                                labels.back()->set_visible(true);
                                tbl.attach(*labels.back(), 0, 1, row, row+1);
                                
                                fields.push_back(shared_ptr<Gtk::Entry>(new Gtk::Entry()));
                                fields.back()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this, &ZigBeePacketBuilder::on_field_change), i, fields.size()-1));
                                fields.back()->set_visible(true);
                                tbl.attach(*fields.back(), 1, 2, row, row+1);
                                
                                row++;
                        }
                        
                }
                
        }
        
        row = 0;
        
        updating_fields = true;
        
        for (int i = 1; i <= pkt.min_length; i++)
        {
                if (pkt.frame_id_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.frame_id;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.at_cmd_offset == i)
                {
                        ss.str("");
                        ss << pkt.at_cmd[0] << pkt.at_cmd[1];
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.status_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.status;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.options_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.options;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.dest64_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(16) << std::hex << pkt.dest64;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.dest16_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.dest16;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.src64_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(16) << std::hex << pkt.src64;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.src16_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.src16;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.sender64_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(16) << std::hex << pkt.sender64;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.sender16_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.sender16;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.parent16_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.parent16;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.new64_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(16) << std::hex << pkt.new64;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.new16_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.new16;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.src_ep_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.src_ep;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.dest_ep_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.dest_ep;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.cluster_id_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.cluster_id;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.profile_id_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.profile_id;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.radius_offset == i)
                {
                        ss.str("");
                        ss << std::dec << (int)pkt.radius;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.transmit_retries_offset == i)
                {
                        ss.str("");
                        ss << std::dec << (int)pkt.transmit_retries;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.delivery_status_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.delivery_status;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.discovery_status_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.discovery_status;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.digital_mask_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.digital_mask;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.analog_mask_offset == i)
                {
                        ss.str("");
                        ss << "0x" << std::setfill('0') << std::setw(2) << std::hex << (int)pkt.analog_mask;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.num_samples_offset == i)
                {
                        ss.str("");
                        ss << std::dec << (int)pkt.num_samples;
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
                if (pkt.data_offset == i)
                {
                        update_data();
                        updating_fields = true;
                        
                        row += 2;
                }
                
                if (pkt.route_records_offset == i)
                {
                        ss.str("");
                        for (int j = 0; j < pkt.route_records.size(); j++)
                        {
                                if (j > 0)
                                        ss << " ";
                                ss << "0x" << std::setfill('0') << std::setw(4) << std::hex << (int)pkt.route_records[j];
                        }
                        fields[row]->set_text(ss.str());
                        
                        row++;
                }
                
        }
        
        updating_fields = false;
        
        m_signal_changed.emit();
}

void ZigBeePacketBuilder::set_packet(ZigBeePacket p)
{
        pkt = p;
        
        for (int i = 0; i < identifier_list.size(); i++)
        {
                if (pkt.identifier == identifier_list[i])
                {
                        cmbt_type.set_active(i);
                        read_packet();
                        break;
                }
        }
}

ZigBeePacket ZigBeePacketBuilder::get_packet()
{
        return pkt;
}

sigc::signal<void> ZigBeePacketBuilder::signal_changed()
{
        return m_signal_changed;
}




