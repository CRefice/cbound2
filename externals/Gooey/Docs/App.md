App::run(Window&) basically just shows a window and sets a destroy handler for it. If you use this function overload, don't set a handler for onDestroy, as it will get overridden.
