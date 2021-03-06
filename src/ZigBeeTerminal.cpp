/************************************************************************/
/* ZigBeeTerminal                                                       */
/*                                                                      */
/* ZigBee Terminal                                                      */
/*                                                                      */
/* ZigBeeTerminal.cpp                                                   */
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

#include "ZigBeeTerminal.h"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

ZigBeeTerminal::ZigBeeTerminal()
{
        set_title("ZigBee Terminal");
        set_position(Gtk::WIN_POS_CENTER);
        
        add(vbox1);
        
        // Main menu
        
        vbox1.pack_start(main_menu, false, true, 0);
        
        file_menu_item.set_label("_File");
        file_menu_item.set_use_underline(true);
        main_menu.append(file_menu_item);
        
        file_menu_item.set_submenu(file_menu);
        
        file_quit_item.set_label(Gtk::Stock::QUIT.id);
        file_quit_item.set_use_stock(true);
        file_quit_item.signal_activate().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_file_quit_item_activate) );
        file_menu.append(file_quit_item);
        
        view_menu_item.set_label("_View");
        view_menu_item.set_use_underline(true);
        main_menu.append(view_menu_item);
        
        view_menu_item.set_submenu(view_menu);
        
        view_hex_terminal.set_label("Hex Terminal");
        view_hex_terminal.signal_toggled().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_view_hex_terminal_toggle) );
        view_menu.append(view_hex_terminal);
        
        view_hex_log.set_label("Hex Log");
        view_hex_log.signal_toggled().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_view_hex_log_toggle) );
        view_menu.append(view_hex_log);
        
        view_menu.append(view_sep1);
        
        view_clear_item.set_label("Clear");
        view_clear_item.signal_activate().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_view_clear_activate) );
        view_menu.append(view_clear_item);
        
        config_menu_item.set_label("_Config");
        config_menu_item.set_use_underline(true);
        main_menu.append(config_menu_item);
        
        config_menu_item.set_submenu(config_menu);
        
        config_port_item.set_label("Port");
        config_port_item.signal_activate().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_config_port_item_activate) );
        config_menu.append(config_port_item);
        
        config_close_port_item.set_label("Close Port");
        config_close_port_item.signal_activate().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_config_close_port_item_activate) );
        config_menu.append(config_close_port_item);
        
        config_menu.append(config_sep1);
        
        config_local_echo.set_label("Local Echo");
        config_menu.append(config_local_echo);
        
        config_api_mode.set_label("Use API Mode");
        config_menu.append(config_api_mode);
        
        // Tabs
        note.set_border_width(5);
        vbox1.pack_start(note, true, true, 0);
        
        // Terminal tab
        note.append_page(vbox_term, "Terminal");
        
        tv_term.set_size_request(400,400);
        tv_term.modify_font(Pango::FontDescription("monospace"));
        tv_term.set_editable(false);
        tv_term.set_wrap_mode(Gtk::WRAP_WORD_CHAR);
        tv_term.signal_key_press_event().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_tv_key_press), false );
        
        // tags
        {
                Glib::RefPtr<Gtk::TextBuffer::Tag> recv = tv_term.get_buffer()->create_tag("recv");
                Glib::RefPtr<Gtk::TextBuffer::Tag> xmit = tv_term.get_buffer()->create_tag("xmit");
                recv->property_weight() = PANGO_WEIGHT_BOLD;
        }
        
        sw_term.add(tv_term);
        sw_term.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        vbox_term.pack_start(sw_term, true, true, 0);
        
        // Raw Log Tab
        note.append_page(vbox_raw_log, "Raw Log");
        tv_raw_log.modify_font(Pango::FontDescription("monospace"));
        tv_raw_log.set_editable(false);
        tv_raw_log.set_wrap_mode(Gtk::WRAP_WORD_CHAR);
        
        // tags
        {
                Glib::RefPtr<Gtk::TextBuffer::Tag> recv = tv_raw_log.get_buffer()->create_tag("recv");
                Glib::RefPtr<Gtk::TextBuffer::Tag> xmit = tv_raw_log.get_buffer()->create_tag("xmit");
                recv->property_weight() = PANGO_WEIGHT_BOLD;
        }
        
        sw_raw_log.add(tv_raw_log);
        sw_raw_log.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        vbox_raw_log.pack_start(sw_raw_log, true, true, 0);
        
        // Packet Log Tab
        note.append_page(vpane_pkt_log, "Packet Log");
        //note.append_page(vbox_pkt_log, "Packet Log");
        
        tv_pkt_log_tm = Gtk::ListStore::create(cPacketLogModel);
        tv_pkt_log.set_model(tv_pkt_log_tm);
        tv_pkt_log.signal_cursor_changed().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_tv_pkt_log_cursor_changed) );
        
        tv_pkt_log.append_column("Dir", cPacketLogModel.Direction);
        tv_pkt_log.append_column("Type", cPacketLogModel.Type);
        tv_pkt_log.append_column("Sz", cPacketLogModel.Size);
        tv_pkt_log.append_column("Data", cPacketLogModel.Data);
        
        tv_pkt_log.modify_font(Pango::FontDescription("monospace"));
        
        sw_pkt_log.add(tv_pkt_log);
        sw_pkt_log.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        vpane_pkt_log.pack1(sw_pkt_log, true, true);
        //vbox_pkt_log.pack_start(sw_pkt_log, true, true, 0);
        
        tv2_pkt_log.set_size_request(400,100);
        tv2_pkt_log.modify_font(Pango::FontDescription("monospace"));
        tv2_pkt_log.set_editable(false);
        tv2_pkt_log.set_wrap_mode(Gtk::WRAP_WORD_CHAR);
        
        sw2_pkt_log.add(tv2_pkt_log);
        sw2_pkt_log.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        vpane_pkt_log.pack2(sw2_pkt_log, false, false);
        
        // Packet Builder Tab
        note.append_page(vbox_pkt_builder, "Packet Builder");
        
        vbox_pkt_builder.pack_start(vpane_pkt_builder, true, true, 0);
        
        pkt_builder.set_border_width(5);
        pkt_builder.signal_changed().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_pkt_builder_change) );
        //vbox_pkt_builder.pack_start(pkt_builder, true, true, 0);
        sw_pkt_builder.add(pkt_builder);
        sw_pkt_builder.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);
        vpane_pkt_builder.pack1(sw_pkt_builder, true, true);
        
        tv_pkt_builder.modify_font(Pango::FontDescription("monospace"));
        tv_pkt_builder.set_editable(false);
        tv_pkt_builder.set_wrap_mode(Gtk::WRAP_WORD_CHAR);
        
        sw2_pkt_builder.add(tv_pkt_builder);
        sw2_pkt_builder.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        vpane_pkt_builder.pack2(sw2_pkt_builder, false, false);
        
        bbox_pkt_builder.set_layout(Gtk::BUTTONBOX_SPREAD);
        bbox_pkt_builder.set_border_width(5);
        vbox_pkt_builder.pack_start(bbox_pkt_builder, false, false, 0);
        
        btn_pkt_builder_send.set_label("Send Packet");
        btn_pkt_builder_send.signal_clicked().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_btn_pkt_builder_send_click) );
        bbox_pkt_builder.add(btn_pkt_builder_send);
        
        // status bar
        
        status.push("Not connected");
        vbox1.pack_start(status, false, true, 0);
        
        baud = 115200;
        port = "";
        parity = SerialInterface::SP_None;
        bits = 8;
        stop_bits = 1;
        flow_control = SerialInterface::SF_None;
        
        data_log_ptr = 0;
        raw_data_log_ptr = 0;
        
        dlgPort.set_port(port);
        dlgPort.set_baud(baud);
        dlgPort.set_parity(parity);
        dlgPort.set_bits(bits);
        dlgPort.set_stop_bits(stop_bits);
        dlgPort.set_flow_control(flow_control);
        
        ser_int = std::tr1::shared_ptr<SerialInterface>(new SerialInterface());
        
        ser_int->port_opened().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_open) );
        ser_int->port_closed().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_close) );
        //ser_int->port_error().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_error) );
        //ser_int->port_receive_data().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_port_receive_data) );
        
        ser_int->set_debug(true);
        
        zb_int.set_serial_interface(ser_int);
        
        zb_int.signal_receive_packet().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_receive_packet) );
        zb_int.signal_receive_raw_data().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_receive_raw_data) );
        zb_int.signal_send_raw_data().connect( sigc::mem_fun(*this, &ZigBeeTerminal::on_send_raw_data) );
        
        show_all_children();
}


