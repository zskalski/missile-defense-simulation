const wsUri = "ws://127.0.0.1:8081";
const websocket = new WebSocket(wsUri);

websocket.addEventListener("open", () => {
    console.log("CONNECTED");
});

websocket.addEventListener("message", (event) => {
    try {
        message = JSON.parse(event.data);
    } catch (error) {
        console.error("Received invalid JSON:", event.data);
        console.error(error);
        return;
    }

    //console.log("RECEIVED:", message);

    try {
        processMessage(message);
    } catch (error) {
        console.error("Error processing message:", error);
    }
});

websocket.addEventListener("close", (event) => {
    console.log("CONNECTION CLOSED");
});

websocket.addEventListener("error", (error) => {
    console.error("WEBSOCKET ERROR:", error);
});
