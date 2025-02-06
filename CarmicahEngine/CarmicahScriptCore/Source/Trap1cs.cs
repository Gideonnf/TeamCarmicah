using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class Trap1 : Entity
    {
        // Building-related variables
        public string TrapPrefabName = "Trap_1";
        public string TrapTranslucentPrefab = "TrapTranslucent";
        public bool IsLeft = false;
        public float depthVal = 3.45f;

        // Animation-related variables
        private bool isAnimPlaying = false;
        private float animTimer = 0.0f;
        private float cooldownTimer = 0.0f;
        private bool isOnCooldown = false;
        public float trapCooldown = 3.0f;

        // Building-related references
        Entity translucentTrap;
        Entity trapEntity;
        Entity TrapIcon;
        bool hovering = false;
        bool built = false;
        bool isActive = false;

        // Animation names
        public string idleAnim = "Trap_Idle";
        public string triggerAnim = "Trap_Trigger";

        public void OnCreate()
        {
            TrapIcon = FindEntityWithName("TrapIcon");
        }

        public void OnUpdate(float dt)
        {
            Entity pauseManager = FindEntityWithName("PauseManager");
            if (pauseManager != null && pauseManager.As<PauseManager>().IsPaused)
                return;

            // Handle building logic
            HandleBuildingLogic();

            // Handle trap animation logic
            if (trapEntity != null && trapEntity.mID != 0)
            {
                HandleTrapAnimation(dt);
            }
        }

        private void HandleBuildingLogic()
        {
            // incase trap entity gets destroyed, it needs to update to null here
            if (trapEntity != null && trapEntity.mID == 0)
            {
                trapEntity = null;
            }

            if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity != null)
            {
                if (translucentTrap == null && built == false)
                {
                    translucentTrap = CreateGameObject(TrapTranslucentPrefab);
                    translucentTrap.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                    translucentTrap.GetComponent<Transform>().Depth = depthVal;
                    if (IsLeft)
                    {
                        Vector2 scale = translucentTrap.GetComponent<Transform>().Scale;
                        translucentTrap.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                    }
                }
            }
            else if (TrapIcon != null && TrapIcon.As<TrapIcon>().trapEntity == null)
            {
                if (translucentTrap != null)
                {
                    translucentTrap.Destroy();
                    translucentTrap = null;

                    // if the player let go when its hovering a build spot
                    if (hovering && trapEntity == null)
                    {
                        // build a trap
                        built = true;
                        trapEntity = CreateGameObject(TrapPrefabName);
                        trapEntity.GetComponent<Transform>().Position = new Vector2(Position.x, Position.y);
                        trapEntity.GetComponent<Transform>().Depth = depthVal;
                        Sound.PlaySFX("trap_placement", 0.5f);

                        if (IsLeft)
                        {
                            Vector2 scale = trapEntity.GetComponent<Transform>().Scale;
                            trapEntity.GetComponent<Transform>().Scale = new Vector2(-scale.x, scale.y);
                        }
                    }
                }
            }
        }

        private void HandleTrapAnimation(float dt)
        {
            if (isAnimPlaying)
            {
                animTimer += dt;

                // Check if trigger animation has finished
                if (animTimer > trapEntity.GetComponent<Animation>().GetMaxTime())
                {
                    // Reset trap state
                    isAnimPlaying = false;
                    animTimer = 0.0f;
                    isOnCooldown = true;

                    // Return to idle animation
                    if (!string.IsNullOrEmpty(idleAnim))
                    {
                        trapEntity.ChangeAnim(idleAnim);
                    }
                }
            }

            // Handle cooldown timer
            if (isOnCooldown)
            {
                cooldownTimer += dt;
                if (cooldownTimer >= trapCooldown)
                {
                    isOnCooldown = false;
                    cooldownTimer = 0.0f;
                }
            }
        }

        public void OnCollide(uint entityID)
        {
            if (trapEntity == null || isAnimPlaying || isOnCooldown)
                return;

            Entity collidedEntity = FindEntityWithID(entityID);
            if (collidedEntity == null)
                return;

            // Check if entity can trigger the trap
            if (collidedEntity.GetTag() == "Enemy" || collidedEntity.GetTag() == "Player")
            {
                TriggerTrap();
            }
        }

        private void TriggerTrap()
        {
            if (trapEntity != null && !string.IsNullOrEmpty(triggerAnim))
            {
                isAnimPlaying = true;
                animTimer = 0.0f;
                trapEntity.ChangeAnim(triggerAnim);
                Sound.PlaySFX("trap_trigger", 0.5f);
            }
        }

        public void OnMouseEnter()
        {
            hovering = true;
        }

        public void OnMouseHover()
        {
            hovering = true;
        }

        public void OnMouseExit()
        {
            hovering = false;
        }
    }
}