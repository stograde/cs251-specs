# cs251-specs

Homework specs as used in https://github.com/StoDevX/cs251-toolkit and https://github.com/StoDevX/referee.

This document is a work in progress.

## Examples

##### Basic Spec

Let's start off with a basic spec:

```yaml
---
assignment: hw5

compilers:
  - &cpp 'g++ --std=c++11 $@ -o $@.exec'

files:
  - [ f1.cpp, *cpp ]
  - [ story2.cpp, *cpp ]

tests:
  - [ f1.cpp, $@.exec ]
  - [ story2.cpp, $@.exec ]
```

At a minimum, a spec needs to declare its name.

Now, that's not very useful, so specs are also expected to declare a list of files that they expect to find. Each entry under the `files` key should be a list, starting with the filename.

The above file is converted to the following representation, which we will now analyze.

```json
{
  "assignment": "hw5",
  "compilers": [
    "g++ --std=c++11 $@ -o $@.exec"
  ],
  "files": [
    { "options": {},
      "commands": ["g++ --std=c++11 $@ -o $@.exec"],
      "filename": "f1.cpp"},
    { "options": {},
      "commands": ["g++ --std=c++11 $@ -o $@.exec"],
      "filename": "story2.cpp"}
  ],
  "tests": [
    { "options": {},
      "commands": ["$@.exec"],
      "filename": "f1.cpp" },
    { "options": {},
      "commands": ["$@.exec"],
      "filename": "story2.cpp" }
  ]
}
```

As we can see, the `files` and `tests` lists both share the same structure. Each file is divided into three parts: `filename`, `commands`, and `options`. The first item in the original list is taken as the filename, any objects in the list are merged into the `options` argument, and any strings are placed into `commands`.


##### Files w/ Options

```yaml
---
assignment: hw4

compilers:
  - &cpp 'g++ --std=c++11 $@ -o $@.exec'

files:
  - [ iotest.cpp, *cpp ]
  - [ countdown.cpp, *cpp, timeout: 0.025, truncate_output: 1000 ]
  - [ countup.cpp, *cpp, timeout: 0.025, truncate_output: 1000 ]
  - [ story.cpp, *cpp ]

tests:
  - [ iotest.cpp, echo Hawken \n 10 \n 42 | $@.exec ]
  - [ countdown.cpp, echo 0 | $@.exec ]
  - [ countup.cpp, echo 10 | $@.exec ]
  - [ story.cpp, $@.exec ]
```

```json
{
  "files": [
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "iotest.cpp"
    },
    {
      "options": {
        "timeout": 0.025,
        "truncate_output": 1000
      },
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "countdown.cpp"
    },
    {
      "options": {
        "timeout": 0.025,
        "truncate_output": 1000
      },
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "countup.cpp"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "story.cpp"
    }
  ],
  "tests": [
    {
      "options": {},
      "commands": [
        "echo Hawken \\n 10 \\n 42 | $@.exec"
      ],
      "filename": "iotest.cpp"
    },
    {
      "options": {},
      "commands": [
        "echo 0 | $@.exec"
      ],
      "filename": "countdown.cpp"
    },
    {
      "options": {},
      "commands": [
        "echo 10 | $@.exec"
      ],
      "filename": "countup.cpp"
    },
    {
      "options": {},
      "commands": [
        "$@.exec"
      ],
      "filename": "story.cpp"
    }
  ],
  "compilers": [
    "g++ --std=c++11 $@ -o $@.exec"
  ],
  "assignment": "hw4"
}
```


##### Multiple Steps and Input Files

```yaml
---
assignment: hw7

compilers:
  - &cpp 'g++ --std=c++11 $@ -o $@.exec'

files:
  - [ avg.cpp, *cpp ]
  - [ stddev.cpp, *cpp ]
  - [ a0.cpp, *cpp ]
  - [ a1.cpp, *cpp ]
  - [ a4.cpp, *cpp ]
  - [ a6.cpp, *cpp ]

tests:
  - [ avg.cpp, cat firefox.txt | $@.exec ]
  - - stddev.cpp
    - cat firefox.txt | $@.exec
    - cat avg-first-firefox.txt | $@.exec
  - [ a0.cpp, $@.exec ]
  - [ a1.cpp, $@.exec ]
  - [ a4.cpp, $@.exec ]
  - [ a6.cpp, $@.exec ]

inputs:
  - firefox.txt
  - avg-first-firefox.txt
```

```json
{
  "files": [
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "avg.cpp"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "stddev.cpp"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "a0.cpp"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "a1.cpp"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "a4.cpp"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "a6.cpp"
    }
  ],
  "tests": [
    {
      "options": {},
      "commands": [
        "cat firefox.txt | $@.exec"
      ],
      "filename": "avg.cpp"
    },
    {
      "options": {},
      "commands": [
        "cat firefox.txt | $@.exec",
        "cat avg-first-firefox.txt | $@.exec"
      ],
      "filename": "stddev.cpp"
    },
    {
      "options": {},
      "commands": [
        "$@.exec"
      ],
      "filename": "a0.cpp"
    },
    {
      "options": {},
      "commands": [
        "$@.exec"
      ],
      "filename": "a1.cpp"
    },
    {
      "options": {},
      "commands": [
        "$@.exec"
      ],
      "filename": "a4.cpp"
    },
    {
      "options": {},
      "commands": [
        "$@.exec"
      ],
      "filename": "a6.cpp"
    }
  ],
  "compilers": [
    "g++ --std=c++11 $@ -o $@.exec"
  ],
  "inputs": [
    "firefox.txt",
    "avg-first-firefox.txt"
  ],
  "assignment": "hw7"
}
```


