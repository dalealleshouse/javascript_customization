console.log("Scheduling 2 timeouts...");

setTimeout(1000, () => console.log("Fired after 1s"));
setTimeout(2000, () => console.log("Fired after 2s"));
// setTimeout(3000, () => {
//   console.log("Fired after 3s");
//   console.log("Scheduling yet another timeout...");
//   setTimeout(2000, () => console.log("Fired after another 2s"));
// });

console.log("All timers scheduled!");
