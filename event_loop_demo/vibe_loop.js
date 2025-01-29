console.log("Scheduling 4 timeouts...");
setTimeout(1000, () => console.log("Fired after 1s"));
setTimeout(2000, () => console.log("Fired after 2s"));
setTimeout(5000, () => console.log("Fired after 5s"));
setTimeout(8000, timeOutInTimeout);
console.log("All timers scheduled!");

function timeOutInTimeout() {
  console.log("Fired after 8s");
  console.log("Scheduling yet another timeouts...");
  setTimeout(2000, () => console.log("Fired after another 2s"));
}
