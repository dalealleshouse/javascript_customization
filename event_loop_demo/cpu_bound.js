function actualElapsedTime(scheduleTime, expected) {
  return function () {
    const endTime = Date.now();
    const elapsed = endTime - scheduleTime;
    console.log(`Expected delay: ${expected} ms`);
    console.log(`Actual delay: ${elapsed} ms`);
  };
}

setTimeout(actualElapsedTime(Date.now(), 1000), 1000);
setTimeout(actualElapsedTime(Date.now(), 2000), 2000);

sumOfSquareRoots(100_000_000);

// ~ 2 second runtime
function sumOfSquareRoots(iterations) {
  let sum = 0;
  for (let i = 0; i < iterations; i++) {
    sum += Math.sqrt(i);
  }
  console.log(`The sum is ${sum}`);
}
