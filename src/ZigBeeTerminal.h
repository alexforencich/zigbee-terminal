/************************************************************************/
/* ZigBeeTerminal                                                       */
/*                                                                      */
/* ZigBee Terminal                                                      */
/*                                                                      */
/* ZigBeeTerminal.h                                                     */
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

#ifndef __ZIGBEE_TERMINAL_H
#define __ZIGBEE_TERMINAL_H

#include <gtkmm.h>

#include "PortConfig.h"
#include "SerialInterface.h"
#include "ZigBeePacket.h"
#include "ZigBeePacketBuilder.h"

// ZigBeeTerminal class
class ZigBeeTerminal : public Gtk::Window
{
public:
        ZigBeeTerminal();
        virtual ~ZigBeeTerminal();
        
protected:
        //Signal handlers:
        void on_file_quit_item_activate();
        void on_config_port_item_activate();
        void on_config_close_port_item_activate();
        
        void on_view_hex_terminal_toggle();
        void on_view_hex_log_toggle();
        
        bool on_tv_key_press(GdkEventKey *key);
        
        void on_tv_pkt_log_cursor_changed();
        
        void on_pkt_builder_change();
        void on_btn_pkt_builder_send_click();
        
        void on_port_open();
        void on_port_close();
        void on_port_receive_data();
        
        void on_receive_data();
        
        void update_log();
        void update_raw_log();
        
        void open_port();
        void close_port();
        
        // Tree model columns
        class PacketLogModel : public Gtk::TreeModel::ColumnRecord
        {
        public:
                PacketLogModel()
                { add(Packet); add(Direction); add(Type); add(Size); add(Data); }
                
                Gtk::TreeModelColumn<ZigBeePacket> Packet;
                Gtk::TreeModelColumn<Glib::ustring> Direction;
                Gtk::TreeModelColumn<Glib::ustring> Type;
                Gtk::TreeModelColumn<int> Size;
                Gtk::TreeModelColumn<Glib::ustring> Data;
        };
        
        PacketLogModel cPacketLogModel;
        
        Glib::RefPtr<Gtk::ListStore> tv_pkt_log_tm;
        
        //Child widgets:
        // window
        Gtk::VBox vbox1;
        // menu bar
        Gtk::MenuBar main_menu;
        Gtk::MenuItem file_menu_item;
        Gtk::Menu file_menu;
        Gtk::ImageMenuItem file_quit_item;
        Gtk::MenuItem view_menu_item;
        Gtk::Menu view_menu;
        Gtk::CheckMenuItem view_hex_terminal;
        Gtk::CheckMenuItem view_hex_log;
        Gtk::MenuItem config_menu_item;
        Gtk::Menu config_menu;
        Gtk::ImageMenuItem config_port_item;
        Gtk::ImageMenuItem config_close_port_item;
        Gtk::SeparatorMenuItem config_sep1;
        Gtk::CheckMenuItem config_local_echo;
        Gtk::CheckMenuItem config_api_mode;
        // tabs
        Gtk::Notebook note;
        // terminal
        Gtk::VBox vbox_term;
        Gtk::ScrolledWindow sw_term;
        Gtk::TextView tv_term;
        // raw log
        Gtk::VBox vbox_raw_log;
        Gtk::ScrolledWindow sw_raw_log;
        Gtk::TextView tv_raw_log;
        // packet log
        Gtk::VPaned vpane_pkt_log;
        //Gtk::VBox vbox_pkt_log;
        Gtk::ScrolledWindow sw_pkt_log;
        Gtk::TreeView tv_pkt_log;
        Gtk::ScrolledWindow sw2_pkt_log;
        Gtk::TextView tv2_pkt_log;
        // packet builder
        Gtk::VBox vbox_pkt_builder;
        Gtk::VPaned vpane_pkt_builder;
        Gtk::HButtonBox bbox_pkt_builder;
        Gtk::Button btn_pkt_builder_send;
        Gtk::ScrolledWindow sw_pkt_builder;
        ZigBeePacketBuilder pkt_builder;
        Gtk::ScrolledWindow sw2_pkt_builder;
        Gtk::TextView tv_pkt_builder;
        // status bar
        Gtk::Statusbar status;
        
        PortConfig dlgPort;
        
        Glib::ustring port;
        unsigned long baud;
        SerialInterface::SerialParity parity;
        int bits;
        int stop_bits;
        SerialInterface::SerialFlow flow_control;
        
        SerialInterface ser_int;
        
        std::deque<char> read_data_queue;
        
        std::vector<int> data_log;
        std::vector<int> raw_data_log;
        
        unsigned int data_log_ptr;
        unsigned int raw_data_log_ptr;
        
};

#endif //__ZIGBEE_TERMINAL_H