ZigBeeTerminal::~ZigBeeTerminal()
{
        
}


void ZigBeeTerminal::on_file_quit_item_activate()
{
        gtk_main_quit();
}


void ZigBeeTerminal::on_config_port_item_activate()
{
        int response;
        
        dlgPort.set_port(port);
        dlgPort.set_baud(baud);
        dlgPort.set_parity(parity);
        dlgPort.set_bits(bits);
        dlgPort.set_stop_bits(stop_bits);
        dlgPort.set_flow_control(flow_control);
        response = dlgPort.run();
        
        if (response == Gtk::RESPONSE_OK)
        {
                close_port();
                
                port = dlgPort.get_port();
                baud = dlgPort.get_baud();
                parity = dlgPort.get_parity();
                bits = dlgPort.get_bits();
                stop_bits = dlgPort.get_stop_bits();
                flow_control = dlgPort.get_flow_control();
                
                open_port();
        }
}


void ZigBeeTerminal::on_config_close_port_item_activate()
{
        close_port();
}


void ZigBeeTerminal::on_view_hex_terminal_toggle()
{
        tv_term.get_buffer()->set_text("");
        data_log_ptr = 0;
        update_log();
}


void ZigBeeTerminal::on_view_hex_log_toggle()
{
        tv_raw_log.get_buffer()->set_text("");
        raw_data_log_ptr = 0;
        update_raw_log();
}


