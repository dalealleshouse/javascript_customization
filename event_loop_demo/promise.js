function actualElapsedTime(scheduleTime, expected, resolve) {
  return function () {
    const endTime = Date.now();
    const elapsed = endTime - scheduleTime;
    console.log(`Expected delay: ${expected} ms`);
    console.log(`Actual delay: ${elapsed} ms`);
    resolve();
  };
}

async function wait(ms) {
  await new Promise((resolve) => {
    setTimeout(actualElapsedTime(Date.now(), ms, resolve), ms);
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
