/************************************************************************/
/* ZigBeePacket                                                         */
/*                                                                      */
/* ZigBee Terminal - ZigBee Packet                                      */
/*                                                                      */
/* ZigBeePacket.cpp                                                     */
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

#include "ZigBeePacket.h"

#include <sstream>
#include <iomanip>

// Static
bool ZigBeePacket::is_valid_identifier(int identifier)
{
        switch (identifier)
        {
                case ZBPID_TxRequest64:
                        return true;
                case ZBPID_TxRequest16:
                        return true;
                case ZBPID_ATCommand:
                        return true;
                case ZBPID_ATCommandQueueRegisterValue:
                        return true;
                case ZBPID_TxRequest:
                        return true;
                case ZBPID_EATxRequest:
                        return true;
                case ZBPID_RemoteATCommand:
                        return true;
                case ZBPID_CreateSourceRoute:
                        return true;
                case ZBPID_RegisterJoiningDevice:
                        return true;
                case ZBPID_RxPacket64:
                        return true;
                case ZBPID_RxPacket16:
                        return true;
                case ZBPID_RxPacketIO64:
                        return true;
                case ZBPID_RxPacketIO16:
                        return true;
                case ZBPID_ATCommandResponse:
                        return true;
                case ZBPID_TxStatusS1:
                        return true;
                case ZBPID_ModemStatus:
                        return true;
                case ZBPID_TxStatusS2:
                        return true;
                case ZBPID_RxPacket:
                        return true;
                case ZBPID_EARxPacket:
                        return true;
                case ZBPID_IODataSampleRx:
                        return true;
                case ZBPID_SensorRead:
                        return true;
                case ZBPID_NodeIdentification:
                        return true;
                case ZBPID_RemoteCommandResponse:
                        return true;
                case ZBPID_OTAFirmwareUpdateStatus:
                        return true;
                case ZBPID_RouteRecord:
                        return true;
                case ZBPID_DeviceAuthenticated:
                        return true;
                case ZBPID_ManyToOneRouteRequest:
                        return true;
                case ZBPID_RegisterJoiningDeviceStatus:
                        return true;
                case ZBPID_JoinNotificationStatus:
                        return true;
                default:
                        return false;
        }
}

// Static
std::vector<int> ZigBeePacket::get_valid_identifiers()
{
        std::vector<int> ids;
        
        for (int i = 0; i < 255; i++)
        {
                if (is_valid_identifier(i))
                        ids.push_back(i);
        }
        
        return ids;
}

// Static
std::string ZigBeePacket::get_type_desc(int identifier)
{
        switch (identifier)
        {
                case ZBPID_TxRequest64:
                        return "Transmit Request (64-bit address)";
                case ZBPID_TxRequest16:
                        return "Transmit Request (16-bit address)";
                case ZBPID_ATCommand:
                        return "AT Command";
                case ZBPID_ATCommandQueueRegisterValue:
                        return "AT Command Queue Register Value";
                case ZBPID_TxRequest:
                        return "Transmit Request";
                case ZBPID_EATxRequest:
                        return "Explicit Addressing Transmit Request";
                case ZBPID_RemoteATCommand:
                        return "Remote AT Command";
                case ZBPID_CreateSourceRoute:
                        return "Create Source Route";
                case ZBPID_RegisterJoiningDevice:
                        return "Register Joining Device";
                case ZBPID_RxPacket64:
                        return "Receive Packet (64-bit address)";
                case ZBPID_RxPacket16:
                        return "Receive Packet (16-bit address)";
                case ZBPID_RxPacketIO64:
                        return "Receive Packet (64-bit address IO)";
                case ZBPID_RxPacketIO16:
                        return "Receive Packet (16-bit address IO)";
                case ZBPID_ATCommandResponse:
                        return "AT Command Response";
                case ZBPID_TxStatusS1:
                        return "Transmit Status (S1)";
                case ZBPID_ModemStatus:
                        return "Modem Status";
                case ZBPID_TxStatusS2:
                        return "Transmit Status (S2)";
                case ZBPID_RxPacket:
                        return "Receive Packet";
                case ZBPID_EARxPacket:
                        return "Explicit Addressing Receive Packet";
                case ZBPID_IODataSampleRx:
                        return "IO Data Sample RX";
                case ZBPID_SensorRead:
                        return "Sensor Read";
                case ZBPID_NodeIdentification:
                        return "Node Identification";
                case ZBPID_RemoteCommandResponse:
                        return "Remote AT Command Response";
                case ZBPID_OTAFirmwareUpdateStatus:
                        return "Over-the-Air Firmware Update Status";
                case ZBPID_RouteRecord:
                        return "Route Record";
                case ZBPID_DeviceAuthenticated:
                        return "Device Authenticated";
                case ZBPID_ManyToOneRouteRequest:
                        return "Many To One Route Request";
                case ZBPID_RegisterJoiningDeviceStatus:
                        return "Register Joining Device Status";
                case ZBPID_JoinNotificationStatus:
                        return "Join Notification Status";
                default:
                        return "Unknown";
        }
}

