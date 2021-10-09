# Minigin QBert
This project contains both a 2D game engine "Minigin" and a game "Qbert" using the engine.

The engine and the game were part of a module called "Programming 4".

![QBert gameplay](/Resources/QBert.gif)

This project uses this library: [Simple-SDL2-Audio](https://github.com/jakebesworth/Simple-SDL2-Audio)

# Minigin
The engine uses [GameObject.h](/Minigin/GameObject.h) for every object that exists in the game. The object's behaviour is defined by all of it's [Component.h](/Minigin/Component.h). 

The engine allows to split up games in different [scenes](/Minigin/Scene.h). These scenes grant the possibility to give a [GameContext](/Minigin/GameContext.h) if needed, for example to have a central location to manage enemies.

## Component communication
Components communicate through composition and Observers. 

Some components require others in order to function, for example: [TextComponent](/Minigin/TextComponent.h) requires a [TextureComponent](/Minigin/TextureComponent.h) because it uses a texture to display the text. When this text needs to change, the TextComponent tells it's TextureComponent to update.

Other components send events to their observers, for example: [ScoreComponent](/QBert/ScoreComponent.h) listens to multiple components untill they send an [EnemyDeathEvent](/QBert/EnemyDeathEvent.h) or a [ScoreEvent.h](/QBert/ScoreEvent.h).

## Player input
Players can give input using either ImGui buttons or [Commands](/Minigin/Command.h). These commands are added through the [InputManager.h](/Minigin/InputManager.h), it handles the actual keyboard and controller inputs and executes the corresponding command. Commands usually need a component to send their inputs to. 

In the QBert game, ImGui buttons are used to traverse menu options like: [choose a game mode](/QBert/GameModeMenuComponent.h), [quit the game](/QBert/BackToMenuComponent.h)... Commands however are used for [Movement](/QBert/MoveCommand.h), [Muting](/QBert/MuteCommand.h), [Killing the player in debug mode](/QBert/KillCommand.h) and whatever else keyboard or controller inputs the game requires.

## Sound
Sound is handled by this mentioned-before library: [Simple-SDL2-Audio](https://github.com/jakebesworth/Simple-SDL2-Audio). Playing a sound is as simple as using the [ServiceLocator](/Minigin/ServiceLocator.h) to get the sound system. According to which sound system is currently active, that system will play the sound. 

Currently implemented sound systems are:
*[SimpleSDL2AudioSoundSystem.h](/Minigin/SimpleSDL2AudioSoundSystem.h) will add the sound to the queue and process the sound on a different thread.
*[ConsoleSoundSystem.h](/Minigin/ConsoleSoundSystem.h) is a decorator which will log the requested sound to the console and send the request through to the system it decorates.

# QBert
[QBert](https://en.wikipedia.org/wiki/Q*bert) is a classic arcade game. In it, QBert traverses through levels by walking over blocks, turning them into different colors whilst avoiding different enemies.

This project includes the QBert game in it's original form (single player) and adds a **co-op** mode where you can work together and a **versus** mode where the second player plays as Coily and tries to kill Player1.

## Movement
QBert and his enemies all move the same way: using [MoveCommands](/QBert/MoveCommand.h). This MoveCommand tells the [MovementComponent](/QBert/MovementComponent.h) where to go. This allows Coily to be controlled by both it's AI component or Player 2 in versus mode.

## Enemies
Enemies are spawned from a [factory](/QBert/EnemyFactory.h) and are driven by an [AIComponent](/QBert/AIComponent.h) that tells it where to go, whether it can kill or will be killed by QBert, whether it can revert tiles to their original color... 
