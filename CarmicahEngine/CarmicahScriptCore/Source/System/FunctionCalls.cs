using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Carmicah
{
    public static class FunctionCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetScale(uint entityID, out Vector2 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetScale(uint entityID, ref Vector2 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetRenderingScale(uint entityID, out Vector2 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetWorldScale(uint entityID, out Vector2 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(uint entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint Entity_GetChild(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ApplyForceWithTime(uint entityID, Vector2 dir, float magnitude, float lifeTime);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ApplyForce(uint entityID, Vector2 dir, float magnitude);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_Move(uint entityID, Vector2 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_StopObject(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsKeyPressed(Keys keyCode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsKeyHold(Keys keyCode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetPosition(uint entityID, out Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetPosition(uint entityID, ref Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetDepth(uint entityID, out float depth);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetDepth(uint entityID, ref float depth);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetRot(uint entityID, out float rot);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetRot(uint entityID, ref float rot);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint Entity_FindEntityWithName(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_PlaySFX(string soundName, float volume, bool isLoop , uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_PlayBGM(string soundName, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_SwitchBGM(string soundName, float fadeTimer, float fadeDuration, bool isLoop, bool fadeInNext);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_SetVolume(string soundName, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_StopBGM(string soundName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_StopSFX(string soundName, uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_StopSFXWithFade(string soundName , float fadeTimer, float fadeDuration);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_StopBGMWithFade(string soundName, float fadeTimer, float fadeDuration);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_StopAllSFX();


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_ToggleMuffleSFX(bool toMuffle, uint entityID = 0);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_ToggleMuffleBGM(bool toMuffle, uint entityID = 0);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_Stop(string soundName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_Pause(string soundName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_Resume(string soundName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_SetCategoryVolume(int category, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Sound_GetCategoryVolume(int category);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ChangeScene(string sceneName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Destroy(uint EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint CreateNewGameObject(string prefabName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(uint entityID, string baseClassName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstanceFromChildren(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool HasScriptInstance(uint entityID, string baseClassName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Animation_ChangeAnim(uint entityID, string prefabName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CloseGame();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Log(string msg);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string GetSceneName();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GetMousePos(out Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GetMouseUIPos(out Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsMousePressed(MouseButtons button);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsMouseReleased(MouseButtons button);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMouseScroll();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetStateCondition(uint entityID, object condition);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetStateTimer(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetAlpha(uint entityID, float alpha);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetColour(uint entityID, float r, float g, float b);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeTexture(uint entityID, string texture);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetMaxTime(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Animation_GetCurrFrameTime(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int Animation_GetCurrFrameNo(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Animation_Pause(uint entityID, bool val);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Animation_IsAnimFinished(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint Entity_GetParent(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint Entity_FindEntityWithID(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint[] Entity_FindEntitiesWithTag(string tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint[] Entity_GetAllChildren(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static double Time_GetFPS();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetLocalPosition(uint entityID, out Vector2 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Transform_GetTag(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ChangeText(uint entityID, string text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float TextGetWidth(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetRedColour(uint entityID, out float red);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetRedColour(uint entityID, ref float red);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetBlueColour(uint entityID, out float Blue);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetBlueColour(uint entityID, ref float Blue);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetGreenColour(uint entityID, out float Green);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetGreenColour(uint entityID, ref float Green);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string GetFilePath();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool GetParticlesActive(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SetParticlesActive(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Particles_SetColor(uint entityID, float r, float g, float b);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_StopForces(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Collider2D_GetCustomWidth(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Collider2D_GetCustomHeight(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Collider2D_SetCustomWidth(uint entityID, float width);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Collider2D_SetCustomHeight(uint entityID, float height);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Collider2D_GetxPivot(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Collider2D_GetyPivot(uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Collider2D_SetxPivot(uint entityID, float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Collider2D_SetyPivot(uint entityID, float x);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsDoneLoading();
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void SetCollisionLayer(uint entityID, int layer);
    }
}