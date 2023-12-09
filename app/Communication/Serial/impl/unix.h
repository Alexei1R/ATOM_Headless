//
// Created by toor on 11/15/23.
//

#ifndef ATOM_UNIX_H
#define ATOM_UNIX_H



#if !defined(_WIN32)

#ifndef SERIAL_IMPL_UNIX_H
#define SERIAL_IMPL_UNIX_H

#include "Communication/Serial/Serial.h"
#include "v8stdint.h"


#include <pthread.h>

namespace serial {

    using std::size_t;
    using std::string;
    using std::invalid_argument;

    using serial::SerialException;
    using serial::IOException;

    class MillisecondTimer {
    public:
        MillisecondTimer(const uint32_t millis);
        int64_t remaining();

    private:
        static timespec timespec_now();
        timespec expiry;
    };

    class serial::Serial::SerialImpl {
    public:
        SerialImpl (const string &port,
                    unsigned long baudrate,
                    bytesize_t bytesize,
                    parity_t parity,
                    stopbits_t stopbits,
                    flowcontrol_t flowcontrol);

        virtual ~SerialImpl ();

        void
        open ();

        void
        close ();

        bool
        isOpen () const;

        size_t
        available ();

        bool
        waitReadable (uint32_t timeout);

        void
        waitByteTimes (size_t count);

        size_t
        read (uint8_t *buf, size_t size = 1);

        size_t
        write (const uint8_t *data, size_t length);

        void
        flush ();

        void
        flushInput ();

        void
        flushOutput ();

        void
        sendBreak (int duration);

        void
        setBreak (bool level);

        void
        setRTS (bool level);

        void
        setDTR (bool level);

        bool
        waitForChange ();

        bool
        getCTS ();

        bool
        getDSR ();

        bool
        getRI ();

        bool
        getCD ();

        void
        setPort (const string &port);

        string
        getPort () const;

        void
        setTimeout (Timeout &timeout);

        Timeout
        getTimeout () const;

        void
        setBaudrate (unsigned long baudrate);

        unsigned long
        getBaudrate () const;

        void
        setBytesize (bytesize_t bytesize);

        bytesize_t
        getBytesize () const;

        void
        setParity (parity_t parity);

        parity_t
        getParity () const;

        void
        setStopbits (stopbits_t stopbits);

        stopbits_t
        getStopbits () const;

        void
        setFlowcontrol (flowcontrol_t flowcontrol);

        flowcontrol_t
        getFlowcontrol () const;

        void
        readLock ();

        void
        readUnlock ();

        void
        writeLock ();

        void
        writeUnlock ();

    protected:
        void reconfigurePort ();

    private:
        string port_;               // Path to the file descriptor
        int fd_;                    // The current file descriptor

        bool is_open_;
        bool xonxoff_;
        bool rtscts_;

        Timeout timeout_;           // Timeout for read operations
        unsigned long baudrate_;    // Baudrate
        uint32_t byte_time_ns_;     // Nanoseconds to transmit/receive a single byte

        parity_t parity_;           // Parity
        bytesize_t bytesize_;       // Size of the bytes
        stopbits_t stopbits_;       // Stop Bits
        flowcontrol_t flowcontrol_; // Flow Control

        // Mutex used to lock the read functions
        pthread_mutex_t read_mutex;
        // Mutex used to lock the write functions
        pthread_mutex_t write_mutex;
    };

}

#endif // SERIAL_IMPL_UNIX_H

#endif // !defined(_WIN32)


#endif //ATOM_UNIX_H