void ZigBeeTerminal::on_view_clear_activate()
{
        read_data_queue.clear();
        data_log.clear();
        data_log_ptr = 0;
        raw_data_log.clear();
        raw_data_log_ptr = 0;
        tv_term.get_buffer()->set_text("");
        tv_raw_log.get_buffer()->set_text("");
        tv_pkt_log_tm->clear();
}


bool ZigBeeTerminal::on_tv_key_press(GdkEventKey *key)
{
        guint u = gdk_keyval_to_unicode(key->keyval);
        Glib::ustring str = "";
        gsize num;
        
        if (u > 0)
        {
                str = Glib::ustring(1, u);
        }
        else
        {
                switch (key->keyval)
                {
                        case GDK_Return:
                                str = "\n";
                                break;
                }
        }
        
        if (str.length() > 0)
        {
                if (ser_int->is_open() && !config_api_mode.get_active())
                {
                        ser_int->write(str.c_str(), str.size(), num);
                        
                        if (config_local_echo.get_active())
                        {
                                for (int i = 0; i < str.size(); i++)
                                {
                                        data_log.push_back(0x1000 | ((int)str[i] & 0x00FF));
                                }
                                
                                update_log();
                        }
                }
        }
}


void ZigBeeTerminal::on_tv_pkt_log_cursor_changed()
{
        Gtk::TreeModel::iterator it = tv_pkt_log.get_selection()->get_selected();
        Gtk::TreeModel::Row row = *it;
        ZigBeePacket pkt = (ZigBeePacket)row[cPacketLogModel.Packet];
        
        tv2_pkt_log.get_buffer()->set_text(pkt.get_desc());
        
        pkt_builder.set_packet(pkt);
}


