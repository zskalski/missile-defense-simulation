const wsUri = "ws://127.0.0.1";
const websocket = new WebSocket(wsUri);

let counter = 1;

const message = {
    iteration: 1,
    content: "hello"
};

websocket.addEventListener("open", () => {
    console.log("connected");
    const pingInterval = setInterval(() => {
        console.log("sent");
        websocket.send(JSON.stringify(message));
    }, 1000);   // milliseconds
});

websocket.addEventListener("message", (e) => {
    const message = JSON.parse(e.data);
    console.log(`RECEIVED: ${message.iteration}: ${message.content}`);
    counter++;
});

websocket.addEventListener("close", () => {
    console.log("DISCONNECTED");
    clearInterval(pingInterval);
});