ZigBeePacket::ZigBeePacket()
{
        zero();
}

ZigBeePacket::~ZigBeePacket()
{
        // nothing
}

void ZigBeePacket::zero()
{
        identifier = ZBPID_ATCommand;
        frame_id = 0;
        frame_id_offset = 0;
        at_cmd[0] = ' ';
        at_cmd[1] = ' ';
        at_cmd_offset = 0;
        status = 0;
        status_offset = 0;
        options = 0;
        options_offset = 0;
        reserved_offset = 0;
        min_length = 0;
        field_count = 0;
        dest64 = 0;
        dest64_offset = 0;
        dest16 = 0;
        dest16_offset = 0;
        src64 = 0;
        src64_offset = 0;
        src16 = 0;
        src16_offset = 0;
        sender64 = 0;
        sender64_offset = 0;
        sender16 = 0;
        sender16_offset = 0;
        parent16 = 0;
        parent16_offset = 0;
        new64 = 0;
        new64_offset = 0;
        new16 = 0;
        new16_offset = 0;
        src_ep = 0;
        src_ep_offset = 0;
        dest_ep = 0;
        dest_ep_offset = 0;
        cluster_id = 0;
        cluster_id_offset = 0;
        profile_id = 0;
        profile_id_offset = 0;
        radius = 0;
        radius_offset = 0;
        transmit_retries = 0;
        transmit_retries_offset = 0;
        delivery_status = 0;
        delivery_status_offset = 0;
        discovery_status = 0;
        discovery_status_offset = 0;
        rssi = 0;
        rssi_offset = 0;
        digital_mask = 0;
        digital_mask_offset = 0;
        analog_mask = 0;
        analog_mask_offset = 0;
        num_samples = 0;
        num_samples_offset = 0;
        data.clear();
        data_offset = 0;
        route_records.clear();
        route_records_offset = 0;
}

uint16_t ZigBeePacket::get_length()
{
        return payload.size()+4;
}

uint8_t ZigBeePacket::get_checksum()
{
        uint8_t sum = 0xFF;
        
        for (int i = 0; i < payload.size(); i++)
        {
                sum -= payload[i];
        }
        
        return sum;
}

std::vector<uint8_t> ZigBeePacket::get_raw_packet()
{
        std::vector<uint8_t> dataout;
        
        dataout.push_back(ZIGBEE_IDENTIFIER);
        dataout.push_back(payload.size() >> 8);
        dataout.push_back(payload.size());
        
        dataout.insert(dataout.end(), payload.begin(), payload.end());
        
        dataout.push_back(get_checksum());
        
        return dataout;
}

std::vector<uint8_t> ZigBeePacket::get_escaped_raw_packet()
{
        std::vector<uint8_t> datain = get_raw_packet();
        std::vector<uint8_t> dataout;
        
        dataout.push_back(ZIGBEE_IDENTIFIER);
        
        for (int i = 1; i < datain.size(); i++)
        {
                if (datain[i] == ZIGBEE_IDENTIFIER || datain[i] == ZIGBEE_ESCAPE ||
                        datain[i] == 0x11 || datain[i] == 0x13)
                {
                        dataout.push_back(ZIGBEE_ESCAPE);
                        dataout.push_back(datain[i]^0x20);
                }
                else
                {
                        dataout.push_back(datain[i]);
                }
        }
        
        return dataout;
}

