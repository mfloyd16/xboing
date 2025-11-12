/*
 * src2/main.c
 * Entry point for the simplified Rayboing demo (uses raylib).
 * This file sets up the window/audio, handles basic command-line
 * parameter validation, performs initialization of subsystems
 * (blocks, paddle, ball, audio), and runs the top-level game loop
 * which dispatches into different game modes.
 */

#include <stdio.h>   // printf, fprintf, FILE I/O
#include <raylib.h>  // raylib framework for window, input, audio, drawing
#include <stdbool.h> // bool, true, false

/*
 * Local headers providing the game's subsystems and mode management.
 * Each header exposes initialization, update, and cleanup functions
 * used by this main program.
 */
#include "gamemodes.h"   // Game mode enum and mode/set/get functions
#include "blockloader.h"// Block texture loading/freeing
#include "ball.h"       // Ball init, update and free functions
#include "paddle.h"     // Paddle init, movement and free functions
#include "audio.h"      // Audio initialization and cleanup helpers
#include "intro.h"      // Intro screen helper (ShowIntroScreen)
#include "scene.h"

/* Screen dimensions used to create the window. Kept as constants so they
   can be referenced throughout and changed in one place. */
const int SCREEN_WIDTH = 575;
const int SCREEN_HEIGHT = 720;

/* Forward declarations for functions implemented at the bottom of this
   translation unit. Keeping these prototypes here keeps main() near the top
   of the file for easier reading. */
bool ValidateParamFilename(int argumentCount, char *arguments[]);
void ReleaseResources(void);

/* Scene manager integration ------------------------------------------------
 * We create small Scene wrappers that delegate to the existing Run*Mode
 * functions so the program logic remains unchanged while the main loop
 * dispatches through the SceneManager.
 */

static const char *DEFAULT_LEVEL = "resource/levels/level01.data";
static const char *g_init_level = NULL; /* filename provided or default */

/* Forward declarations for scene wrapper callbacks. Each scene's Update
   callback calls the existing Run*Mode() function so behavior stays
   identical to the previous mode-based dispatch. */
static void scene_init_Init(void);
static void scene_init_Update(void);
static void scene_init_Draw(void);
static void scene_init_Unload(void);

static void scene_play_Init(void);
static void scene_play_Update(void);
static void scene_play_Draw(void);
static void scene_play_Unload(void);

static void scene_end_Init(void);
static void scene_end_Update(void);
static void scene_end_Draw(void);
static void scene_end_Unload(void);

static Scene initScene = { NULL, scene_init_Update, NULL, NULL, "InitGame" };
static Scene playScene = { NULL, scene_play_Update, NULL, NULL, "Play" };
static Scene endScene  = { NULL, scene_end_Update,  NULL, NULL, "End" };

/* Scene callback implementations: simply call the corresponding Run*Mode
   functions that already implement the mode behavior. We intentionally
   keep Init/Draw/Unload empty because the existing Run*Mode functions
   handle rendering and state transitions for each mode. */
static void scene_init_Init(void) { /* unused */ }
static void scene_init_Update(void) {
    /* Run per-frame logic for init mode using configured level */
    if (g_init_level) RunInitGameMode(g_init_level);
    else RunInitGameMode(DEFAULT_LEVEL);
}
static void scene_init_Draw(void) { /* drawing is handled by RunInitGameMode */ }
static void scene_init_Unload(void) { /* nothing to free here */ }

static void scene_play_Init(void)  { /* unused */ }
static void scene_play_Update(void) { RunPlayMode(); }
static void scene_play_Draw(void)  { /* drawing handled in RunPlayMode */ }
static void scene_play_Unload(void) { /* nothing to free here */ }

static void scene_end_Init(void) { /* unused */ }
static void scene_end_Update(void) { RunEndMode(); }
static void scene_end_Draw(void) { /* drawing handled in RunEndMode */ }
static void scene_end_Unload(void) { /* nothing to free here */ }



