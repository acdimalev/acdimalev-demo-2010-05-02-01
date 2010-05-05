![screenshot](http://github.com/acdimalev/acdimalev-demo-2010-05-02-01/raw/master/screenshot.png)

A C+SDL+Cairo demo.  Test combined joystick / keyboard input.  Based on *acdimalev-demo-2010-04-21-01*.

http://github.com/acdimalev/acdimalev-demo-2010-05-02-01

You must have Cairo and SDL installed to build this demo.

* http://cairographics.org
* http://www.libsdl.org

This demo was developed on a Debian Lenny system with the following dev packages installed:

* libcairo2-dev=1.6.4-7
* libsdl1.2-dev=1.2.13-2

To build and execute the demo, run "make" and then "./foo".

    $ make
    $ ./foo

The demo will pop up in a window, and may be closed with the *q* key.  You can then use the following controls to manipulate the triangle:

* Keyboard
** LEFT &mdash; steer left
** RIGHT &mdash; steer right
** UP &mdash; gas
* Xbox 360 Controller
** Left Thumbstick &mdash; steer
** Right Trigger &mdash; gas