bool ZigBeePacket::read_packet(std::vector<char> bytes, size_t &bytes_read)
{
        return read_packet((uint8_t *)&bytes[0], bytes.size(), bytes_read);
}

bool ZigBeePacket::read_packet(std::vector<uint8_t> bytes, size_t &bytes_read)
{
        return read_packet(&bytes[0], bytes.size(), bytes_read);
}

bool ZigBeePacket::read_packet(std::deque<char> bytes, size_t &bytes_read)
{
        return read_packet((uint8_t *)&bytes[0], bytes.size(), bytes_read);
}

bool ZigBeePacket::read_packet(std::deque<uint8_t> bytes, size_t &bytes_read)
{
        return read_packet(&bytes[0], bytes.size(), bytes_read);
}

bool ZigBeePacket::read_packet(uint8_t *bytes, size_t count, size_t &bytes_read)
{
        size_t n = 0;
        uint8_t *ptr = bytes;
        uint16_t size;
        uint8_t b;
        uint8_t sum = 0xff;
        
        if (count == 0)
                return false;
        
        // find packet start byte
        while ((*(ptr++) != ZIGBEE_IDENTIFIER) & (n++ < count)) { }
        
        // start of packet offset
        // to discard junk ahead of incomplete packet
        // (assuming packet is currently truncated)
        bytes_read = n-1;
        
        // return if out of buffer
        if (n > count)
                return false;
        
        // return if not enough bytes left
        if (count - n < 3)
                return false;
        
        // get packet size
        size = (uint16_t)*(ptr++) << 8;
        size |= (uint16_t)*(ptr++);
        n += 2;
        
        // return if we don't have the whole packet
        if (count - n < size + 1)
                return false;
        
        payload.clear();
        
        // read payload
        for (int i = 0; i < size; i++)
        {
                b = *(ptr++);
                payload.push_back(b);
                sum -= b;
                n++;
        }
        
        // add one for checksum and set bytes read
        n++;
        bytes_read = n;
        
        // confirm checksum
        if (sum != *ptr)
                return false;
        
        return true;
}

