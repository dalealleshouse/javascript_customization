console.log("Scheduling 2 timeouts...");

setTimeout(1000, () => console.log("Fired after 1s"));
setTimeout(2000, () => console.log("Fired after 2s"));

console.log("All timers scheduled!");
