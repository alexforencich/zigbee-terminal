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

#define ZIGBEE_IDENTIFIER 0x7E
#define ZIGBEE_ESCAPE 0x7D

/** ZigBee packet
 * 
 * The ZigBee packet class is used to create and parse ZigBee packets for Digi
 * XBee series 1 and series 2 radios.  
 */
class ZigBeePacket
{
public:
        /**
         * ZigBee Packet Identifiers.
         * All valid ZigBee Packet identifier values.
         */
        typedef enum
        {
                ZBPID_TxRequest64 = 0x00,                       ///< Transmit request, 64-bit address (S1)
                ZBPID_TxRequest16 = 0x01,                       ///< Transmit request, 16-bit address (S1)
                ZBPID_ATCommand = 0x08,                         ///< AT command request
                ZBPID_ATCommandQueueRegisterValue = 0x09,       ///< AT command request, queue value
                ZBPID_TxRequest = 0x10,                         ///< Transmit request (S2)
                ZBPID_EATxRequest = 0x11,                       ///< Transmit request, explicit addressing (S2)
                ZBPID_RemoteATCommand = 0x17,                   ///< Remote AT command request
                ZBPID_CreateSourceRoute = 0x21,                 ///< Create Source Route (S2)
                ZBPID_RegisterJoiningDevice = 0x24,             ///< Register Joining Device (S2)
                ZBPID_RxPacket64 = 0x80,                        ///< Receive packet, 64-bit address (S1)
                ZBPID_RxPacket16 = 0x81,                        ///< Receive packet, 16-bit address (S1)
                ZBPID_RxPacketIO64 = 0x82,                      ///< Receive IO packet, 64-bit address (S1)
                ZBPID_RxPacketIO16 = 0x83,                      ///< Receive IO packet, 16-bit address (S1)
                ZBPID_ATCommandResponse = 0x88,                 ///< AT command response
                ZBPID_TxStatusS1 = 0x89,                        ///< Transmit status (S1)
                ZBPID_ModemStatus = 0x8a,                       ///< Modem status
                ZBPID_TxStatusS2 = 0x8b,                        ///< Transmit status (S2)
                ZBPID_RxPacket = 0x90,                          ///< Receive packet (S2)
                ZBPID_EARxPacket = 0x91,                        ///< Receive packet, explicit addressing (S2)
                ZBPID_IODataSampleRx = 0x92,                    ///< IO data sample receive packet (S2)
                ZBPID_SensorRead = 0x94,                        ///< Sensor read (S2)
                ZBPID_NodeIdentification = 0x95,                ///< Node identification (S2)
                ZBPID_RemoteCommandResponse = 0x97,             ///< Remote command response
                ZBPID_OTAFirmwareUpdateStatus = 0xa0,           ///< Over-the-air firmware update status (S2)
                ZBPID_RouteRecord = 0xa1,                       ///< Route record (S2)
                ZBPID_DeviceAuthenticated = 0xa2,               ///< Device authenticated (S2)
                ZBPID_ManyToOneRouteRequest = 0xa3,             ///< Many-to-one route request (S2)
                ZBPID_RegisterJoiningDeviceStatus = 0xa4,       ///< Register joining device status (S2)
                ZBPID_JoinNotificationStatus = 0xa5,            ///< Join notification status (S2)
        }
        ZBP_Identifier;
        
