function actualElapsedTime(scheduleTime, expected) {
  return function () {
    const endTime = Date.now();
    const elapsed = endTime - scheduleTime;
    console.log(`Expected delay: ${expected} ms`);
    console.log(`Actual delay: ${elapsed} ms`);
  };
}

async function wait(ms) {
  await new Promise((resolve) => {
    setTimeout(actualElapsedTime(Date.now(), ms), ms);
    resolve();
  });
}

async function test() {
  await wait(2000);
  await wait(1000);
  console.log("All done!");
}

(async () => {
  await test();
})();
