// JS code (script to eval):
console.log("Starting promises...");

function wait(ms) {
  return new Promise((resolve) => {
    setTimeout(ms, () => {
      resolve("Waited " + ms + " ms");
    });
  });
}

async function test() {
  console.log(await wait(1000));
  console.log(await wait(2000));
  console.log("All done!");
}

test();
console.log("test() started...");
