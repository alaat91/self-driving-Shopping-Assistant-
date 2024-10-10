const express = require('express');
const mqtt = require('mqtt');

// Connection variables
const host = 'localhost'
const port = '1883'
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`
const connectUrl = `mqtt://${host}:${port}`

// Mosquitto client that will publish to mosquitto broker
const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: '',
    password: '',
    reconnectPeriod: 1000,
})

// The http server that serves the website and listens for publish messages.
const app = express();

// If a connection is made to mosquitto, start the http server.
client.on('connect', () => {
    console.log('Connected to Mosquitto server')
    const port = 3000
    app.use(express.json());
    app.use('/', express.static('public'));

    // If a POST request is made from the website, transmit the request to the broker.
    app.post('/throttle', (req, res) => {
        const topic ='/smartcar/control/throttle'
        client.publish(topic, req.body.throttle, { qos: 0, retain: false }, (error) => {
            if (error) {
                console.error(error)
            }
        })
        res.sendStatus(201);
    })
    app.post('/steering', (req, res) => {
        const topic ='/smartcar/control/steering'
        client.publish(topic, req.body.steering, { qos: 0, retain: false }, (error) => {
            if (error) {
                console.error(error)
            }
        })
        res.sendStatus(201);
    })

    app.post('/drive', (req, res) => {
        const topic ='/smartcar/control/drive'
        client.publish(topic, req.body.drive, { qos: 0, retain: false }, (error) => {
            if (error) {
                console.error(error)
            }
        })
        res.sendStatus(201);
    })

 app.listen(port, () => {
        console.log(`Backend server listening on port ${port}`)
    })

})

