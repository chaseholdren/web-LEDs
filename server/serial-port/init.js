const SerialPort = require('serialport');

const init = () => {
    const port = new SerialPort(process.env.ARDUINO_SERIAL_PORT, {
        baudRate: process.env.ARDUINO_BAUDRATE,
    }, (error) => {
        if (error) {
            console.log('Error opening port: ', error.message)
        } else {
            console.log('port opened')
        };
    });

    port.on('error', (error) => {
        if (error) {
            console.log('Error: ', error.message)
        }
    });

    return port;
};

exports.default = init;
