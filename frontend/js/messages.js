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
let pendingPieces = new Map();

function populateMessageHandler() {
    
    return new Map([
        ["ready", handleReadyMessage],
        ["update.response", applyUpdate],
        ["start.response", simulationStarted],
        ["pause.response", simulationPaused],
        ["reset.response", simulationReset],
        ["doAuto.response", recieveSimulationAutoUpdate],
        ["radarVis.response", recieveSimulationRadarVisUpdate],
        ["simSpeed.response", recieveSimulationSpeedUpdate],
        ["placement.response", recieveSimulationPlacementResponse]
    ])
}

function sendMessage(message) {
    if(websocket.readyState == WebSocket.OPEN) {
        websocket.send(JSON.stringify(message));
        //console.log("Sending message:", message);
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



// 50 ms Updates: ---------------------

let updateRequestID;

function handleReadyMessage(message) {
    if (updateRequestID !== undefined) {
        clearInterval(updateRequestID);
    }
    updateRequestID = setInterval(() => {
        sendMessage({type: "update.request"});
    }, 50);
    addToEventLog("BACKEND SERVER CONNECTED.");
}

function applyUpdate(message) {
    
    // update time
    // Format numbers to always have two digits
    const displayHours = String(message.payload.timer.hours).padStart(2, '0');
    const displayMinutes = String(message.payload.timer.minutes).padStart(2, '0');
    const displaySeconds = String(message.payload.timer.seconds).padStart(2, '0');

    updateTimeMetric(`${displayHours}:${displayMinutes}:${displaySeconds}`);

    // update totals for each piece type
    placedCommandCenter = message.payload.totalPieces.commandCenters;
    placedRadar = message.payload.totalPieces.radars;
    placedTarget = message.payload.totalPieces.targets;
    placedInterceptor = message.payload.totalPieces.interceptors;
    placedEnemyMissile = message.payload.totalPieces.enemyMissiles;
    placedEnemyMissileBarrage = message.payload.totalPieces.enemyMissileBarrages;
    placedTree = message.payload.totalPieces.trees;
    placedLake = message.payload.totalPieces.lakes;
    updateTargetMetric();
    updateTrackMetric(message.payload.tracks.total);
    updateInterceptorMetric();
}


// Control Messages ---------------------

function sendSimulationPlayRequest() {
    sendMessage({
        type: "start.request"
    });
}

function sendSimulationPauseRequest() {
    sendMessage({
        type: "pause.request"
    });
}

function sendSimulationResetRequest() {
    sendMessage({
        type: "reset.request"
    });
}

function simulationStarted() {
    console.log("Simulation started.");
}

function simulationPaused() {
    console.log("Simulation paused.");
}

function simulationReset() {
    resetMetrics();
    clearGrid();
    redrawCanvas(options);
    clearEventLog();
    console.log("Simulation reset.");
}



// User Options ---------------------

// Auto Mode
function sendSimulationAutoRequest(doAuto) {
    sendMessage({
        type: "doAuto.request",
        payload: {
            doAuto: doAuto
        }
    });
}

function recieveSimulationAutoUpdate(message) {
    updateAutoCheckBox(message.payload.doAuto);
    console.log("Auto mode changed to: ", message.payload.doAuto);
}

// Radar Visibility
function sendSimulationRadarVisRequest(radarVis) {
    sendMessage({
        type: "radarVis.request",
        payload: {
            radarVis: radarVis
        }
    });
}

function recieveSimulationRadarVisUpdate(message) {
    updateRadarVis(message.payload.radarVis);
    console.log("Radar visibility changed to: ", message.payload.radarVis);
}

// Simulation Speed
function sendSimulationSpeedRequest(simSpeed) {
    sendMessage({
        type: "simSpeed.request",
        payload: {
            simSpeed: simSpeed
        }
    });
}

function recieveSimulationSpeedUpdate(message) {
    updateSimSpeed(message.payload.simSpeed);
    console.log("Simulation speed changed to: ", message.payload.simSpeed);
}



// Placement Logic
function sendSimulationPlacementRequest(piece) {
    pendingPieces.set(piece.id, piece);

    sendMessage({
        type: "placement.request",
        payload: {
            id: piece.id,
            type: piece.dataset.type,
            position: {
                row: Number(piece.dataset.row),
                column: Number(piece.dataset.col)
            }
        }
    });
}

function recieveSimulationPlacementResponse(message) {
    const status = message.payload.status;
    const pieceId = message.payload.id;
    const pendingPiece = pendingPieces.get(pieceId);

    if (!status) {
        pendingPieces.delete(pieceId);
        console.log("Placement denied by backend.");
        return;
    }

    if (pendingPiece) {
        placePiece(pendingPiece);
        pendingPieces.delete(pieceId);
    }

    console.log("Placement accepted by backend.");
}