##### `make` example

```yaml
---
assignment: lab11
folder: projtech

compilers:
  - &cpp 'g++ --std=c++11 $@ -o $@.exec'

files:
  - - proto-chat.cpp
    - rm -f *.o proto-chat
    - make proto-chat
  - - proto-multiplayer.cpp
    - rm -f *.o proto-multiplayer
    - make proto-multiplayer
```

```json
{
  "files": [
    {
      "options": {},
      "commands": [
        "rm -f *.o proto-chat",
        "make proto-chat"
      ],
      "filename": "proto-chat.cpp"
    },
    {
      "options": {},
      "commands": [
        "rm -f *.o proto-multiplayer",
        "make proto-multiplayer"
      ],
      "filename": "proto-multiplayer.cpp"
    }
  ],
  "folder": "projtech",
  "compilers": [
    "g++ --std=c++11 $@ -o $@.exec"
  ],
  "assignment": "lab11"
}
```


##### Really Long `make` Example

```yaml
---
assignment: lab8
folder: graphics

compilers:
  - &cpp 'g++ --std=c++11 $@ -o $@.exec'

files:
  - [ ColorPoint2.h, null ]
  - [ ColorPoint2.cpp, *cpp, optional: true ]

  - [ Shape.h, null ]
  - [ Shape.cpp, *cpp ]
  - - ShapeDriver.cpp
    - rm -f ShapeDriver *.o
    - make ShapeDriver

  - [ Rectangle.h, null ]
  - [ Rectangle.cpp, *cpp ]
  - - RectangleDriver.cpp
    - rm -f RectangleDriver *.o
    - make RectangleDriver

  - [ Circle.h, null ]
  - [ Circle.cpp, *cpp ]
  - - CircleDriver.cpp
    - rm -f CircleDriver *.o
    - make CircleDriver

  - [ shapes.txt, null ]
  - - ShapeDriver1.cpp
    - rm -f ShapeDriver1 *.o
    - make ShapeDriver1
  - - ShapeDriver2.cpp
    - rm -f ShapeDriver2 *.o
    - make ShapeDriver2

  - - proto-draw.cpp
    - rm -f proto-draw *.o
    - make proto-draw


tests:
  - [ ShapeDriver.cpp, ./ShapeDriver ]
  - [ RectangleDriver.cpp, ./RectangleDriver ]
  - [ CircleDriver.cpp, ./CircleDriver ]
  - [ ShapeDriver1.cpp, ./ShapeDriver1 ]
  - [ ShapeDriver2.cpp, ./ShapeDriver2 ]
```

```json
{
  "files": [
    {
      "options": {},
      "commands": [],
      "filename": "ColorPoint2.h"
    },
    {
      "options": {
        "optional": true
      },
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "ColorPoint2.cpp"
    },
    {
      "options": {},
      "commands": [],
      "filename": "Shape.h"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "Shape.cpp"
    },
    {
      "options": {},
      "commands": [
        "rm -f ShapeDriver *.o",
        "make ShapeDriver"
      ],
      "filename": "ShapeDriver.cpp"
    },
    {
      "options": {},
      "commands": [],
      "filename": "Rectangle.h"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "Rectangle.cpp"
    },
    {
      "options": {},
      "commands": [
        "rm -f RectangleDriver *.o",
        "make RectangleDriver"
      ],
      "filename": "RectangleDriver.cpp"
    },
    {
      "options": {},
      "commands": [],
      "filename": "Circle.h"
    },
    {
      "options": {},
      "commands": [
        "g++ --std=c++11 $@ -o $@.exec"
      ],
      "filename": "Circle.cpp"
    },
    {
      "options": {},
      "commands": [
        "rm -f CircleDriver *.o",
        "make CircleDriver"
      ],
      "filename": "CircleDriver.cpp"
    },
    {
      "options": {},
      "commands": [],
      "filename": "shapes.txt"
    },
    {
      "options": {},
      "commands": [
        "rm -f ShapeDriver1 *.o",
        "make ShapeDriver1"
      ],
      "filename": "ShapeDriver1.cpp"
    },
    {
      "options": {},
      "commands": [
        "rm -f ShapeDriver2 *.o",
        "make ShapeDriver2"
      ],
      "filename": "ShapeDriver2.cpp"
    },
    {
      "options": {},
      "commands": [
        "rm -f proto-draw *.o",
        "make proto-draw"
      ],
      "filename": "proto-draw.cpp"
    }
  ],
  "folder": "graphics",
  "compilers": [
    "g++ --std=c++11 $@ -o $@.exec"
  ],
  "tests": [
    {
      "options": {},
      "commands": [
        "./ShapeDriver"
      ],
      "filename": "ShapeDriver.cpp"
    },
    {
      "options": {},
      "commands": [
        "./RectangleDriver"
      ],
      "filename": "RectangleDriver.cpp"
    },
    {
      "options": {},
      "commands": [
        "./CircleDriver"
      ],
      "filename": "CircleDriver.cpp"
    },
    {
      "options": {},
      "commands": [
        "./ShapeDriver1"
      ],
      "filename": "ShapeDriver1.cpp"
    },
    {
      "options": {},
      "commands": [
        "./ShapeDriver2"
      ],
      "filename": "ShapeDriver2.cpp"
    }
  ],
  "assignment": "lab8"
}
```
