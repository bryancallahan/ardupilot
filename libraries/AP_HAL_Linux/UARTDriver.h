#pragma once

#include <AP_HAL/utility/OwnPtr.h>

#include "AP_HAL_Linux.h"
#include "SerialDevice.h"

namespace Linux {

class UARTDriver : public AP_HAL::UARTDriver {
public:
    UARTDriver(bool default_console);

    static UARTDriver *from(AP_HAL::UARTDriver *uart) {
        return static_cast<UARTDriver*>(uart);
    }

    /* Linux implementations of UARTDriver virtual methods */
    void begin(uint32_t b);
    void begin(uint32_t b, uint16_t rxS, uint16_t txS);
    void end();
    void flush();
    bool is_initialized();
    void set_blocking_writes(bool blocking);
    bool tx_pending();

    /* Linux implementations of Stream virtual methods */
    int16_t available();
    int16_t txspace();
    int16_t read();

    /* Linux implementations of Print virtual methods */
    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);

    void set_device_path(const char *path);

    bool _write_pending_bytes(void);
    virtual void _timer_tick(void);

    virtual enum flow_control get_flow_control(void) override
    {
        return _device->get_flow_control();
    }

    virtual void set_flow_control(enum flow_control flow_control_setting) override
   {
       _device->set_flow_control(flow_control_setting);
   }

private:
    AP_HAL::OwnPtr<SerialDevice> _device;
    bool _nonblocking_writes;
    bool _console;
    volatile bool _in_timer;
    uint16_t _base_port;
    char *_ip;
    char *_flag;
    bool _connected; // true if a client has connected
    bool _packetise; // true if writes should try to be on mavlink boundaries

    void _allocate_buffers(uint16_t rxS, uint16_t txS);
    void _deallocate_buffers();

    AP_HAL::OwnPtr<SerialDevice> _parseDevicePath(const char *arg);
    uint64_t _last_write_time;

protected:
    const char *device_path;
    volatile bool _initialised;
    // we use in-task ring buffers to reduce the system call cost
    // of ::read() and ::write() in the main loop
    uint8_t *_readbuf;
    uint16_t _readbuf_size;

    // _head is where the next available data is. _tail is where new
    // data is put
    volatile uint16_t _readbuf_head;
    volatile uint16_t _readbuf_tail;

    uint8_t *_writebuf;
    uint16_t _writebuf_size;
    volatile uint16_t _writebuf_head;
    volatile uint16_t _writebuf_tail;

    virtual int _write_fd(const uint8_t *buf, uint16_t n);
    virtual int _read_fd(uint8_t *buf, uint16_t n);

};

}
