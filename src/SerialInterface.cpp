/************************************************************************/
/* SerialInterface                                                      */
/*                                                                      */
/* ZigBee Terminal - Serial Port Interface                              */
/*                                                                      */
/* SerialInterface.cpp                                                  */
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

#include "SerialInterface.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "alphanum.h"

std::vector<std::string> SerialInterface::enumerate_ports()
{
        std::vector<std::string> SerialDeviceList = std::vector<std::string>();
        DIR *dp;
        struct dirent *dirp;
        std::string f, d;
        std::vector<std::string>::iterator it;
        char buf[PATH_MAX];
        
        struct serial_struct serinfo;
        int fd;
        
        if ((dp = opendir("/dev/")) == NULL)
        {
                std::cout << "Error (" << errno << ") opening /dev/" << std::endl;
        }
        else
        {
                while ((dirp = readdir(dp)) != NULL)
                {
                        f = dirp->d_name;
                        d = "/dev/" + f;
                        if (f.find("ttyS") == 0)
                        {
                                if ((fd = ::open(d.c_str(), O_RDWR|O_NONBLOCK)) < 0)
                                {
                                        std::cout << "Cannot open port " << d << std::endl;
                                        continue;
                                }
                                
                                serinfo.reserved_char[0] = 0;
                                
                                if (::ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
                                {
                                        std::cout << "Cannot get serial info for " << d << std::endl;
                                        ::close(fd);
                                        continue;
                                }
                                
                                if (serinfo.port != 0)
                                        SerialDeviceList.push_back(d);
                                
                                ::close(fd);
                                
                                continue;
                        }
                        if (f.find("ttyUSB") == 0)
                        {
                                SerialDeviceList.push_back(d);
                        }
                }
                
                closedir(dp);
        }
        
        if ((dp = opendir("/dev/serial/by-id/")) != NULL)
        {
                while ((dirp = readdir(dp)) != NULL)
                {
                        f = dirp->d_name;
                        if (f == "." || f == "..")
                                continue;
                        f = "/dev/serial/by-id/" + f;
                        if (realpath(f.c_str(), buf));
                        {
                                f = buf;
                                SerialDeviceList.push_back(f);
                        }
                }
                
                closedir(dp);
        }
        
        // sort it
        sort(SerialDeviceList.begin(), SerialDeviceList.end(), doj::alphanum_less<std::string>());
        
        // remove duplicates
        SerialDeviceList.erase(std::unique(SerialDeviceList.begin(), SerialDeviceList.end()), SerialDeviceList.end());
        
        return SerialDeviceList;
}

SerialInterface::SerialInterface()
{
        port_fd = -1;
        baud = 0;
}

SerialInterface::~SerialInterface()
{
        close_port();
}

bool SerialInterface::port_callback(Glib::IOCondition io_condition)
{
        static char buf[1024];
        gsize num;
        int st;
        int i;
        char ch;
        
        /*num = 1024;
        
        while (num == 1024)
        {
                st = read(buf, 1024, num);
                for (i = 0; i < num; i++)
                        read_data_queue.push_back(buf[i]);
        }*/
        
        m_port_receive_data.emit();
        
        return true;
}

int SerialInterface::write(const char *buf, gsize count, gsize& bytes_written)
{
        int st;
        gsize i;
        
        if (port_fd == -1)
                return I_PORT_NOT_OPEN;
        
        try
        {
                st = port_iochannel->write(buf, count, bytes_written);
                port_iochannel->flush();
        }
        catch (Glib::IOChannelError &ex)
        {
                std::cout << "Error writing serial port: (" << ex.code() << ") " << ex.what() << std::endl;
                m_port_error.emit();
                close_port();
                return I_ERROR;
        }
        
        if (bytes_written > 0)
        {
                std::cout << "Write: ";
                for (i = 0; i < bytes_written; i++)
                        std::cout << std::setfill('0') << std::setw(2) << std::hex << ((uint)buf[i] & 0xff) << ' ';
                std::cout << std::endl;
        }
        
        if (st == Glib::IO_STATUS_NORMAL)
                return I_SUCCESS;
        
        return I_ERROR;
}

int SerialInterface::read(char *buf, gsize count, gsize& bytes_read)
{
        int st;
        gsize i;
        
        if (port_fd == -1)
                return I_PORT_NOT_OPEN;
        
        try
        {
                st = port_iochannel->read(buf, count, bytes_read);
        }
        catch (Glib::IOChannelError &ex)
        {
                std::cout << "Error reading serial port: (" << ex.code() << ") " << ex.what() << std::endl;
                m_port_error.emit();
                close_port();
                return I_ERROR;
        }
        
        if (bytes_read > 0)
        {
                std::cout << "Read: ";
                for (i = 0; i < bytes_read; i++)
                        std::cout << std::setfill('0') << std::setw(2) << std::hex << ((uint)buf[i] & 0xff) << ' ';
                std::cout << std::endl;
        }
        
        if (st == Glib::IO_STATUS_NORMAL)
                return I_SUCCESS;
        
        return I_ERROR;
}

int SerialInterface::open_port()
{
        struct termios t;
        
        close_port();
        
        port_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        
        if (port_fd == -1)
        {
                std::cout << "Error opening port " + port << std::endl;
                return I_ERROR;
        }
        
        tcgetattr(port_fd, &t);
        memcpy(&port_termios_saved, &t, sizeof(struct termios));
        
        switch (baud)
        {
                case 300:
                        t.c_cflag = B300;
                        break;
                case 600:
                        t.c_cflag = B600;
                        break;
                case 1200:
                        t.c_cflag = B1200;
                        break;
                case 2400:
                        t.c_cflag = B2400;
                        break;
                case 4800:
                        t.c_cflag = B4800;
                        break;
                case 9600:
                        t.c_cflag = B9600;
                        break;
                case 19200:
                        t.c_cflag = B19200;
                        break;
                case 38400:
                        t.c_cflag = B38400;
                        break;
                case 57600:
                        t.c_cflag = B57600;
                        break;
                case 115200:
                        t.c_cflag = B115200;
                        break;
        }
        
        // 8 bits
        t.c_cflag |= CS8;
        
        t.c_cflag |= CREAD;
        
        // no flow control
        t.c_cflag |= CLOCAL;
        
        t.c_iflag = IGNPAR | IGNBRK;
        t.c_oflag = 0;
        t.c_lflag = 0;
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 1;
        tcsetattr(port_fd, TCSANOW, &t);
        tcflush(port_fd, TCOFLUSH);  
        tcflush(port_fd, TCIFLUSH);
        
        reset_buffer();
        
        port_iochannel = Glib::IOChannel::create_from_fd(port_fd);
        port_iochannel->set_encoding("");
        port_callback_conn = Glib::signal_io().connect(sigc::mem_fun(*this, &SerialInterface::port_callback), port_fd, Glib::IO_IN);
        
        std::cout << "Port opened." << std::endl;
        
        m_port_opened.emit();
        
        return I_SUCCESS;
}

int SerialInterface::close_port()
{
        if (port_fd != -1)
        {
                port_callback_conn.disconnect();
                
                try
                {
                        port_iochannel->close();
                }
                catch (Glib::IOChannelError &ex)
                {
                        // ignore iochannel error
                }
                
                tcsetattr(port_fd, TCSANOW, &port_termios_saved);
                tcflush(port_fd, TCOFLUSH);  
                tcflush(port_fd, TCIFLUSH);
                close(port_fd);
                port_fd = -1;
                
                std::cout << "Port closed." << std::endl;
                
                m_port_closed.emit();
        }
        
        return I_SUCCESS;
}

void SerialInterface::reset_buffer()
{
        read_data_queue.clear();
}

Glib::ustring SerialInterface::set_port(Glib::ustring p)
{
        if (!is_open())
                port = p;
        
        return port;
}

Glib::ustring SerialInterface::get_port()
{
        return port;
}

unsigned long SerialInterface::set_baud(unsigned long b)
{
        if (!is_open())
                baud = b;
        
        return baud;
}

unsigned long SerialInterface::get_baud()
{
        return baud;
}

bool SerialInterface::is_open()
{
        return (port_fd != -1);
}

sigc::signal<void> SerialInterface::port_opened()
{
        return m_port_opened;
}

sigc::signal<void> SerialInterface::port_closed()
{
        return m_port_closed;
}

sigc::signal<void> SerialInterface::port_error()
{
        return m_port_error;
}

sigc::signal<void> SerialInterface::port_receive_data()
{
        return m_port_receive_data;
}





