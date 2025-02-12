function busyLoop(iterations) {
  let sum = 0;
  for (let i = 0; i < iterations; i++) {
    sum += Math.sqrt(i);
  }
  return sum;
}

console.log("Starting busy work...");
console.log("Result:", busyLoop(100_000_000));
console.log("Busy work done!");
