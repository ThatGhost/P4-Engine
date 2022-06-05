https://github.com/ThatGhost/P4-Engine

The biggest choices I made were all about simplicity.
I became a big fan of callbacks during development so a lot od the structure i used are all about that.
The EventManager class is also the most called class in my engine.

I don't like engines that require you to use a lot of premade solutions. I would rather make them myself.
So i focused on eas of use instead of feature complete.

Have fun with the ONLINE mode ;)
(it works only in x64 debug mode due to the PlayFab package not working in x86)

The x64 Release doesn't work since the lib_json.lib file from PlayFab has the wrong compiler.
If it did everything would work.