bool ZigBeePacket::set_offsets()
{
        // clear offsets
        frame_id_offset = 0;
        at_cmd_offset = 0;
        status_offset = 0;
        options_offset = 0;
        reserved_offset = 0;
        dest64_offset = 0;
        dest16_offset = 0;
        src64_offset = 0;
        src16_offset = 0;
        sender64_offset = 0;
        sender16_offset = 0;
        parent16_offset = 0;
        new64_offset = 0;
        new16_offset = 0;
        src_ep_offset = 0;
        dest_ep_offset = 0;
        cluster_id_offset = 0;
        profile_id_offset = 0;
        radius_offset = 0;
        transmit_retries_offset = 0;
        delivery_status_offset = 0;
        discovery_status_offset = 0;
        rssi_offset = 0;
        digital_mask_offset = 0;
        analog_mask_offset = 0;
        data_offset = 0;
        num_samples_offset = 0;
        route_records_offset = 0;
        min_length = 0;
        field_count = 0;
        
        switch (identifier)
        {
                case ZBPID_TxRequest64:
                        frame_id_offset = 1;
                        dest64_offset = 2;
                        options_offset = 10;
                        data_offset = 11;
                        min_length = 11;
                        field_count = 5;
                        return true;
                case ZBPID_TxRequest16:
                        frame_id_offset = 1;
                        dest16_offset = 2;
                        options_offset = 4;
                        data_offset = 5;
                        min_length = 5;
                        field_count = 5;
                        return true;
                case ZBPID_ATCommand:
                        frame_id_offset = 1;
                        at_cmd_offset = 2;
                        data_offset = 4;
                        min_length = 4;
                        field_count = 4;
                        return true;
                case ZBPID_ATCommandQueueRegisterValue:
                        frame_id_offset = 1;
                        at_cmd_offset = 2;
                        data_offset = 4;
                        min_length = 4;
                        field_count = 4;
                        return true;
                case ZBPID_TxRequest:
                        frame_id_offset = 1;
                        dest64_offset = 2;
                        dest16_offset = 10;
                        radius_offset = 12;
                        options_offset = 13;
                        data_offset = 14;
                        min_length = 14;
                        field_count = 7;
                        return true;
                case ZBPID_EATxRequest:
                        frame_id_offset = 1;
                        dest64_offset = 2;
                        dest16_offset = 10;
                        src_ep_offset = 12;
                        dest_ep_offset = 13;
                        cluster_id_offset = 14;
                        profile_id_offset = 16;
                        radius_offset = 18;
                        options_offset = 19;
                        data_offset = 20;
                        min_length = 20;
                        field_count = 11;
                        return true;
                case ZBPID_RemoteATCommand:
                        frame_id_offset = 1;
                        dest64_offset = 2;
                        dest16_offset = 10;
                        options_offset = 12;
                        at_cmd_offset = 13;
                        data_offset = 15;
                        min_length = 15;
                        field_count = 7;
                        return true;
                case ZBPID_CreateSourceRoute:
                        frame_id_offset = 1;
                        dest64_offset = 2;
                        dest16_offset = 10;
                        options_offset = 12;
                        route_records_offset = 13;
                        min_length = 14;
                        field_count = 6;
                        return true;
                case ZBPID_RegisterJoiningDevice:
                        frame_id_offset = 1;
                        dest64_offset = 2;
                        dest16_offset = 10;
                        options_offset = 12;
                        data_offset = 13;
                        min_length = 13;
                        field_count = 6;
                        return true;
                case ZBPID_RxPacket64:
                        src64_offset = 1;
                        rssi_offset = 9;
                        options_offset = 10;
                        data_offset = 11;
                        min_length = 11;
                        field_count = 5;
                        return true;
                case ZBPID_RxPacket16:
                        src16_offset = 1;
                        rssi_offset = 3;
                        options_offset = 4;
                        data_offset = 5;
                        min_length = 5;
                        field_count = 5;
                        return true;
                case ZBPID_RxPacketIO64:
                        src64_offset = 1;
                        rssi_offset = 9;
                        options_offset = 10;
                        data_offset = 11;
                        min_length = 11;
                        field_count = 5;
                        return true;
                case ZBPID_RxPacketIO16:
                        src16_offset = 1;
                        rssi_offset = 3;
                        options_offset = 4;
                        data_offset = 5;
                        min_length = 5;
                        field_count = 5;
                        return true;
                case ZBPID_ATCommandResponse:
                        frame_id_offset = 1;
                        at_cmd_offset = 2;
                        status_offset = 4;
                        data_offset = 5;
                        min_length = 5;
                        field_count = 5;
                        return true;
                case ZBPID_TxStatusS1:
                        frame_id_offset = 1;
                        status_offset = 2;
                        min_length = 3;
                        field_count = 3;
                        return true;
                case ZBPID_ModemStatus:
                        status_offset = 1;
                        min_length = 2;
                        field_count = 2;
                        return true;
                case ZBPID_TxStatusS2:
                        frame_id_offset = 1;
                        dest16_offset = 2;
                        transmit_retries_offset = 4;
                        delivery_status_offset = 5;
                        discovery_status_offset = 6;
                        min_length = 7;
                        field_count = 6;
                        return true;
                case ZBPID_RxPacket:
                        src64_offset = 1;
                        src16_offset = 9;
                        options_offset = 11;
                        data_offset = 12;
                        min_length = 12;
                        field_count = 5;
                        return true;
                case ZBPID_EARxPacket:
                        src64_offset = 1;
                        src16_offset = 9;
                        src_ep_offset = 11;
                        dest_ep_offset = 12;
                        cluster_id_offset = 13;
                        profile_id_offset = 15;
                        options_offset = 17;
                        data_offset = 18;
                        min_length = 18;
                        field_count = 9;
                        return true;
                case ZBPID_IODataSampleRx:
                        src64_offset = 1;
                        src16_offset = 9;
                        options_offset = 11;
                        num_samples_offset = 12;
                        digital_mask_offset = 13;
                        analog_mask_offset = 15;
                        // TODO
                        data_offset = 16;
                        min_length = 16;
                        field_count = 8;
                        return true;
                case ZBPID_SensorRead:
                        src64_offset = 1;
                        src16_offset = 9;
                        options_offset = 11;
                        // TODO
                        data_offset = 12;
                        min_length = 12;
                        field_count = 5;
                        return true;
                case ZBPID_NodeIdentification:
                        sender64_offset = 1;
                        sender16_offset = 9;
                        options_offset = 11;
                        src64_offset = 12;
                        src16_offset = 20;
                        data_offset = 21;
                        min_length = 21;
                        field_count = 7;
                        return true;
                case ZBPID_RemoteCommandResponse:
                        frame_id_offset = 1;
                        src64_offset = 2;
                        src16_offset = 10;
                        at_cmd_offset = 12;
                        status_offset = 14;
                        data_offset = 15;
                        min_length = 15;
                        field_count = 7;
                        return true;
                case ZBPID_OTAFirmwareUpdateStatus:
                        src64_offset = 1;
                        dest16_offset = 9;
                        options_offset = 11;
                        // TODO
                        data_offset = 12;
                        min_length = 12;
                        field_count = 5;
                        return true;
                case ZBPID_RouteRecord:
                        src64_offset = 1;
                        src16_offset = 9;
                        options_offset = 11;
                        route_records_offset = 12;
                        min_length = 13;
                        field_count = 5;
                        return true;
                case ZBPID_DeviceAuthenticated:
                        src64_offset = 1;
                        src16_offset = 9;
                        status_offset = 11;
                        min_length = 12;
                        field_count = 4;
                        return true;
                case ZBPID_ManyToOneRouteRequest:
                        frame_id_offset = 1;
                        src64_offset = 2;
                        src16_offset = 10;
                        reserved_offset = 12;
                        min_length = 13;
                        field_count = 5;
                        return true;
                case ZBPID_RegisterJoiningDeviceStatus:
                        frame_id_offset = 1;
                        status_offset = 2;
                        min_length = 3;
                        field_count = 3;
                        return true;
                case ZBPID_JoinNotificationStatus:
                        parent16_offset = 1;
                        new16_offset = 3;
                        new64_offset = 5;
                        status_offset = 13;
                        min_length = 14;
                        field_count = 5;
                        return true;
                default:
                        return false;
        }
}

