/*
    JSON MESSAGE STRUCUTURE
    {
        "type": "object.created"        (type of the message )
        "payload:" {                    (specific information being sent)
            "object": {
                "id": "radar-14",
                "objectType": "radar",
                "position": {
                    "row": 5,
                    "column": 8
                },
                "range": 300,
                "active": true
            }
        }
    }
*/

// functions for each message will be stored inside of a map:
let messageHandler = populateMessageHandler();

function populateMessageHandler() {
    
    return new Map([
        ["ready", handleReadyMessage],
        ["update.response", applyUpdate]
    ])
}

function sendMessage(message) {
    if(websocket.readyState == WebSocket.OPEN) {
        websocket.send(JSON.stringify(message));
        console.log("Sending message:", message);
    } else {
        console.error("Error: Cannot send message, webSocket is closed.")
    }
}

function processMessage(message) {

    // process message by type
    messageType = message.type;
    
    if (!messageHandler.has(messageType)) {
        console.error("Error: processMessage does not recognize message type.");
        sendMessage({
            type: "error",
            payload: {
                message: "processMessage does not recognize message type.",
                receivedType: messageType
            }
        });
        return;
    }
    
    const handler = messageHandler.get(messageType);
    handler(message);

    return;
}



let updateRequestID;

function handleReadyMessage(message) {
    if (updateRequestID !== undefined) {
                clearInterval(updateRequestID);
            }
            
            updateRequestID = setInterval(() => {
                sendMessage({type: "update.request"});
            }, 50);
}

function applyUpdate(message) {
    
    // update time
    // Format numbers to always have two digits
    const displayHours = String(message.payload.hours).padStart(2, '0');
    const displayMinutes = String(message.payload.minutes).padStart(2, '0');
    const displaySeconds = String(message.payload.seconds).padStart(2, '0');

    updateTimeMetric(`${displayHours}:${displayMinutes}:${displaySeconds}`);
}