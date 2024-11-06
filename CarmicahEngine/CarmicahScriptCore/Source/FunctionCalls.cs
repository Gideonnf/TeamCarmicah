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
    }
}