bool ZigBeePacket::build_packet()
{
        // get offsets
        if (!set_offsets())
                return false;
        
        // init payload
        payload.clear();
        payload.push_back(identifier);
        payload.resize(min_length);
        
        // fill in fields
        write_payload_uint8(frame_id_offset, frame_id);
        
        if (at_cmd_offset)
        {
                write_payload_uint8(at_cmd_offset,   at_cmd[0]);
                write_payload_uint8(at_cmd_offset+1, at_cmd[1]);
        }
        
        write_payload_uint8(status_offset, status);
        write_payload_uint8(options_offset, options);
        write_payload_uint64(dest64_offset, dest64);
        write_payload_uint16(dest16_offset, dest16);
        write_payload_uint64(src64_offset, src64);
        write_payload_uint16(src16_offset, src16);
        write_payload_uint64(sender64_offset, sender64);
        write_payload_uint16(sender16_offset, sender16);
        write_payload_uint16(parent16_offset, parent16);
        write_payload_uint64(new64_offset, new64);
        write_payload_uint16(new16_offset, new16);
        write_payload_uint8(src_ep_offset, src_ep);
        write_payload_uint8(dest_ep_offset, dest_ep);
        write_payload_uint16(cluster_id_offset, cluster_id);
        write_payload_uint16(profile_id_offset, profile_id);
        write_payload_uint8(radius_offset, radius);
        write_payload_uint8(transmit_retries_offset, transmit_retries);
        write_payload_uint8(delivery_status_offset, delivery_status);
        write_payload_uint8(discovery_status_offset, discovery_status);
        write_payload_uint8(rssi_offset, rssi);
        write_payload_uint16(digital_mask_offset, digital_mask);
        write_payload_uint8(analog_mask_offset, analog_mask);
        write_payload_uint8(num_samples_offset, num_samples);
        
        if (data_offset)
        {
                payload.insert(payload.begin()+data_offset, data.begin(), data.end());
        }
        
        if (route_records_offset)
        {
                payload.insert(payload.begin()+route_records_offset, route_records.size()*2, 0);
                
                write_payload_uint8(route_records_offset, route_records.size());
                
                for (int i = 0; i < route_records.size(); i++)
                {
                        payload[route_records_offset+1+i*2]   = route_records[i] >> 8;
                        payload[route_records_offset+1+i*2+1] = route_records[i];
                }
        }
}

