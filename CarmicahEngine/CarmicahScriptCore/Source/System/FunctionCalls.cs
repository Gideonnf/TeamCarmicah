using System;
using System.Runtime.CompilerServices;

namespace Carmicah
{
    public static class FunctionCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetScale(uint entityID, out Vector2 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetScale(uint entityID, ref Vector2 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(uint entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ApplyForceWithTime(uint entityID, Vector2 dir, float magnitude, float lifeTime);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody_ApplyForce(uint entityID, Vector2 dir, float magnitude);

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
        internal extern static uint Entity_FindEntityWithName(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_PlaySFX(string soundName, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_PlayBGM(string soundName, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_SetVolume(string soundName, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_Stop(string soundName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_Pause(string soundName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_Resume(string soundName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Sound_SetCategoryVolume(int category, float volume);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ChangeScene(string sceneName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Destroy(uint EntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint CreateNewGameObject(string prefabName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(uint entityID);
    }
}