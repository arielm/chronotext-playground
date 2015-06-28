var Module = {
  preRun: function() {
    FS.mkdir("/resources");
    FS.mount(NODEFS, { root: "../.." }, "/resources");

    console.log(FS.stat('resources/test.txt'));
    console.log('preRun executed');
  },
  postRun: function() {
    FS.unmount("/resources");
    console.log('postRun executed');
  },
};

console.log('pre.js executed');
