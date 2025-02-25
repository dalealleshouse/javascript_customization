const startTime = Date.now();

function actualElaspedTime(expected) {
  const endTime = Date.now();
  const elapsed = endTime - startTime;
  console.log(`Expected delay: ${expected} ms`);
  console.log(`Actual delay: ${elapsed} ms`);
}

setTimeout(() => actualElaspedTime(1000), 1000);
setTimeout(() => actualElaspedTime(2000), 2000);

// sumOfSquares(100_000_000);

function sumOfSquares(iterations) {
  let sum = 0;
  for (let i = 0; i < iterations; i++) {
    sum += Math.sqrt(i);
  }
  console.log(`The sum is ${sum}`);
}
