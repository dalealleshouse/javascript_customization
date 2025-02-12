heartbeat();

function heartbeat() {
  console.log("Thump thump");
  setTimeout(heartbeat, 2000);
}