void ZigBeeTerminal::on_pkt_builder_change()
{
        tv_pkt_builder.get_buffer()->set_text(pkt_builder.get_packet().get_hex_packet());
}


void ZigBeeTerminal::on_btn_pkt_builder_send_click()
{
        gsize num;
        int ret;
        int len;
        char *ptr;
        
        ZigBeePacket pkt = pkt_builder.get_packet();
        
        if (config_api_mode.get_active())
        {
                zb_int.send_packet(pkt);
                
                if (pkt.identifier == ZigBeePacket::ZBPID_TxRequest ||
                        pkt.identifier == ZigBeePacket::ZBPID_EATxRequest ||
                        pkt.identifier == ZigBeePacket::ZBPID_RxPacket ||
                        pkt.identifier == ZigBeePacket::ZBPID_EARxPacket)
                {
                        for (int i = 0; i < pkt.data.size(); i++)
                        {
                                data_log.push_back(0x1000 | ((int)pkt.data[i] & 0x00FF));
                        }
                }
                
                update_log();
                update_raw_log();
                
                Gtk::TreeModel::iterator it = tv_pkt_log_tm->append();
                Gtk::TreePath path = Gtk::TreePath(it);
                Gtk::TreeModel::Row row = *it;
                row[cPacketLogModel.Packet] = pkt;
                row[cPacketLogModel.Direction] = "TX";
                row[cPacketLogModel.Type] = pkt.get_type_desc();
                row[cPacketLogModel.Size] = pkt.get_length();
                row[cPacketLogModel.Data] = pkt.get_hex_packet();
                tv_pkt_log.scroll_to_row(path);
        }
        
}


void ZigBeeTerminal::on_port_open()
{
        gsize num;
        
        std::cout << "on_port_open()" << std::endl;
        
        port = ser_int->get_port();
        baud = ser_int->get_baud();
        
        status.pop();
        status.push(ser_int->get_status_string());
}


void ZigBeeTerminal::on_port_close()
{
        status.pop();
        status.push(ser_int->get_status_string());
}


void ZigBeeTerminal::on_receive_packet(ZigBeePacket pkt)
{
        if (config_api_mode.get_active())
        {
                Gtk::TreeModel::iterator it = tv_pkt_log_tm->append();
                Gtk::TreePath path = Gtk::TreePath(it);
                Gtk::TreeModel::Row row = *it;
                row[cPacketLogModel.Packet] = pkt;
                row[cPacketLogModel.Direction] = "RX";
                row[cPacketLogModel.Type] = pkt.get_type_desc();
                row[cPacketLogModel.Size] = pkt.get_length();
                row[cPacketLogModel.Data] = pkt.get_hex_packet();
                tv_pkt_log.scroll_to_row(path);
                
                if (pkt.identifier == ZigBeePacket::ZBPID_TxRequest ||
                        pkt.identifier == ZigBeePacket::ZBPID_EATxRequest ||
                        pkt.identifier == ZigBeePacket::ZBPID_RxPacket ||
                        pkt.identifier == ZigBeePacket::ZBPID_EARxPacket)
                {
                        for (int i = 0; i < pkt.data.size(); i++)
                        {
                                data_log.push_back(((int)pkt.data[i] & 0x00FF));
                        }
                }
                
                update_log();
        }
}


void ZigBeeTerminal::on_receive_raw_data(const char *data, size_t len)
{
        for (size_t i = 0; i < len; i++)
        {
                raw_data_log.push_back(((int)data[i] & 0x00FF));
        }
        
        if (!config_api_mode.get_active())
        {
                for (size_t i = 0; i < len; i++)
                {
                        data_log.push_back(((int)data[i] & 0x00FF));
                }
        }
        
        update_log();
        update_raw_log();
}


void ZigBeeTerminal::on_send_raw_data(const char *data, size_t len)
{
        for (size_t i = 0; i < len; i++)
        {
                raw_data_log.push_back(0x1000 | ((int)data[i] & 0x00FF));
        }
        
        update_raw_log();
}


