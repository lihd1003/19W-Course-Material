function doubleAfter1Second(x) {
  return new Promise((resolve, reject) => {
    setTimeout(() => {
      resolve(x * 2);
      // reject('error');
    }, 1000);
  });
}

async function addAsync(x) {
  const a = await doubleAfter1Second(10);
  const b = await doubleAfter1Second(a);
  const c = await doubleAfter1Second(b);
  return x + a + b + c;
  //throw new Error('this is an error')
}


addAsync(10).then((sum) => {
  console.log(sum);
}).catch((error) => {
  console.log('catch: ', error)
})
