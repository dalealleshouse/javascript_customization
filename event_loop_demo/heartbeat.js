heartbeat();

function heartbeat() {
  console.log("Thump thump");
  setTimeout(2000, heartbeat);
}