void ZigBeeTerminal::update_log()
{
        Glib::RefPtr<Gtk::TextBuffer> buffer = tv_term.get_buffer();
        
        if (view_hex_terminal.get_active())
        {
                for (unsigned int i = data_log_ptr; i < data_log.size(); i++)
                {
                        std::stringstream ss;
                        
                        if (i > 0)
                        {
                                if (i % 16 == 0)
                                        ss << "\n";
                                else
                                        ss << " ";
                        }
                        
                        ss << std::setfill('0') << std::setw(2) << std::hex << (data_log[i] & 0x00FF);
                        
                        if (data_log[i] & 0x1000)
                        {
                                buffer->insert_with_tag(buffer->end(), ss.str(), "xmit");
                        }
                        else
                        {
                                buffer->insert_with_tag(buffer->end(), ss.str(), "recv");
                        }
                }
        }
        else
        {
                for (unsigned int i = data_log_ptr; i < data_log.size(); i++)
                {
                        std::stringstream ss;
                        
                        ss << (uint8_t)data_log[i];
                        
                        if (data_log[i] & 0x1000)
                        {
                                buffer->insert_with_tag(buffer->end(), Glib::convert(ss.str(), "utf-8", "iso-8859-1"), "xmit");
                        }
                        else
                        {
                                buffer->insert_with_tag(buffer->end(), Glib::convert(ss.str(), "utf-8", "iso-8859-1"), "recv");
                        }
                }
        }
        
        Glib::RefPtr<Gtk::TextMark> end_mark = buffer->create_mark (buffer->end()); 
        tv_term.scroll_to(end_mark);
        buffer->delete_mark(end_mark);
        
        data_log_ptr = data_log.size();
}


void ZigBeeTerminal::update_raw_log()
{
        Glib::RefPtr<Gtk::TextBuffer> buffer = tv_raw_log.get_buffer();
        
        if (view_hex_log.get_active())
        {
                for (unsigned int i = raw_data_log_ptr; i < raw_data_log.size(); i++)
                {
                        std::stringstream ss;
                        
                        if (i > 0)
                        {
                                if (i % 16 == 0)
                                        ss << "\n";
                                else
                                        ss << " ";
                        }
                        
                        ss << std::setfill('0') << std::setw(2) << std::hex << (raw_data_log[i] & 0x00FF);
                        
                        if (raw_data_log[i] & 0x1000)
                        {
                                buffer->insert_with_tag(buffer->end(), ss.str(), "xmit");
                        }
                        else
                        {
                                buffer->insert_with_tag(buffer->end(), ss.str(), "recv");
                        }
                }
        }
        else
        {
                for (unsigned int i = raw_data_log_ptr; i < raw_data_log.size(); i++)
                {
                        std::stringstream ss;
                        
                        ss << (uint8_t)raw_data_log[i];
                        
                        if (raw_data_log[i] & 0x1000)
                        {
                                buffer->insert_with_tag(buffer->end(), Glib::convert(ss.str(), "utf-8", "iso-8859-1"), "xmit");
                        }
                        else
                        {
                                buffer->insert_with_tag(buffer->end(), Glib::convert(ss.str(), "utf-8", "iso-8859-1"), "recv");
                        }
                }
        }
        
        Glib::RefPtr<Gtk::TextMark> end_mark = buffer->create_mark (buffer->end()); 
        tv_raw_log.scroll_to(end_mark);
        buffer->delete_mark(end_mark);
        
        raw_data_log_ptr = raw_data_log.size();
}


void ZigBeeTerminal::open_port()
{
        if (ser_int->is_open())
                close_port();
        
        ser_int->set_port(port);
        ser_int->set_baud(baud);
        ser_int->set_parity(parity);
        ser_int->set_bits(bits);
        ser_int->set_stop(stop_bits);
        ser_int->set_flow(flow_control);
        ser_int->open_port();
}


void ZigBeeTerminal::close_port()
{
        ser_int->close_port();
}