bool ZigBeePacket::decode_packet()
{
        std::vector<uint8_t>::iterator it = payload.begin();
        
        if (payload.size() < 1)
                return false;
        
        // get identifier
        identifier = ZBP_Identifier(payload[0]);
        
        // get offsets
        if (!set_offsets())
                return false;
        
        // check length
        if (payload.size() < min_length)
                return false;
        
        // read fields
        if (frame_id_offset)
        {
                frame_id = read_payload_uint8(frame_id_offset);
        }
        
        if (at_cmd_offset)
        {
                at_cmd[0] = read_payload_uint8(at_cmd_offset);
                at_cmd[1] = read_payload_uint8(at_cmd_offset+1);
        }
        
        if (status_offset)
        {
                status = read_payload_uint8(status_offset);
        }
        
        if (options_offset)
        {
                options = read_payload_uint8(options_offset);
        }
        
        if (dest64_offset)
        {
                dest64 = read_payload_uint64(dest64_offset);
        }
        
        if (dest16_offset)
        {
                dest16 = read_payload_uint16(dest16_offset);
        }
        
        if (src64_offset)
        {
                src64 = read_payload_uint64(src64_offset);
        }
        
        if (src16_offset)
        {
                src16 = read_payload_uint16(src16_offset);
        }
        
        if (sender64_offset)
        {
                sender64 = read_payload_uint64(sender64_offset);
        }
        
        if (sender16_offset)
        {
                sender16 = read_payload_uint16(sender16_offset);
        }
        
        if (parent16_offset)
        {
                parent16 = read_payload_uint16(parent16_offset);
        }
        
        if (new64_offset)
        {
                new64 = read_payload_uint64(new64_offset);
        }
        
        if (new16_offset)
        {
                new16 = read_payload_uint16(new16_offset);
        }
        
        if (src_ep_offset)
        {
                src_ep = read_payload_uint8(src_ep_offset);
        }
        
        if (dest_ep_offset)
        {
                dest_ep = read_payload_uint8(dest_ep_offset);
        }
        
        if (cluster_id_offset)
        {
                cluster_id = read_payload_uint16(cluster_id_offset);
        }
        
        if (profile_id_offset)
        {
                profile_id = read_payload_uint16(profile_id_offset);
        }
        
        if (radius_offset)
        {
                radius = read_payload_uint8(radius_offset);
        }
        
        if (transmit_retries_offset)
        {
                transmit_retries = read_payload_uint8(transmit_retries_offset);
        }
        
        if (delivery_status_offset)
        {
                delivery_status = read_payload_uint8(delivery_status_offset);
        }
        
        if (discovery_status_offset)
        {
                discovery_status = read_payload_uint8(discovery_status_offset);
        }
        
        if (rssi_offset)
        {
                rssi = read_payload_uint8(rssi_offset);
        }
        
        if (digital_mask_offset)
        {
                digital_mask = read_payload_uint16(digital_mask_offset);
        }
        
        if (analog_mask_offset)
        {
                analog_mask = read_payload_uint8(analog_mask_offset);
        }
        
        if (num_samples_offset)
        {
                num_samples = read_payload_uint8(num_samples_offset);
        }
        
        if (data_offset)
        {
                data.clear();
                data.insert(data.begin(), payload.begin()+data_offset, payload.end());
        }
        
        if (route_records_offset)
        {
                route_records.clear();
                
                for (int i = read_payload_uint8(route_records_offset); i > 0; i--)
                {
                        uint16_t n = (uint16_t)payload[route_records_offset+1+n*2] << 8;
                        n |= (uint16_t)payload[route_records_offset+1+n*2+1];
                        route_records.push_back(n);
                }
        }
}