int main(int argumentCount, char *arguments[]) {

    /* Disable raylib tracing logs (suppress debug/info output). This keeps
       console output clean when running the demo. */
    SetTraceLogLevel(LOG_NONE);

    /* Return code visible to the OS; default to failure (1) and set to 0 on
       successful initialization. */
    int rtnCode = 1;

    /* Track whether we successfully created a window. This ensures we only
       call CloseWindow() if InitWindow succeeded. */
    bool windowInitialized = false;

    /* Initialize the graphics window and audio device. Comments:
       - InitWindow must be called before loading any textures that rely on
         the graphics context.
       - InitAudioDevice prepares the audio subsystem used by the game.
       - SetTargetFPS requests a fixed update/render rate from raylib. */
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rayboing Demo");
    InitAudioDevice();
    SetTargetFPS(60);
    windowInitialized = true;

    /* If the user ran the program without any command-line arguments, we
       present an intro or start menu. The intro may block until the player
       presses a key (e.g., Enter/Space) or may exit early if the window is
       closed. If the window is closed while showing the intro, we clean up
       and exit immediately. */
    if (argumentCount == 1) {
        ShowIntroScreen();
        if (WindowShouldClose()) {
            if (windowInitialized) CloseWindow();
            ReleaseResources();
            return rtnCode; /* Early exit with failure code (no game started) */
        }
    }

    /* Initialization sequence for game subsystems. Each step is checked and
       if any initialization fails, we print an error to stderr and avoid
       continuing into the main game. The checks are done in an if/else-if
       chain so we can print a specific reason for the halt. */
    if (!ValidateParamFilename(argumentCount, arguments)) {
        /* Command-line validation failed (bad usage or filename). */
        fprintf(stderr, "Program halt on map validation");
    } else if (!loadBlockTextures()) {
        /* Failed to initialize block textures (graphics resources for blocks). */
        fprintf(stderr, "Program halt on iniitalize block texture");
    } else if (!InitialisePaddle()) {
        /* Paddle subsystem failed to initialize. */
        fprintf(stderr, "Program halt on initialize paddle");
    } else if (!InitializeBall()) {
        /* Ball subsystem failed to initialize. */
        fprintf(stderr, "Program halt on initialize ball");
    } else if (!initAudioFiles()) {
        /* Audio resources failed to initialize (sound files, buffers, etc.). */
        fprintf(stderr, "Program halt on initialize sounds");
    } else {

        /* All required subsystems initialized successfully. Prepare the
           play area, set the starting game mode and mark return code as
           success. */
        initializePlayArea();
        SetGameMode(MODE_INITGAME);
        rtnCode = 0;
    }

   /* Configure which level the init scene should use. We set this global
      pointer before creating the first scene so scene callbacks can use
      it. If the user supplied a filename on the command line, use it,
      otherwise use the default built into the program. */
   g_init_level = (argumentCount == 2) ? arguments[1] : DEFAULT_LEVEL;

   /* Map the current game mode to the scene manager and run until the
      mode becomes MODE_EXIT. We track the last seen mode and switch the
      active Scene when the mode changes. Scenes delegate to the existing
      Run*Mode() functions, preserving original behavior. */
   GAME_MODES currentMode = GetGameMode();
   GAME_MODES lastMode = currentMode;

   /* Set initial scene based on starting mode */
   if (currentMode == MODE_INITGAME) SceneManager_SetScene(&initScene);
   else if (currentMode == MODE_PLAY) SceneManager_SetScene(&playScene);
   else SceneManager_SetScene(&endScene);

   while (GetGameMode() != MODE_EXIT) {

      /* Input handling remains the same: move the paddle by mouse when
         in PLAY mode, with keyboard fallback. */
      if (GetGameMode() == MODE_PLAY) {
         Vector2 mousePos = GetMousePosition();
         SetPaddlePosition(mousePos.x);
         if (IsKeyDown(KEY_LEFT))  MovePaddle(PADDLE_LEFT);
         if (IsKeyDown(KEY_RIGHT)) MovePaddle(PADDLE_RIGHT);
      }

      /* If the game mode changed since last frame, switch the active
         scene so SceneManager_Update/Draw will call the correct
         callbacks. */
      currentMode = GetGameMode();
      if (currentMode != lastMode) {
         switch (currentMode) {
            case MODE_INITGAME:
               SceneManager_SetScene(&initScene);
               break;
            case MODE_PLAY:
               SceneManager_SetScene(&playScene);
               break;
            case MODE_WIN:
            case MODE_LOSE:
            case MODE_CANCEL:
               SceneManager_SetScene(&endScene);
               break;
            default:
               SetGameMode(MODE_CANCEL);
               SceneManager_SetScene(&endScene);
               break;
         }
         lastMode = currentMode;
      }

      /* Dispatch per-frame logic and drawing to whatever scene is
         currently active. The scene callbacks call into the existing
         Run*Mode functions, so rendering and transitions happen there. */
      SceneManager_Update();
      SceneManager_Draw();

      if (WindowShouldClose()) SetGameMode(MODE_EXIT);
   }

    /* Clean up: unload the active scene (so per-scene resources are freed),
       close the window only if it was created successfully, and then
       release all remaining allocated subsystem resources. */
    SceneManager_UnloadCurrent();
    if (windowInitialized) CloseWindow();
    ReleaseResources();

    /* Return to the OS. rtnCode is 0 on successful initialization, or 1 if
       initialization failed earlier. */
    return rtnCode;
}


/* ValidateParamFilename
 * - If no arguments are given (argumentCount == 1), the function returns
 *   true because the program can run using the default level; the intro
 *   screen will be shown earlier.
 * - If exactly one argument is supplied (argumentCount == 2), treat that
 *   argument as a path to a level/map file. Attempt to open it for reading
 *   to verify existence and readability; return true only if the file can
 *   be opened. Print an informative message to stdout/stderr as appropriate.
 * - Any other number of arguments is considered invalid usage; print
 *   a usage message and return false. */
bool ValidateParamFilename(int argumentCount, char *arguments[]) {

    if (argumentCount == 1) return true; /* No filename provided; OK. */

    if (argumentCount == 2) {
        const char *fileName = arguments[1];
        FILE *file = fopen(fileName, "r");
        if (!file) {
            /* fopen failed — either the file doesn't exist or we lack
               permissions. Report the problem and return false. */
            fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", fileName);
            return false;
        } else {
            /* File opened successfully — print a short message and close
               the file immediately; the loader will reopen it later. */
            fprintf(stdout, "Running Rayboing with map '%s'\n", fileName);
            fclose(file);
            return true;
        }
    }

    /* Any other argument count is invalid — print usage string. */
    fprintf(stderr, "Usage: %s <filename>\n", arguments[0]);
    return false;
}


/* ReleaseResources
 * Free resources allocated by subsystems. The order here is generally not
 * critical but we free higher-level game objects (paddle/ball) and then
 * shared systems (textures/audio). These functions are expected to be
 * implemented in their respective modules. */
void ReleaseResources(void) {
    FreePaddle();        /* free paddle-related memory/state */
    FreeBall();          /* free ball-related memory/state */
    freeBlockTextures(); /* free block texture images and GPU resources */
    FreeAudioSystem();   /* release audio buffers and shut down audio */
}