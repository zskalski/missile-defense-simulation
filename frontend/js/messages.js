

function sendMessage(message) {
    if(websocket.readyState == WebSocket.OPEN) {
        websocket.send(JSON.stringify(message));
    } else {
        console.error("Error: Cannot send message, webSocket is closed.")
    }
}

function processMessage(message) {
    // process message by type

    // TYPE: READY
    // when ready is recieved, begin to send periodic simulation update requests

    // TYPE: PLACEMENT
    
    // TYPE: SIMULATION UPDATE
}

function beginSimulationUpdateRequests() {
    
}