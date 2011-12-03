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

ZigBeePacketBuilder::ZigBeePacketBuilder()
{
        tbl.resize(1, 2);
        tbl.set_col_spacings(10);
        tbl.set_row_spacings(5);
        pack_start(tbl, false, false, 0);
        
        label_type.set_label("Type:");
        tbl.attach(label_type, 0, 1, 0, 1);
        
        std::vector<int> lst = ZigBeePacket::get_valid_identifiers();
        
        for (int i = 0; i < lst.size(); i++)
        {
                std::stringstream ss;
                ss << ZigBeePacket::get_type_desc(lst[i]);
                ss << " (0x" << std::setfill('0') << std::setw(2) << std::hex << lst[i] << ")";
                cmbt_type.append(ss.str());
        }
        
        cmbt_type.set_active(0);
        
        tbl.attach(cmbt_type, 1, 2, 0, 1);
        
}


ZigBeePacketBuilder::~ZigBeePacketBuilder()
{
        
}




