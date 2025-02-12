console.log("Starting promises...");

async function wait(ms) {
  await new Promise((resolve) => setTimeout(resolve, ms));
  return `Waited ${ms} ms`;
}

async function test() {
  console.log(await wait(1000));
  console.log(await wait(2000));
  console.log("All done!");
}

(async () => {
  await test();
})();

console.log("test() started...");