        // Data must be written in big endian
        struct sZBP_TxRequest64
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_TxRequest16
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint16_t dest16;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_ATCommand
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t at_cmd[2];
                uint8_t at_cmd_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_ATCommandQueueRegisterValue
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t at_cmd[2];
                uint8_t at_cmd_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_TxRequest
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t radius;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
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
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RemoteATCommand
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t options;
                uint8_t at_cmd[2];
                uint8_t at_cmd_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_CreateSourceRoute
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t options;
                uint8_t num_route_records;
                uint16_t route_records[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RegisterJoiningDevice
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t dest64;
                uint16_t dest16;
                uint8_t options;
                uint8_t key[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RxPacket64
        {
                uint8_t identifier;
                uint64_t src64;
                uint8_t rssi;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RxPacket16
        {
                uint8_t identifier;
                uint64_t src16;
                uint8_t rssi;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RxPacketIO64
        {
                uint8_t identifier;
                uint64_t src64;
                uint8_t rssi;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RxPacketIO16
        {
                uint8_t identifier;
                uint64_t src16;
                uint8_t rssi;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_ATCommandResponse
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t at_cmd[2];
                uint8_t status;
                uint8_t at_cmd_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_TxStatusS1
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t status;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_ModemStatus
        {
                uint8_t identifier;
                uint8_t status;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_TxStatusS2
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint16_t dest16;
                uint8_t transmit_retries;
                uint8_t delivery_status;
                uint8_t discovery_status;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RxPacket
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t rf_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
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
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_IODataSampleRx
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t num_samples;
                uint16_t digital_mask;
                uint8_t analog_mask;
                uint8_t sample[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_SensorRead
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t one_wire_sensors;
                uint16_t analog_sensors[4];
                uint16_t temp_read;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_NodeIdentification
        {
                uint8_t identifier;
                uint64_t sender64;
                uint16_t sender16;
                uint8_t options;
                uint16_t src16;
                uint64_t src64;
                uint8_t other_fields[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RemoteCommandResponse
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t src64;
                uint16_t src16;
                uint8_t at_cmd[2];
                uint8_t status;
                uint8_t at_cmd_data[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_OTAFirmwareUpdateStatus
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t bootloader_msg_type;
                uint8_t block_num;
                uint64_t target64;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RouteRecord
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t options;
                uint8_t num_route_records;
                uint16_t route_records[];
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_DeviceAuthenticated
        {
                uint8_t identifier;
                uint64_t src64;
                uint16_t src16;
                uint8_t status;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_ManyToOneRouteRequest
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint64_t src64;
                uint16_t src16;
                uint8_t reserved;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_RegisterJoiningDeviceStatus
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint8_t status;
        } __attribute__ ((gcc_struct, __packed__));
        
        struct sZBP_JoinNotificationStatus
        {
                uint8_t identifier;
                uint8_t frame_id;
                uint16_t parent16;
                uint16_t new16;
                uint64_t new64;
                uint8_t status;
        } __attribute__ ((gcc_struct, __packed__));
        
        ZigBeePacket();
        virtual ~ZigBeePacket();
        
        // overall packet
        /**
         * Raw payload data.
         * Raw packet payload data without identifier byte, size, and checksum,
         * used as an intermediary with packets being read into payload and
         * then parsed out into fields or assembled into payload and then
         * transmitted.  
         */
        std::vector<uint8_t> payload;
        
        // packet fields
        
        ZBP_Identifier identifier;      ///< Packet type identifier
        uint8_t frame_id;               ///< Frame ID field
        int frame_id_offset;            ///< Frame ID field offset
        
        // AT commands
        uint8_t at_cmd[2];              ///< AT command field
        int at_cmd_offset;              ///< AT command field offset
        
        // general fields
        uint8_t status;                 ///< Status field
        int status_offset;              ///< Status field offset
        uint8_t options;                ///< Options field
        int options_offset;             ///< Options field offset
        int reserved_offset;            ///< Reserved offset
        int min_length;                 ///< Min length
        int field_count;                ///< Field count
        
        // addressing
        uint64_t dest64;                ///< Destination 64-bit address field
        int dest64_offset;              ///< Destination 64-bit address field offset
        uint16_t dest16;                ///< Destination 16-bit address field
        int dest16_offset;              ///< Destination 16-bit address field offset
        uint64_t src64;                 ///< Source 64-bit address field
        int src64_offset;               ///< Source 64-bit address field offset
        uint16_t src16;                 ///< Source 16-bit address field
        int src16_offset;               ///< Source 16-bit address field offset
        uint64_t sender64;              ///< Sender 64-bit address field
        int sender64_offset;            ///< Sender 64-bit address field offset
        uint16_t sender16;              ///< Sender 16-bit address field
        int sender16_offset;            ///< Sender 16-bit address field offset
        uint16_t parent16;              ///< Parent 16-bit address field
        int parent16_offset;            ///< Parent 16-bit address field offset
        uint64_t new64;                 ///< New 64-bit address field
        int new64_offset;               ///< New 64-bit address field offset
        uint16_t new16;                 ///< New 16-bit address field
        int new16_offset;               ///< New 16-bit address field offset
        
        // explicit addressing
        uint8_t src_ep;                 ///< Source endpoint field
        int src_ep_offset;              ///< Source endpoint field offset
        uint8_t dest_ep;                ///< Destination endpoint field
        int dest_ep_offset;             ///< Destination endpoint field offset
        uint16_t cluster_id;            ///< Cluster ID field
        int cluster_id_offset;          ///< Cluster ID field offset
        uint16_t profile_id;            ///< Profile ID field
        int profile_id_offset;          ///< Profile ID field offset
        
        // transmit parameters
        uint8_t radius;                 ///< Radius field
        int radius_offset;              ///< Radius field offset
        uint8_t transmit_retries;       ///< Transmit retries field
        int transmit_retries_offset;    ///< Transmit retries field offset
        uint8_t delivery_status;        ///< Delivery status field
        int delivery_status_offset;     ///< Delivery status field offset
        uint8_t discovery_status;       ///< Discovery status field
        int discovery_status_offset;    ///< Discovery status field offset
        
        // RSSI
        uint8_t rssi;                   ///< Receive signal strength indication field
        int rssi_offset;                ///< Receive signal strength indication field offset
        
        // sampling
        uint16_t digital_mask;          ///< Digital mask field
        int digital_mask_offset;        ///< Digital mask field offset
        uint8_t analog_mask;            ///< Analog mask field
        int analog_mask_offset;         ///< Analog mask field offset
        uint8_t num_samples;            ///< Num samples field
        int num_samples_offset;         ///< Num samples field offset
        
        // packet data (rf, at, etc.)
        std::vector<uint8_t> data;      ///< Packet data field
        int data_offset;                ///< Packet data field offset
        
        // routing
        std::vector<uint16_t> route_records;    ///< Route records field
        int route_records_offset;               ///< Route records field offset
        
        /**
         * Zero out all packet fields.
         */
        void zero();
        
        /**
         * Get size of packet and header.
         * @return packet size in bytes
         */
        uint16_t get_length();
        
        /**
         * Calculate checksum of payload.
         * @return checksum byte
         */
        uint8_t get_checksum();
        
        /**
         * Get raw packet data, including identifier and size.
         * @return vector containing raw packet data
         */
        std::vector<uint8_t> get_raw_packet();
        
        /**
         * Get raw packet data, including identifier and size, with escaped bytes.
         * @return vector containing raw, escaped packet data
         */
        std::vector<uint8_t> get_escaped_raw_packet();
        
        /**
         * Try to read packet from a vector of bytes. Looks for identifier
         * byte to indicate start of packet.
         * @param bytes vector of type char to read from
         * @param bytes_read Return number bytes read
         * @return true if packet read, false if not
         */
        bool read_packet(std::vector<char> bytes, size_t &bytes_read);
        
        /**
         * Try to read packet from a vector of bytes. Looks for identifier
         * byte to indicate start of packet.
         * @param bytes vector of type uint8_t to read from
         * @param bytes_read Return number bytes read
         * @return true if packet read, false if not
         */
        bool read_packet(std::vector<uint8_t> bytes, size_t &bytes_read);
        
        /**
         * Try to read packet from a deque of bytes. Looks for identifier
         * byte to indicate start of packet.
         * @param bytes deque of type char to read from
         * @param bytes_read Return number bytes read
         * @return true if packet read, false if not
         */
        bool read_packet(std::deque<char> bytes, size_t &bytes_read);
        
        /**
         * Try to read packet from a deque of bytes. Looks for identifier
         * byte to indicate start of packet.
         * @param bytes deque of type char to read from
         * @param bytes_read Return number bytes read
         * @return true if packet read, false if not
         */
        bool read_packet(std::deque<uint8_t> bytes, size_t &bytes_read);
        
        /**
         * Try to read packet from a deque of bytes. Looks for identifier
         * byte to indicate start of packet.
         * @param bytes deque of type uint8_t to read from
         * @param bytes_read Return number bytes read
         * @return true if packet read, false if not
         */
        bool read_packet(uint8_t *bytes, size_t count, size_t &bytes_read);
        
        /**
         * Configure field offsets based on identifier.
         * @return true if success, false if bad identifier
         */
        bool set_offsets();
        
        /**
         * Populate payload with header fields and data.
         * @return true if packet successfully built
         * @see payload
         * @see decode_packet()
         */
        bool build_packet();
        
        /**
         * Parse bytes in payload and extract header fields and data.
         * @return true if packet successfully parsed
         * @see payload
         * @see build_packet()
         */
        bool decode_packet();
        
        /**
         * Get a string description of the packet type.
         * @return description string
         * @see ZBP_Identifier
         */
        std::string get_type_desc();
        
        /**
         * Get a string representation of the entire packet, field by field.
         * @return description string
         */
        std::string get_desc();
        
        /**
         * Raw packet data in hex.  Returns the result of get_raw_packet,
         * converted to a hex string.  
         * @return packet data in hex
         * @see get_raw_packet()
         */
        std::string get_hex_packet();
        
        /**
         * Check identifier
         * @return true if valid identifier
         * @see ZBP_Identifier
         */
        static bool is_valid_identifier(int identifier);
        
        /**
         * Get list of valid identifiers
         * @return vector of valid identifiers
         * @see ZBP_Identifier
         */
        static std::vector<int> get_valid_identifiers();
        
        /**
         * Get a string description for a given identifier.
         * @return description string
         * @see ZBP_Identifier
         */
        static std::string get_type_desc(int identifier);
        
protected:
        // read and write payload data
        /**
         * Read an 8 bit integer from payload
         * @param offset offset to read from
         * @return read value
         * @see payload
         */
        uint8_t read_payload_uint8(int offset);
        /**
         * Read a 16 bit integer from payload
         * @param offset offset to read from
         * @return read value
         * @see payload
         */
        uint16_t read_payload_uint16(int offset);
        /**
         * Read a 32 bit integer from payload
         * @param offset offset to read from
         * @return read value
         * @see payload
         */
        uint32_t read_payload_uint32(int offset);
        /**
         * Read a 64 bit integer from payload
         * @param offset offset to read from
         * @return read value
         * @see payload
         */
        uint64_t read_payload_uint64(int offset);
        /**
         * Write an 8 bit integer to payload
         * @param offset offset to write to
         * @param value value to write
         * @see payload
         */
        void write_payload_uint8(int offset, uint8_t value);
        /**
         * Write a 16 bit integer to payload
         * @param offset offset to write to
         * @param value value to write
         * @see payload
         */
        void write_payload_uint16(int offset, uint16_t value);
        /**
         * Write a 32 bit integer to payload
         * @param offset offset to write to
         * @param value value to write
         * @see payload
         */
        void write_payload_uint32(int offset, uint32_t value);
        /**
         * Write a 64 bit integer to payload
         * @param offset offset to write to
         * @param value value to write
         * @see payload
         */
        void write_payload_uint64(int offset, uint64_t value);
        
};

#endif //__ZIGBEE_PACKET_H
