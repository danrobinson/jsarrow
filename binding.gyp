{
  "targets": [
    {
      "target_name": "jsarrow",
      "sources": [ "cpp/jsarrow.cc", "cpp/helpers.cc", "cpp/common.cc", "cpp/adapters/builtin.cc", 
                   "cpp/formatting.cc", "cpp/status.cc", "cpp/get.cc" ],
      "include_dirs": [
        "$(ARROW_HOME)/include",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [ "$(ARROW_HOME)/lib/libarrow.dylib" ],
      "conditions": [
        [ "OS=='mac'", {
          "xcode_settings": {
              "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++"],
              "OTHER_LDFLAGS": ["-stdlib=libc++"],
              "MACOSX_DEPLOYMENT_TARGET": "10.7"
            },
        }],
      ],
    }
  ]
}