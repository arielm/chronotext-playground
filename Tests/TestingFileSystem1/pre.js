var fs = require('fs');
fs.writeFileSync('foobar.txt', 'yeehaw');

var Module = {
  preRun: function() {
    FS.mkdir("/src");
    FS.mount(NODEFS, { root: "." }, "/src");
    console.log('preRun executed');
  },
  postRun: function() {
    FS.unmount("/src");
    console.log('postRun executed');
  },
};

console.log('pre.js executed');
