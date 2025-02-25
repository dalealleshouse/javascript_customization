async function wait(ms) {
  await new Promise((resolve) => setTimeout(resolve, ms));
  console.log(`Waited ${ms} ms`);
}

async function test() {
  await wait(2000);
  await wait(1000);
  console.log("All done!");
}

(async () => {
  await test();
})();
