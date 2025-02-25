console.log("Scheduling 2 timeouts...");

setTimeout(() => console.log("Fired after 2s"), 2000);
setTimeout(() => console.log("Fired after 1s"), 1000);

console.log("All timers scheduled!");
