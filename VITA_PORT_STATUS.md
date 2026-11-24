# PS Vita port: estado

Hecho (sin compilar aún):
- LiveArea: sce_sys/icon0.png, sce_sys/pic0.png, sce_sys/livearea/contents/{bg0.png,startup.png,template.xml}.
- Bubble: "Plants vs Zombies" (TITLE en param.sfo), TITLE_ID PVZGOTY01.
- Capa de ventana y entrada Vita: platform/vita/Window.cpp e Input.cpp (vitaGL, sceCtrl, sceTouch).
- Ramas __vita__ en GLPlatform.h, GLInterface.cpp y SexyAppBase.h.
- CMake: rama VITA (fuentes, enlaces, vita_create_self, vita_create_vpk), SDL3 excluido en Vita.
- Workflow vita.yml (solo workflow_dispatch) con contenedor VitaSDK.

Pendiente (bloqueantes):
1. Audio: SexyAppFramework/sound usa SDL3_mixer; falta backend sceAudioOut (o un stub para el primer build).
2. SDL3 en main.cpp, SexyApp y LawnApp: sustituir lo que dependa de SDL por la capa Vita.
3. libopenmpt (música .mo3) para ARM Cortex-A9: probar coste de CPU; alternativa: desactivar música en el primer build.
4. Recursos: main.pak y properties/ van en ux0:app/PVZGOTY01/ (no se empaquetan en el VPK).
5. Rendimiento 60 fps: la lógica original corre a 100 Hz; perfilar en hardware, reducir resolución de capa, LOW_MEMORY.
