// prime.js

// Pure Ecmascript version of low level helper
function primeCheckEcmascript(val, limit) {
    for (var i = 2; i <= limit; i++) {
        if ((val % i) == 0) { return false; }
    }
    return true;
}

// Select available helper at load time
var primeCheckHelper = (this.primeCheckNative || primeCheckEcmascript);

// Check 'val' for primality
function primeCheck(val) {
    if (val == 1 || val == 2) { return true; }
    var limit = Math.ceil(Math.sqrt(val));
    while (limit * limit < val) { limit += 1; }
    return primeCheckHelper(val, limit);
}

function primeTest() {
    var res = [];

    print('Have native helper: ' + (primeCheckHelper !== primeCheckEcmascript) + '\n');
    for (var i = 2; i <= 1000; i++) {
        if (primeCheck(i)) { res.push(i); }
    } 
    print(res.join(' '));
}

print = this.send || conn.write

print('HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n');

primeTest();