std::string ZigBeePacket::get_type_desc()
{
        return get_type_desc(identifier);
}

std::string ZigBeePacket::get_desc()
{
        std::stringstream desc;
        
        desc << "ZigBee Packet: " << get_type_desc() << std::endl;
        desc << "  Length: " << std::dec << get_length() << std::endl;
        desc << "  Identifier: 0x" << std::setw(2) << std::hex << identifier << std::endl;
        
        
        for (int i = 1; i <= min_length; i++)
        {
                if (frame_id_offset == i)
                {
                        desc << "  Frame ID: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)frame_id << std::endl;
                }
                
                if (at_cmd_offset == i)
                {
                        desc << "  AT Command: " << at_cmd[0] << at_cmd[1] << std::endl;
                }
                
                if (status_offset == i)
                {
                        desc << "  Status: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)status << std::endl;
                }
                
                if (options_offset == i)
                {
                        desc << "  Options: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)options << std::endl;
                }
                
                if (dest64_offset == i)
                {
                        desc << "  Dest64: 0x" << std::setfill('0') << std::setw(16) << std::hex << dest64 << std::endl;
                }
                
                if (dest16_offset == i)
                {
                        desc << "  Dest16: 0x" << std::setfill('0') << std::setw(4) << std::hex << dest16 << std::endl;
                }
                
                if (src64_offset == i)
                {
                        desc << "  Src64: 0x" << std::setfill('0') << std::setw(16) << std::hex << src64 << std::endl;
                }
                
                if (src16_offset == i)
                {
                        desc << "  Src16: 0x" << std::setfill('0') << std::setw(4) << std::hex << src16 << std::endl;
                }
                
                if (sender64_offset == i)
                {
                        desc << "  Sender64: 0x" << std::setfill('0') << std::setw(16) << std::hex << sender64 << std::endl;
                }
                
                if (sender16_offset == i)
                {
                        desc << "  Sender16: 0x" << std::setfill('0') << std::setw(4) << std::hex << sender16 << std::endl;
                }
                
                if (parent16_offset == i)
                {
                        desc << "  Parent16: 0x" << std::setfill('0') << std::setw(4) << std::hex << parent16 << std::endl;
                }
                
                if (new64_offset == i)
                {
                        desc << "  New64: 0x" << std::setfill('0') << std::setw(16) << std::hex << new64 << std::endl;
                }
                
                if (new16_offset == i)
                {
                        desc << "  New16: 0x" << std::setfill('0') << std::setw(4) << std::hex << new16 << std::endl;
                }
                
                if (src_ep_offset == i)
                {
                        desc << "  Source Endpoint: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)src_ep << std::endl;
                }
                
                if (dest_ep_offset == i)
                {
                        desc << "  Destination Endpoint: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)dest_ep << std::endl;
                }
                
                if (cluster_id_offset == i)
                {
                        desc << "  Cluster ID: 0x" << std::setfill('0') << std::setw(4) << std::hex << cluster_id << std::endl;
                }
                
                if (profile_id_offset == i)
                {
                        desc << "  Profile ID: 0x" << std::setfill('0') << std::setw(4) << std::hex << profile_id << std::endl;
                }
                
                if (radius_offset == i)
                {
                        desc << "  Radius: " << std::dec << (int)radius << std::endl;
                }
                
                if (transmit_retries_offset == i)
                {
                        desc << "  Transmit Retries: " << std::dec << (int)transmit_retries << std::endl;
                }
                
                if (delivery_status_offset == i)
                {
                        desc << "  Delivery Status: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)delivery_status << std::endl;
                }
                
                if (discovery_status_offset == i)
                {
                        desc << "  Discovery Status: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)discovery_status << std::endl;
                }
                
                if (rssi_offset == i)
                {
                        desc << "  RSSI: -" << std::dec << (int)rssi << " dBm" << std::endl;
                }
                
                if (digital_mask_offset == i)
                {
                        desc << "  Digital Mask: 0x" << std::setfill('0') << std::setw(4) << std::hex << digital_mask << std::endl;
                }
                
                if (analog_mask_offset == i)
                {
                        desc << "  Analog Mask: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)analog_mask << std::endl;
                }
                
                if (num_samples_offset == i)
                {
                        desc << "  Samples: " << std::dec << (int)num_samples << std::endl;
                }
                
                if (data_offset == i)
                {
                        desc << "  Data (hex):";
                        for (int i = 0; i < data.size(); i++)
                        {
                                if (i > 0 && i % 16 == 0)
                                        desc << std::endl << "             ";
                                desc << " " << std::setfill('0') << std::setw(2) << std::hex << (int)data[i];
                        }
                        desc << std::endl;
                }
                
                if (route_records_offset == i)
                {
                        desc << "  Route records (hex):";
                        for (int i = 0; i < route_records.size(); i++)
                        {
                                if (i > 0 && i % 16 == 0)
                                        desc << std::endl << "                      ";
                                desc << " " << std::setfill('0') << std::setw(4) << std::hex << route_records[i];
                        }
                        desc << std::endl;
                }
        }
        
        desc << "  Checksum: 0x" << std::setfill('0') << std::setw(2) << std::hex << (int)get_checksum();
        
        return desc.str();
}

