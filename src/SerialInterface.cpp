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

#ifdef __unix__

#include <sys/ioctl.h>
#include <linux/serial.h>

#endif

#include <iostream>
#include <iomanip>

#include "alphanum.h"

std::vector<std::string> SerialInterface::enumerate_ports()
{
        std::vector<std::string> SerialDeviceList = std::vector<std::string>();
        
        #ifdef __unix__
        
        DIR *dp;
        struct dirent *dirp;
        std::string f, d;
        std::vector<std::string>::iterator it;
        char buf[PATH_MAX];
        
        struct serial_struct serinfo;
        int fd;
        
        if ((dp = opendir("/dev/")) == NULL)
        {
                std::cerr << "Error (" << errno << ") opening /dev/" << std::endl;
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
                                        std::cerr << "Cannot open port " << d << std::endl;
                                        continue;
                                }
                                
                                serinfo.reserved_char[0] = 0;
                                
                                if (::ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
                                {
                                        std::cerr << "Cannot get serial info for " << d << std::endl;
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
        
        #elif defined _WIN32
        
        TCHAR szDevices[65535];
        unsigned long dwChars = QueryDosDevice(NULL, szDevices, 65535);
        TCHAR *ptr = szDevices;
        TCHAR *temp_ptr;
        std::string c;
        
        while (dwChars)
        {
                int port;
                
                if (sscanf(ptr, "COM%d", &port) == 1)
                {
                        c = ptr;
                        SerialDeviceList.push_back(c);
                }
                
                temp_ptr = strchr(ptr, 0);
                dwChars -= (DWORD)((temp_ptr - ptr) / sizeof(TCHAR) + 1);
                ptr = temp_ptr + 1;
        }
        
        #endif
        
        // sort it
        sort(SerialDeviceList.begin(), SerialDeviceList.end(), doj::alphanum_less<std::string>());
        
        // remove duplicates
        SerialDeviceList.erase(std::unique(SerialDeviceList.begin(), SerialDeviceList.end()), SerialDeviceList.end());
        
        return SerialDeviceList;
}

SerialInterface::SerialInterface()
{
        #ifdef __unix__
        
        port_fd = -1;
        
        #elif defined _WIN32
        
        h_port = INVALID_HANDLE_VALUE;
        h_overlapped = INVALID_HANDLE_VALUE;
        h_overlapped_thread = INVALID_HANDLE_VALUE;
        
        #endif
        
        baud = 0;
        signal_receive_data.connect( sigc::mem_fun(*this, &SerialInterface::on_receive_data) );
}

SerialInterface::~SerialInterface()
{
        close_port();
}

void SerialInterface::on_receive_data()
{
        m_port_receive_data.emit();
}

void SerialInterface::launch_select_thread()
{
        Glib::Thread::create( sigc::mem_fun(*this, &SerialInterface::select_thread), false );
}

void SerialInterface::select_thread()
{
        #ifdef __unix__
        
        int n, max_fd;
        fd_set input;
        struct timeval timeout;
        
        FD_ZERO(&input);
        FD_SET(port_fd, &input);
        max_fd = port_fd + 1;
        
        while (is_open())
        {
                timeout.tv_sec = 1;
                timeout.tv_usec = 0;
                
                n = select(max_fd, &input, NULL, NULL, &timeout);
                
                if (n < 0)
                {
                        std::cerr << "Error: select failed!" << std::endl;
                        return;
                }
                else if (n == 0)
                {
                        // timeout...
                }
                else
                {
                        if (FD_ISSET(port_fd, &input))
                        {
                                signal_receive_data.emit();
                        }
                }
        }
        
        #elif defined _WIN32
        
        std::cout << "Thread start" << std::endl;
        
        while (is_open())
        {
                ResetEvent(h_overlapped_thread);
                
                DWORD e_event = 0;
                
                OVERLAPPED ov;
                memset(&ov, 0, sizeof(OVERLAPPED));
                ov.hEvent = h_overlapped_thread;
                
                if (!WaitCommEvent(h_port, &e_event, &ov))
                {
                        if (GetLastError() != ERROR_IO_PENDING)
                        {
                                std::cerr << "Unable to wait for COM event (" << GetLastError() << ")" << std::endl;
                                return;
                        }
                }
                
                if (WaitForSingleObject(h_overlapped_thread,INFINITE) != WAIT_OBJECT_0)
                {
                        std::cerr << "Unable to wait until COM event has arrived" << std::endl;
                        return;
                }
                
                if (e_event == EV_RXCHAR)
                        signal_receive_data.emit();
        }
        
        #endif
}

int SerialInterface::write(const char *buf, gsize count, gsize& bytes_written)
{
        #ifdef __WIN32
        DWORD d;
        #endif
        
        if (!is_open())
                return I_PORT_NOT_OPEN;
        
        #ifdef __unix__
        
        bytes_written = ::write(port_fd, buf, count);
        
        if (bytes_written == -1)
        {
                std::cerr << "Error writing serial port (errno " << errno << ")" << std::endl;
                m_port_error.emit();
                close_port();
                return I_ERROR;
        }
        
        #elif defined _WIN32
        
        OVERLAPPED ov;
        memset(&ov, 0, sizeof(OVERLAPPED));
        ov.hEvent = h_overlapped;
        
        if (!WriteFile(h_port, buf, count, &d, &ov))
        {
                if (GetLastError() != ERROR_IO_PENDING)
                {
                        std::cerr << "Error writing serial port (" << GetLastError() << ")" << std::endl;
                        return I_ERROR;
                }
                switch (WaitForSingleObject(ov.hEvent, INFINITE))
                {
                case WAIT_OBJECT_0:
                        if (!GetOverlappedResult(h_port, &ov, &d, TRUE))
                        {
                                std::cerr << "Overlapped completed without result (" << GetLastError() << ")" << std::endl;
                                return I_ERROR;
                        }
                        break;
                case WAIT_TIMEOUT:
                        CancelIo(h_port);
                        std::cerr << "Timeout" << std::endl;
                        return I_ERROR;
                default:
                        std::cerr << "Unable to wait until data has been sent (" << GetLastError() << ")" << std::endl;
                        return I_ERROR;
                }
        }
        else
        {
                SetEvent(ov.hEvent);
        }
        
        bytes_written = d;
        
        #endif
        
        if (bytes_written > 0)
        {
                std::cout << "Write: ";
                for (gsize i = 0; i < bytes_written; i++)
                        std::cout << std::setfill('0') << std::setw(2) << std::hex << ((unsigned int)buf[i] & 0xff) << ' ';
                std::cout << std::endl;
        }
        
        return I_SUCCESS;
}

int SerialInterface::read(char *buf, gsize count, gsize& bytes_read)
{
        #ifdef __WIN32
        DWORD d;
        #endif
        
        if (!is_open())
                return I_PORT_NOT_OPEN;
        
        #ifdef __unix__
        
        bytes_read = ::read(port_fd, buf, count);
        
        if (bytes_read == -1)
        {
                std::cerr << "Error reading serial port (errno " << errno << ")" << std::endl;
                m_port_error.emit();
                close_port();
                return I_ERROR;
        }
        
        #elif defined _WIN32
        
        OVERLAPPED ov;
        memset(&ov, 0, sizeof(OVERLAPPED));
        ov.hEvent = h_overlapped;
        
        if (!ReadFile(h_port, buf, count, &d, &ov))
        {
                if (GetLastError() != ERROR_IO_PENDING)
                {
                        std::cerr << "Error reading serial port (" << GetLastError() << ")" << std::endl;
                        return I_ERROR;
                }
                switch (WaitForSingleObject(ov.hEvent, INFINITE))
                {
                case WAIT_OBJECT_0:
                        if (!GetOverlappedResult(h_port, &ov, &d, FALSE))
                        {
                                std::cerr << "Overlapped completed without result (" << GetLastError() << ")" << std::endl;
                                return I_ERROR;
                        }
                        break;
                case WAIT_TIMEOUT:
                        CancelIo(h_port);
                        std::cerr << "Timeout" << std::endl;
                        return I_ERROR;
                default:
                        std::cerr << "Unable to wait until data has been read (" << GetLastError() << ")" << std::endl;
                        return I_ERROR;
                }
        }
        else
        {
                SetEvent(ov.hEvent);
        }
        
        bytes_read = d;
        
        #endif
        
        if (bytes_read > 0)
        {
                std::cout << "Read: ";
                for (gsize i = 0; i < bytes_read; i++)
                        std::cout << std::setfill('0') << std::setw(2) << std::hex << ((unsigned int)buf[i] & 0xff) << ' ';
                std::cout << std::endl;
        }
        
        return I_SUCCESS;
}

int SerialInterface::open_port()
{
        #ifdef __unix__
        
        struct termios t;
        
        #elif defined _WIN32
        
        DCB dcb_serial_params = {0};
        COMMTIMEOUTS timeouts = {0};
        
        #endif
        
        close_port();
        
        #ifdef __unix__
        
        port_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        
        #elif defined _WIN32
        
        h_port = CreateFile(("\\\\.\\" + port).c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                0,
                OPEN_EXISTING,
                //FILE_ATTRIBUTE_NORMAL,
                FILE_FLAG_OVERLAPPED,
                0);
        
        #endif
        
        if (!is_open())
        {
                std::cerr << "Error opening port " + port << std::endl;
                return I_ERROR;
        }
        
        #ifdef __unix__
        
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
        
        #elif defined _WIN32
        
        if (!SetCommMask(h_port, EV_RXCHAR))
        {
                std::cerr << "Error setting mask!" << std::endl;
                close_port();
                return I_ERROR;
        }
        
        dcb_serial_params.DCBlength=sizeof(dcb_serial_params);
        
        if (!GetCommState(h_port, &dcb_serial_params)) {
                std::cerr << "Error getting state!" << std::endl;
                close_port();
                return I_ERROR;
        }
        
        memcpy(&dcb_serial_params_saved, &dcb_serial_params, sizeof(dcb_serial_params));
        
        dcb_serial_params.BaudRate=CBR_19200;
        switch (baud)
        {
                case 300:
                        dcb_serial_params.BaudRate=CBR_300;
                        break;
                case 600:
                        dcb_serial_params.BaudRate=CBR_600;
                        break;
                case 1200:
                        dcb_serial_params.BaudRate=CBR_1200;
                        break;
                case 2400:
                        dcb_serial_params.BaudRate=CBR_2400;
                        break;
                case 4800:
                        dcb_serial_params.BaudRate=CBR_4800;
                        break;
                case 9600:
                        dcb_serial_params.BaudRate=CBR_9600;
                        break;
                case 19200:
                        dcb_serial_params.BaudRate=CBR_19200;
                        break;
                case 38400:
                        dcb_serial_params.BaudRate=CBR_38400;
                        break;
                case 57600:
                        dcb_serial_params.BaudRate=CBR_57600;
                        break;
                case 115200:
                        dcb_serial_params.BaudRate=CBR_115200;
                        break;
        }
        
        dcb_serial_params.ByteSize=8;
        dcb_serial_params.StopBits=ONESTOPBIT;
        dcb_serial_params.Parity=NOPARITY;
        
        if(!SetCommState(h_port, &dcb_serial_params)){
                std::cerr << "Error setting state!" << std::endl;
                close_port();
                return I_ERROR;
        }
        
        if(!GetCommTimeouts(h_port, &timeouts)){
                std::cerr << "Error getting timeouts!" << std::endl;
                close_port();
                return I_ERROR;
        }
        
        timeouts.ReadIntervalTimeout=MAXDWORD;
        timeouts.ReadTotalTimeoutConstant=0;
        timeouts.ReadTotalTimeoutMultiplier=0;
        //timeouts.WriteTotalTimeoutConstant=50;
        //timeouts.WriteTotalTimeoutMultiplier=10;
        
        if(!SetCommTimeouts(h_port, &timeouts)){
                std::cerr << "Error setting timeouts!" << std::endl;
                close_port();
                return I_ERROR;
        }
        
        h_overlapped = CreateEvent(0, true, false, 0);
        
        if (h_overlapped == INVALID_HANDLE_VALUE)
        {
                std::cerr << "Error creating event (1)!" << std::endl;
                close_port();
                return I_ERROR;
        }
        
        h_overlapped_thread = CreateEvent(0, true, false, 0);
        
        if (h_overlapped_thread == INVALID_HANDLE_VALUE)
        {
                std::cerr << "Error creating event (2)!" << std::endl;
                close_port();
                return I_ERROR;
        }
        
        #endif
        
        launch_select_thread();
        
        std::cout << "Port opened." << std::endl;
        
        m_port_opened.emit();
        
        return I_SUCCESS;
}

int SerialInterface::close_port()
{
        if (is_open())
        {
                #ifdef __unix__
                
                tcsetattr(port_fd, TCSANOW, &port_termios_saved);
                tcflush(port_fd, TCOFLUSH);  
                tcflush(port_fd, TCIFLUSH);
                close(port_fd);
                port_fd = -1;
                
                #elif defined _WIN32
                
                SetCommState(h_port, &dcb_serial_params_saved);
                CloseHandle(h_port);
                CloseHandle(h_overlapped);
                CloseHandle(h_overlapped_thread);
                h_port = INVALID_HANDLE_VALUE;
                h_overlapped = INVALID_HANDLE_VALUE;
                h_overlapped_thread = INVALID_HANDLE_VALUE;
                
                #endif
                
                std::cout << "Port closed." << std::endl;
                
                m_port_closed.emit();
        }
        
        return I_SUCCESS;
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
        #ifdef __unix__
        
        return (port_fd != -1);
        
        #elif defined _WIN32
        
        return (h_port != INVALID_HANDLE_VALUE);
        
        #endif
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





