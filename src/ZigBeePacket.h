/************************************************************************/
/* ZigBeePacket                                                         */
/*                                                                      */
/* ZigBee Terminal - ZigBee Packet                                      */
/*                                                                      */
/* ZigBeePacket.h                                                       */
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

#ifndef __ZIGBEE_PACKET_H
#define __ZIGBEE_PACKET_H

#include <string>
#include <vector>
#include <deque>
#include <inttypes.h>

// ZigBeePacket class
class ZigBeePacket
{
public:
        typedef enum
        {
                ZBPID_ATCommand = 0x08,
                ZBPID_ATCommandQueueRegisterValue = 0x09,
                ZBPID_TxRequest = 0x10,
                ZBPID_EATxRequest = 0x11,
                ZBPID_RemoteATCommand = 0x17,
                ZBPID_CreateSourceRoute = 0x21,
                ZBPID_RegisterJoiningDevice = 0x24,
                ZBPID_ATCommandResponse = 0x88,
                ZBPID_ModemStatus = 0x8a,
                ZBPID_TxStatus = 0x8b,
                ZBPID_RxPacket = 0x90,
                ZBPID_EARxPacket = 0x91,
                ZBPID_IODataSampleRx = 0x92,
                ZBPID_SensorRead = 0x94,
                ZBPID_NodeIdentification = 0x95,
                ZBPID_RemoteCommandResponse = 0x97,
                ZBPID_OTAFirmwareUpdateStatus = 0xa0,
                ZBPID_RouteRecord = 0xa1,
                ZBPID_DeviceAuthenticated = 0xa2,
                ZBPID_ManyToOneRouteRequest = 0xa3,
                ZBPID_RegisterJoiningDeviceStatus = 0xa4,
                ZBPID_JoinNotificationStatus = 0xa5,
        }
        ZBP_Identifier;
        
        // Data must be written in big endian
        struct sZBP_ATCommand
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t at_cmd[2];
                uint8_t at_cmd_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_ATCommandQueueRegisterValue
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t at_cmd[2];
                uint8_t at_cmd_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_TxRequest
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t radius;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_EATxRequest
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t src_ep;
                uint8_t dest_ep;
                uint16_t cluster_id;
                uint16_t profile_id;
                uint8_t radius;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_RemoteATCommand
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t options;
                uint8_t at_cmd[2];
                uint8_t at_cmd_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_CreateSourceRoute
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t options;
                uint8_t num_route_records;
                uint16_t route_records[];
        } __attribute__ ((__packed__));
        
        struct sZBP_RegisterJoiningDevice
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t options;
                uint8_t key[];
        } __attribute__ ((__packed__));
        
        struct sZBP_ATCommandResponse
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t at_cmd[2];
                uint8_t status;
                uint8_t at_cmd_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_ModemStatus
        {
                uint8_t identifier;
                uint8_t status;
        } __attribute__ ((__packed__));
        
        struct sZBP_TxStatus
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint16_t dest16;
                uint8_t transmit_retries;
                uint8_t delivery_status;
                uint8_t discovery_status;
        } __attribute__ ((__packed__));
        
        struct sZBP_RxPacket
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_EARxPacket
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t src_ep;
                uint8_t dest_ep;
                uint16_t cluster_id;
                uint16_t profile_id;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_IODataSampleRx
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t num_samples;
                uint16_t digital_mask;
                uint8_t analog_mask;
                uint8_t sample;
        } __attribute__ ((__packed__));
        
        struct sZBP_SensorRead
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t one_wire_sensors;
                uint16_t analog_sensors[4];
                uint16_t temp_read;
        } __attribute__ ((__packed__));
        
        struct sZBP_NodeIdentification
        {
                uint8_t identifier;
                uint64_t sender64;
                uint16_t sender16;
                uint8_t options;
                uint16_t src16;
                uint64_t src64;
                uint8_t other_fields[];
        } __attribute__ ((__packed__));
        
        struct sZBP_RemoteCommandResponse
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t src64;
                uint16_t src16;
                uint8_t at_cmd[2];
                uint8_t status;
                uint8_t at_cmd_data[];
        } __attribute__ ((__packed__));
        
        struct sZBP_OTAFirmwareUpdateStatus
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t bootloader_msg_type;
                uint8_t block_num;
                uint64_t target64;
        } __attribute__ ((__packed__));
        
        struct sZBP_RouteRecord
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t num_route_records;
                uint16_t route_records[];
        } __attribute__ ((__packed__));
        
        struct sZBP_DeviceAuthenticated
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t status;
        } __attribute__ ((__packed__));
        
        struct sZBP_ManyToOneRouteRequest
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t src64;
                uint16_t src16;
                uint8_t reserved;
        } __attribute__ ((__packed__));
        
        struct sZBP_RegisterJoiningDeviceStatus
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t status;
        } __attribute__ ((__packed__));
        
        struct sZBP_JoinNotificationStatus
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint16_t parent16;
                uint16_t new16;
                uint64_t new64;
                uint8_t status;
        } __attribute__ ((__packed__));
        
        // overall packet
        std::vector<uint8_t> payload;
        
        // packet fields
        
        ZBP_Identifier identifier;
        uint8_t frame_id;
        int frame_id_offset;
        
        // AT commands
        uint8_t at_cmd[2];
        int at_cmd_offset;
        
        // general fields
        uint8_t status;
        int status_offset;
        uint8_t options;
        int options_offset;
        int reserved_offset;
        int min_length;
        int field_count;
        
        // addressing
        uint64_t dest64;
        int dest64_offset;
        uint16_t dest16;
        int dest16_offset;
        uint64_t src64;
        int src64_offset;
        uint16_t src16;
        int src16_offset;
        
        // explicit addressing
        uint8_t src_ep;
        int src_ep_offset;
        uint8_t dest_ep;
        int dest_ep_offset;
        uint16_t cluster_id;
        int cluster_id_offset;
        uint16_t profile_id;
        int profile_id_offset;
        
        // transmit parameters
        uint8_t radius;
        int radius_offset;
        uint8_t transmit_retries;
        int transmit_retries_offset;
        uint8_t delivery_status;
        int delivery_status_offset;
        uint8_t discovery_status;
        int discovery_status_offset;
        
        // sampling
        uint16_t digital_mask;
        int digital_mask_offset;
        uint8_t analog_mask;
        int analog_mask_offset;
        
        // packet data (rf, at, etc.)
        std::vector<uint8_t> data;
        int data_offset;
        
        // routing
        std::vector<uint16_t> route_records;
        int route_records_offset;
        
        uint16_t get_length();
        uint8_t get_checksum();
        std::vector<uint8_t> get_raw_packet();
        std::vector<uint8_t> get_escaped_raw_packet();
        bool read_packet(std::vector<char> bytes, size_t &bytes_read);
        bool read_packet(std::vector<uint8_t> bytes, size_t &bytes_read);
        bool read_packet(std::deque<char> bytes, size_t &bytes_read);
        bool read_packet(std::deque<uint8_t> bytes, size_t &bytes_read);
        bool read_packet(uint8_t *bytes, size_t count, size_t &bytes_read);
        bool set_offsets();
        bool build_packet();
        bool decode_packet();
        
        std::string get_type_desc();
        std::string get_desc();
        std::string get_hex_packet();
        
protected:
};

#endif //__ZIGBEE_PACKET_H