std::string ZigBeePacket::get_hex_packet()
{
        std::vector<uint8_t> pkt = get_raw_packet();
        std::stringstream out;
        
        for (int i = 0; i < pkt.size(); i++)
        {
                if (i > 0)
                        out << " ";
                out << std::setfill('0') << std::setw(2) << std::hex << (int)pkt[i];
        }
        
        return out.str();
}


// read and write payload data
uint8_t ZigBeePacket::read_payload_uint8(int offset)
{
        if (offset <= 0)
                return 0;
        return payload[offset];
}

uint16_t ZigBeePacket::read_payload_uint16(int offset)
{
        if (offset <= 0)
                return 0;
        uint16_t value;
        value  = (uint16_t)payload[offset]   << 8;
        value |= (uint16_t)payload[offset+1];
        return value;
}

uint32_t ZigBeePacket::read_payload_uint32(int offset)
{
        if (offset <= 0)
                return 0;
        uint32_t value;
        value  = (uint32_t)payload[offset]   << 24;
        value |= (uint32_t)payload[offset+1] << 16;
        value |= (uint32_t)payload[offset+2] << 8;
        value |= (uint32_t)payload[offset+3];
        return value;
}

uint64_t ZigBeePacket::read_payload_uint64(int offset)
{
        if (offset <= 0)
                return 0;
        uint64_t value;
        value  = (uint64_t)payload[offset]   << 56;
        value |= (uint64_t)payload[offset+1] << 48;
        value |= (uint64_t)payload[offset+2] << 40;
        value |= (uint64_t)payload[offset+3] << 32;
        value |= (uint64_t)payload[offset+4] << 24;
        value |= (uint64_t)payload[offset+5] << 16;
        value |= (uint64_t)payload[offset+6] << 8;
        value |= (uint64_t)payload[offset+7];
        return value;
}

void ZigBeePacket::write_payload_uint8(int offset, uint8_t value)
{
        if (offset <= 0)
                return;
        payload[offset] = value;
}

void ZigBeePacket::write_payload_uint16(int offset, uint16_t value)
{
        if (offset <= 0)
                return;
        payload[offset]   = value >> 8;
        payload[offset+1] = value;
}

void ZigBeePacket::write_payload_uint32(int offset, uint32_t value)
{
        if (offset <= 0)
                return;
        payload[offset]   = value >> 24;
        payload[offset+1] = value >> 16;
        payload[offset+2] = value >> 8;
        payload[offset+3] = value;
}

void ZigBeePacket::write_payload_uint64(int offset, uint64_t value)
{
        if (offset <= 0)
                return;
        payload[offset]   = value >> 56;
        payload[offset+1] = value >> 48;
        payload[offset+2] = value >> 40;
        payload[offset+3] = value >> 32;
        payload[offset+4] = value >> 24;
        payload[offset+5] = value >> 16;
        payload[offset+6] = value >> 8;
        payload[offset+7] = value;
}



