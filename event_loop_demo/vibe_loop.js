function actualElapsedTime(scheduleTime, expected) {
  return function () {
    const endTime = Date.now();
    const elapsed = endTime - scheduleTime;
    console.log(`Expected delay: ${expected} ms`);
    console.log(`Actual delay: ${elapsed} ms`);
  };
}

setTimeout(actualElapsedTime(Date.now(), 2000), 2000);
setTimeout(actualElapsedTime(Date.now(), 1000), 1000);

console.log("All timers scheduled!");
