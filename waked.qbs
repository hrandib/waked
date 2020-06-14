import qbs

Project {

    references: [
        "pc_wake_lib/wakelib.qbs"
    ]

CppApplication {
    name: "waked"


    qbs.architecture: "x86_64"
    cpp.optimization: "small"
    cpp.debugInformation: true

    cpp.includePaths: [
        "include"
    ]
    cpp.cFlags: [
        "-std=c17"
    ]
    cpp.cxxFlags: [
        "-std=c++20"
    ]
    cpp.commonCompilerFlags: [
        "-Wall", "-Wextra", "-Wno-unused-parameter", "-Wno-unused-variable"
    ]

    Group { name: "source"
        prefix: "src/"
        files: [
            "config.cpp",
            "daemon.cpp",
            "main.cpp",
        ]
    }

    Group { name: "include"
        prefix: "include/"
        files: [
            "*.h",
        ]
    }

    Group { name: "resource"
        prefix: "res/"
        files: [
            "*",
        ]
        qbs.install: false
    }

} //CppApplication

} //Project
