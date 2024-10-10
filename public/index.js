/* global fetch */
/** throttle **/
const forwardSpeed = '25';
const reverseSpeed = '-25';
const steeringForward = '0,0';
const turnLeft = '-35';
const turnRight ='35';

const sendMessageThrottle = async (direction) => {
    const response = await fetch('http://localhost:3000/throttle', {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ throttle:direction })
    });
    if (!response.ok) {
        throw new Error('Could not send message', response)
    }

}

/** throttle **/

const moveForward = async () => {
    await sendMessageThrottle(forwardSpeed);
    await sendMessageSteering(steeringForward);
}
const moveBackward = async () => {
    await sendMessageThrottle(reverseSpeed);
    await sendMessageSteering(steeringForward);

}

const stop = async () => {
    await sendMessageThrottle(steeringForward);
}
/** steering **/

const sendMessageSteering = async (direction) => {
    const response = await fetch('http://localhost:3000/steering', {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ steering:direction })
    });
    if (!response.ok) {
        throw new Error('Could not send message', response)
    }
}
/** steering **/
const moveRight = async () => {
    await sendMessageSteering(turnRight);
}
const moveLeft = async () => {
    await sendMessageSteering(turnLeft);
}

/** Drive mode **/

const sendMessageDrive = async (direction) => {
    const response = await fetch('http://localhost:3000/drive', {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ drive:direction })
    });
    if (!response.ok) {
        throw new Error('Could not send message', response)
    }
}

const stopMode = async (stopModeValue) => {
    stopModeValue = '0';
    await sendMessageDrive(stopModeValue);
}

const AimlessMode = async (aimlessValue) => {
    aimlessValue = '1'
    await sendMessageDrive(aimlessValue);
}

const FrozenSection = async (pathOne) => {
    pathOne = '2';
    await sendMessageDrive(pathOne);
}

const Entrance = async (pathTwo) => {
    pathTwo = '3';
    await sendMessageDrive(pathTwo);
}

const LargeArea = async (pathThree) => {
    pathThree = '4';
    await sendMessageDrive(pathThree);
}
